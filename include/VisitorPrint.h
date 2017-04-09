//
// Created by olegartys on 01->04->17->
//

#ifndef DUMMYCOMPILER_VISITORPRINT_H
#define DUMMYCOMPILER_VISITORPRINT_H

#include "IVisitor.h"

#undef LOG_TAG
#define LOG_TAG "VisitorPrint"

class VisitorPrint : public IVisitor {
public:
    ~VisitorPrint() override = default;

    void visit(struct NIntegerConst *aConst, void* = nullptr) override {
        Log::info(LOG_TAG, "[NIntegerConst]: val={}", aConst->mVal);
    }

    void visit(struct NDoubleConst *aConst, void* = nullptr) override {
        Log::info(LOG_TAG, "[NDoubleConst]: val={}", aConst->mVal);
    }

    void visit(struct NIdentifier *identifier, void* = nullptr) override {
        Log::info(LOG_TAG, "[NIdentifier]: val={}", identifier->mVal);
    }

    void visit(struct NVariableDeclaration *declaration, void* = nullptr) override {
        Log::info(LOG_TAG, "[NVariableDeclaration]: visited", declaration->mType->mVal, declaration->mId->mVal);
        declaration->mType->accept(*this);
        declaration->mId->accept(*this);
        if (declaration->mAsignExpr)
            declaration->mAsignExpr->accept(*this);
    }

    void visit(struct NBlock *block, void* = nullptr) override {
        Log::info(LOG_TAG, "[NBlock]: visited");
            for (const auto& arg: block->mStatements)
                arg->accept(*this);
    }

    void visit(struct NAssignment *assignment, void* = nullptr) override {
        if (assignment->mLhs)
            assignment->mLhs->accept(*this);
        Log::info(LOG_TAG, "[NAssignment]: op={}", assignment->mOp);
        if (assignment->mRhs)
            assignment->mRhs->accept(*this);
    }

    void visit(struct NFunctionDeclaration *declaration, void* = nullptr) override {
        Log::info(LOG_TAG, "[NFunctionDeclaration]: visited");
        declaration->mType->accept(*this);
        declaration->mId->accept(*this);
        if (declaration->mArguments)
            for (const auto& arg: *declaration->mArguments)
                arg->accept(*this);
        if (declaration->mBlock)
            declaration->mBlock->accept(*this);
    }

    void visit(struct NFunctionCall *call, void* = nullptr) override {
        Log::info(LOG_TAG, "[NFunctionCall]: visited");
        call->mId->accept(*this);
        if (call->mArguments)
            for (const auto& arg: *call->mArguments)
                arg->accept(*this);
    }

    void visit(struct NExpressionStatement *statement, void* = nullptr) override {
        Log::info(LOG_TAG, "[NExpressionStatement] visited");
        statement->mExpression->accept(*this);
    }

    void visit(struct NBinaryOp *op, void* = nullptr) override {
        if (op->mLhs)
            op->mLhs->accept(*this);
        Log::info(LOG_TAG, "[NBinaryOp]: op={}", op->mOp);
        if (op->mRhs)
            op->mRhs->accept(*this);
    }

    void visit(class NReturnStatement *statement, void* = nullptr) override {
        Log::info(LOG_TAG, "[NReturnStatement]: visited");
        if (statement->mExpression)
            statement->mExpression->accept(*this);
    }

    virtual void visit(class NIfElseStatement *statement, void* = nullptr) override {
        Log::info(LOG_TAG, "[NIfElseStatement]: visited");
        statement->mCondExpression->accept(*this);
        statement->mIfBlock->accept(*this);
        if (statement->mElseBlock) {
            statement->mElseBlock->accept(*this);
        }
    }

    virtual void visit(class NForLoop *loop, void* = nullptr) override {
        Log::info(LOG_TAG, "[NForLoop]: visited");
        loop->mStart->accept(*this);
        loop->mEnd->accept(*this);
        loop->mStep->accept(*this);
        loop->mBlock->accept(*this);
    }

    virtual void visit(class NStructField *field, void *pVoid) override {
        Log::info(LOG_TAG, "[NStructField]: visited. access = {}", field->mAccessSpecifier);
        field->mDeclaration->accept(*this);
    }

    virtual void visit(class NStructDecl *decl, void *pVoid) override {
        Log::info(LOG_TAG, "[NStructDecl]: visited");
        decl->mIdentifier->accept(*this);
        decl->mStructFields->accept(*this);
    }

};

#endif //DUMMYCOMPILER_VISITORPRINT_H
