//
// Created by oleglevin on 04.04.17.
//
#ifndef DUMMYCOMPILER_VISITORIRCODEGEN_H
#define DUMMYCOMPILER_VISITORIRCODEGEN_H

#include <llvm/IR/Constants.h>
#include <llvm/IR/Type.h>

#include "IVisitor.h"
#include "IRCodeGenContext.h"

class VisitorIRCodeGen : public IVisitor {
public:
    VisitorIRCodeGen(IRCodeGenContext& ctx) :
            mCtx(ctx) { }

    virtual ~VisitorIRCodeGen() override = default;

    virtual bool isIdentifierInGlobalScope (const std::string& val, llvm::Function* f, llvm::BasicBlock*& inBlock);

    virtual void visit(struct NIntegerConst *aConst, void* val = nullptr) override;

    virtual void visit(struct NDoubleConst *aConst, void* val = nullptr) override;

    virtual void visit(struct NIdentifier *identifier, void* val = nullptr) override;

    virtual void visit(struct NVariableDeclaration *declaration,  void* val = nullptr) override;

    virtual void visit(struct NBlock *block, void* val = nullptr) override;

    virtual void visit(struct NAssignment *assignment, void* val = nullptr) override;

    virtual void visit(struct NFunctionDeclaration *declaration, void* val = nullptr) override;

    virtual void visit(struct NFunctionCall *call, void* val = nullptr) override;

    virtual void visit(struct NExpressionStatement *statement, void* val = nullptr) override;

    virtual void visit(struct NBinaryOp *op, void* val = nullptr) override;

    virtual void visit(struct NReturnStatement *statement, void* val = nullptr) override;

    virtual void visit(class NIfElseStatement *statement, void* val = nullptr) override;

    virtual void visit(class NForLoop *loop, void *pVoid) override;

    virtual void visit(class NStructField *field, void *pVoid) override;

    virtual void visit(class NStructDecl *decl, void *pVoid) override;

    virtual void visit(class NStructFieldAccess *decl, void *pVoid) override;

    virtual void visit(class NStructMethodCall *decl, void *pVoid) override;

private:
    IRCodeGenContext& mCtx;

};

#endif //DUMMYCOMPILER_VISITORIRCODEGEN_H
