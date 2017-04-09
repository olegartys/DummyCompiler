//
// Created by oleglevin on 06.04.17.
//

#include <VisitorIRCodeGen.h>

#include <llvm/IR/Function.h>
#include <llvm/IR/Module.h>

#define YY_NULLPTR nullptr // FIXME: bison YY_NULLPTR bug
#include "flex_bison_output/BisonParser.hpp"

using namespace llvm;

#undef LOG_TAG
#define LOG_TAG "VisitorIRCodeGen"

void VisitorIRCodeGen::visit(struct NIntegerConst *aConst, void* val) {
    Log::info(LOG_TAG, "Constructing NIntegerConst");
    llvm::Value** retVal = (llvm::Value**)val;

    int rVal = -1;
    try {
        rVal = std::stoi(aConst->mVal);
    } catch (std::invalid_argument& e) {
        Log::error(LOG_TAG, "Error integer value \"{}\".", aConst->mVal);
        exit(EXIT_FAILURE);
    } catch (std::out_of_range& e) {
        Log::error(LOG_TAG, "Value \"{}\" is out of range of int32 type.", aConst->mVal);
        exit(EXIT_FAILURE);
    }

    llvm::Value* v = llvm::ConstantInt::get(llvm::Type::getInt32Ty(llvm::getGlobalContext()), rVal);

    *retVal = v;
}

void VisitorIRCodeGen::visit(class NDoubleConst *aConst, void *val) {
    Log::info(LOG_TAG, "Constructing NDoubleConst");
    llvm::Value** retVal = (llvm::Value**)val;

    double rVal = -1;
    try {
        rVal = std::stof(aConst->mVal);
    } catch (std::invalid_argument& e) {
        Log::error(LOG_TAG, "Error double value \"{}\".", aConst->mVal);
        exit(EXIT_FAILURE);
    } catch (std::out_of_range& e) {
        Log::error(LOG_TAG, "Value \"{}\" is out of range of double type.", aConst->mVal);
        exit(EXIT_FAILURE);
    }

    llvm::Value* v = llvm::ConstantFP::get(llvm::Type::getDoubleTy(llvm::getGlobalContext()), rVal);

    *retVal = v;
}

void VisitorIRCodeGen::visit(struct NVariableDeclaration *declaration,  void* val) {
    Log::info(LOG_TAG, "Constructing NVariableDeclaration");
    llvm::Value** retVal = (llvm::Value**)val;

    Type* varType;
    Value* varDeclaration;

    // Check variable type
    if ("int" == declaration->mType->mVal) {
        varType = mCtx.builder().getInt32Ty();
    } else if ("double" == declaration->mType->mVal) {
        varType = mCtx.builder().getDoubleTy();
    } else {
        Log::error(LOG_TAG, "\"{}\" type is undefined", declaration->mType->mVal);
        exit(EXIT_FAILURE);
    }

    // Check whether variable is already declared
    bool isDeclared = mCtx.topBlock()->isInLocalSymTable(declaration->mId->mVal);

    // Create new on stack variable allocation instruction and add varialbe into symtable
    if (!isDeclared) {
        varDeclaration = new AllocaInst(varType, 0, declaration->mId->mVal, mCtx.topBlock()->mLlvmBlock);
        mCtx.topBlock()->mSymTable[declaration->mId->mVal] = varDeclaration;
    } else {
        Log::error(LOG_TAG, "Variable \"{}\" already declared", declaration->mId->mVal);
        exit(EXIT_FAILURE);
    }

    // Generate and assign rvalue for the variable if exists
    if (declaration->mAsignExpr) {
        Value* varStoreValue = mCtx.stubVal();
        NAssignment assignment(declaration->mId, declaration->mAsignExpr);
        assignment.accept(*this, &varStoreValue);
    }

    *retVal = varDeclaration;
}

void VisitorIRCodeGen::visit(struct NBlock *block, void* val) {
    Log::info(LOG_TAG, "Constructing NBlock");
    llvm::Value** retVal = (llvm::Value**)val;

    llvm::Value* lastVal = nullptr;
    for (const auto& st: block->mStatements) {
        st->accept(*this, &lastVal);
    }

    *retVal = lastVal;
}

void VisitorIRCodeGen::visit(class NIdentifier *identifier, void *val) {
    Log::info(LOG_TAG, "Constructing NIdentifier");
    llvm::Value** retVal = (llvm::Value**)val;

    bool isDeclared = mCtx.topBlock()->isInLocalSymTable(identifier->mVal);
    if (!isDeclared) {
        Log::error(LOG_TAG, "Variable \"{}\" was not declared", identifier->mVal);
        exit(EXIT_FAILURE);
    } else {
        auto loadInstVal = new LoadInst(mCtx.topBlock()->mSymTable[identifier->mVal], "__tmp_val", mCtx.topBlock()->mLlvmBlock);
        *retVal = loadInstVal;
    }
}

void VisitorIRCodeGen::visit(class NAssignment *assignment, void *val) {
    Log::info(LOG_TAG, "Constructing NAssignment");
    llvm::Value** retVal = (llvm::Value**)val;

    // Check whether the variable is in the namespace
    bool isDeclared = mCtx.topBlock()->isInLocalSymTable(assignment->mLhs->mVal);
    if (!isDeclared) {
        Log::error(LOG_TAG, "Variable \"{}\" was not declared", assignment->mLhs->mVal);
        exit(EXIT_FAILURE);
    } else {
        // Generate Value* for an rhs
        auto rvalExprVal = mCtx.stubVal();
        assignment->mRhs->accept(*this, &rvalExprVal);

        // Generate store Value*
        auto storeInstVal = new StoreInst(rvalExprVal, mCtx.topBlock()->mSymTable[assignment->mLhs->mVal], false,
                                          mCtx.topBlock()->mLlvmBlock);

        *retVal = storeInstVal;
    }
}

void VisitorIRCodeGen::visit(class NFunctionDeclaration *declaration, void *val) {
    Log::info(LOG_TAG, "Constructing NFunctionDeclaration");
    llvm::Value** retVal = (llvm::Value**)val;

    // Fiil argument types array
    std::vector<Type*> argTypes;
    for (const auto& arg: *declaration->mArguments) {
        auto type = dynamic_cast<NVariableDeclaration*>(arg.get())->mType->mVal;
        if (!mCtx.getTypeOf(type)) {
            Log::error(LOG_TAG, "Unknown variable type \"{}\".", type);
            exit(EXIT_FAILURE);
        }
        argTypes.push_back(const_cast<Type*>(mCtx.getTypeOf(type)));
    }

    // Create function type (return and arguments)
    auto _funcType = mCtx.getTypeOf(declaration->mType->mVal);
    if (!_funcType) {
        Log::error(LOG_TAG, "Unknown function return type \"{}\".", declaration->mType->mVal);
        exit(EXIT_FAILURE);
    }
    FunctionType* functionType = FunctionType::get(const_cast<Type*>(_funcType), argTypes, false);

    // Create function and block for it
    Function* function = Function::Create(functionType, GlobalValue::InternalLinkage, declaration->mId->mVal, &mCtx.module());
    BasicBlock* functionBlock = BasicBlock::Create(getGlobalContext(), declaration->mId->mVal, function, 0);

    mCtx.pushBlock(functionBlock);

    // Set new block for builder to insert instructions
    //mCtx.builder().SetInsertPoint(mCtx.topBlock()->mLlvmBlock);

    // Generate IR for arguments
    for (const auto& arg: *declaration->mArguments) {
        arg->accept(*this, val);
    }

    // Generate IR for block of code inside it
    declaration->mBlock->accept(*this, val);

    mCtx.popBlock();

    // Set old block for builder to insert instructions
   // mCtx.builder().SetInsertPoint(mCtx.topBlock()->mLlvmBlock);

    *retVal = function;
}

void VisitorIRCodeGen::visit(class NFunctionCall *call, void *val) {
    Log::info(LOG_TAG, "Constructing NFunctionCall");
    llvm::Value** retVal = (llvm::Value**)val;

    auto caleeFunc = mCtx.module().getFunction(call->mId->mVal);
    if (!caleeFunc) {
        Log::error(LOG_TAG, "Unknown function reference.");
        exit(EXIT_FAILURE);
    } else if (caleeFunc->arg_size() != call->mArguments->size()) {
        Log::error(LOG_TAG, "Incorrect amount fo argmunets during function call.");
        exit(EXIT_FAILURE);
    } else {
        std::vector<Value *> funcArgs;
        for (const auto& arg: *call->mArguments) {
            arg->accept(*this, val);
            funcArgs.push_back(*(Value**)val);
        }

        // Check types of the variables passed and in function definition
        auto caleeArgIter = caleeFunc->arg_begin();
        for (int i = 0; i <  caleeFunc->arg_size(); ++i, ++caleeArgIter) {
            if (caleeArgIter->getType()->getTypeID() != (funcArgs[i]->getType()->getTypeID())) {
                Log::error(LOG_TAG, "Incorrect types of argument passed.");
                exit(EXIT_FAILURE);
            }
        }

        *retVal = CallInst::Create(caleeFunc, funcArgs, "__tmp_call", mCtx.topBlock()->mLlvmBlock);
    }
}

void VisitorIRCodeGen::visit(class NBinaryOp *op, void *val) {
    Log::info(LOG_TAG, "Constructing NBinaryOp");
    llvm::Value** retVal = (llvm::Value**)val;

    Value *L = mCtx.stubVal();
    op->mLhs->accept(*this, &L);

    Value *R = mCtx.stubVal();
    op->mRhs->accept(*this, &R);
    if (!L || !R) {
        Log::critical(LOG_TAG, "AST evaluation error.");
        exit(EXIT_FAILURE);
    }

    // TODO: comparison operators
    // FIXME: only double arithmetic
    switch (op->mOp) {
        case yy::BisonParser::token::TOK_PLUS:
            *retVal = mCtx.builder().CreateFAdd(L, R, "__tmp_val_add");
            break;
        case yy::BisonParser::token::TOK_MINUS:
            *retVal =  mCtx.builder().CreateFSub(L, R, "__tmp_val_sub");
            break;
        case yy::BisonParser::token::TOK_MUL:
            *retVal = mCtx.builder().CreateFMul(L, R, "__tmp_val_mul");
            break;
        case yy::BisonParser::token::TOK_DIV:
            *retVal = mCtx.builder().CreateFDiv(L, R, "__tmp_val_div");
            break;
        case yy::BisonParser::token::TOK_CLT:
            // FIXME: only int are comparable
            if (L->getType() != R->getType()) {
                Log::error(LOG_TAG, "Can't compare values with different types.");
                exit(EXIT_FAILURE);

            // Int32 comparison
            } else if (L->getType()->isIntegerTy()) {
                *retVal = mCtx.builder().CreateICmp(ICmpInst::FCMP_OLT, L, R, "__tmp_val_clt");

            // Double comparison
            } else if (L->getType()->isDoubleTy()) {
                *retVal = mCtx.builder().CreateFCmp(FCmpInst::FCMP_OLT, L, R, "__tmp_val_clt");
            } else {
                Log::error(LOG_TAG, "Uncompairable types.");
                exit(EXIT_FAILURE);
            }
            break;

        default:
            Log::error(LOG_TAG, "Invalid binary operator \"{}\".", op->mOp);
            exit(EXIT_FAILURE);
    }
}

void VisitorIRCodeGen::visit(class NReturnStatement *statement, void *val) {
    Log::info(LOG_TAG, "Constructing NReturnStatement");
    llvm::Value** retVal = (llvm::Value**)val;

    statement->mExpression->accept(*this, val);

    auto returnInst = mCtx.builder().CreateRet((Value*)val);
    returnInst->Create(getGlobalContext(), *(Value**)val, mCtx.topBlock()->mLlvmBlock);

    auto funcTypeId = mCtx.topBlock()->mLlvmBlock->getParent()->getReturnType()->getTypeID();/*getType()->getTypeID();*/
    auto exprTypeId = (*(Value**)val)->getType()->getTypeID();

    if (funcTypeId != exprTypeId) {
        Log::error(LOG_TAG, "Function and return expression types are not equal: {} {}", funcTypeId, exprTypeId);
        exit(EXIT_FAILURE);
    }
}

void VisitorIRCodeGen::visit(struct NExpressionStatement *statement, void* val) {
    Log::info(LOG_TAG, "Constructing NExpressionStatement");
    statement->mExpression->accept(*this, val);
}
