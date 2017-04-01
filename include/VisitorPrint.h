//
// Created by olegartys on 01->04->17->
//

#ifndef DUMMYCOMPILER_VISITORPRINT_H
#define DUMMYCOMPILER_VISITORPRINT_H

#include "IVisitor.h"

#define LOG_TAG "VisitorPrint"

class VisitorPrint : public IVisitor {
public:
    ~VisitorPrint() override = default;

    void visit(struct NIntegerConst *aConst) override {
        Log::info(LOG_TAG, "[NIntegerConst]: val={}", aConst->mVal);
    }

    void visit(struct NDoubleConst *aConst) override {
        Log::info(LOG_TAG, "[NDoubleConst]: val={}", aConst->mVal);
    }

    void visit(struct NIdentifier *identifier) override {
        Log::info(LOG_TAG, "[NIdentifier]: val={}", identifier->mVal);
    }

    void visit(struct NVariableDeclaration *declaration) override {
        Log::info(LOG_TAG, "[NVariableDeclaration]: type={} id={}", declaration->mType->mVal, declaration->mId->mVal);
        if (declaration->mAsignExpr)
            declaration->mAsignExpr->accept(*this);
    }

    void visit(struct NBlock *block) override {
        Log::info(LOG_TAG, "[NBlock]: visited");
            for (const auto& arg: block->mStatements)
                arg->accept(*this);
    }

    void visit(struct NAssignment *assignment) override {
        if (assignment->mLhs)
            assignment->mLhs->accept(*this);
        Log::info(LOG_TAG, "[NAssignment]: op={}", assignment->mOp);
        if (assignment->mRhs)
            assignment->mRhs->accept(*this);
    }

    void visit(struct NFunctionDeclaration *declaration) override {
        if (declaration->mArguments)
            for (const auto& arg: *declaration->mArguments)
                arg->accept(*this);
        Log::info(LOG_TAG, "[NFunctionDeclaration]: type={} id={}", declaration->mType->mVal, declaration->mId->mVal);
        if (declaration->mBlock)
            declaration->mBlock->accept(*this);
    }

    void visit(struct NFunctionCall *call) override {
        if (call->mArguments)
            for (const auto& arg: *call->mArguments)
                arg->accept(*this);
        Log::info(LOG_TAG, "[NFunctionCall]: id={}", call->mId->mVal);
    }

    void visit(struct NExpressionStatement *statement) override {
        Log::info(LOG_TAG, "[NExpressionStatement] visited");
        statement->mExpression->accept(*this);
    }

    void visit(struct NBinaryOp *op) override {
        if (op->mLhs)
            op->mLhs->accept(*this);
        Log::info(LOG_TAG, "[NBinaryOp]: op={}", op->mOp);
        if (op->mRhs)
            op->mRhs->accept(*this);
    }

    void visit(class NReturnStatement *statement) override {
        Log::info(LOG_TAG, "[NReturnStatement]: visited");
        if (statement->mReturnIdent)
            statement->mReturnIdent->accept(*this);
        if (statement->mNumericVal)
            statement->mNumericVal->accept(*this);
    }
};

#endif //DUMMYCOMPILER_VISITORPRINT_H
