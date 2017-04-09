//
// Created by oleglevin on 04.04.17.
//

#include <Log.h>

#include <llvm/IR/PassManager.h>
#include <llvm/IR/IRPrintingPasses.h>
#include <llvm/IR/IRBuilder.h>
#include <VisitorIRCodeGen.h>

#undef LOG_TAG
#define LOG_TAG "IRCodeGenContext"

using namespace llvm;

IRCodeGenContext::IRCodeGenContext(const FILE* outputFile) :
        mOutputFile(outputFile) {

    mBuilder = new IRBuilder<>(llvm::getGlobalContext());
    mModule = new Module("main_module", llvm::getGlobalContext());

    mVisitorIRCodeGen = std::make_shared<VisitorIRCodeGen>(*this);
}

void IRCodeGenContext::genIRCode(NBlock& rootBlock) {
    Log::debug(LOG_TAG, "CodeGen started");

    // Create main function which is wrapper for all the global statements
    // Here all the IR compiling starts.
    FunctionType *globalFunctionType = FunctionType::get(Type::getInt32Ty(getGlobalContext()), false);
    mGlobalFunction = Function::Create(globalFunctionType, GlobalValue::ExternalLinkage, "main", mModule);
    BasicBlock* bblock = BasicBlock::Create(getGlobalContext(), mGlobalFunction->getName(), mGlobalFunction);

    pushBlock(bblock);
    //mBuilder->SetInsertPoint(bblock); // Set root block for builder to insert instructions
    auto stub = stubVal();
    rootBlock.accept(*mVisitorIRCodeGen, &(stub)); // FIXME: this is stub for creatng some empty value. Is this correct?
    popBlock();

    Log::debug(LOG_TAG, "CodeGen finished");

    // Dump generated IR to file
    auto llvmRawStream = new raw_fd_ostream(fileno(const_cast<FILE*>(mOutputFile)), false);
    PrintModulePass pm(*llvmRawStream);
    pm.run(*mModule);
}

const llvm::Type* IRCodeGenContext::getTypeOf(const std::string& type) const {
    if ("int" == type) {
        return mBuilder->getInt32Ty();
    } else if ("double" == type) {
        return mBuilder->getDoubleTy();
    } else {
        return nullptr;
    }
}
