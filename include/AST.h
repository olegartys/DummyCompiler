//
// Created by olegartys on 30.03.17.
//

#ifndef DUMMYCOMPILER_NODE_H
#define DUMMYCOMPILER_NODE_H

#include <iostream>
#include <string>
#include <Log.h>
#include "IVisitor.h"

#undef LOG_TAG
#define LOG_TAG "AST"

class NVariableDeclaration;
class NStatement;
class NExpression;

using VariableList   = std::vector<std::shared_ptr<NStatement>>;
using StatementList  = std::vector<std::shared_ptr<NStatement>>;
using ExpressionList = std::vector<std::shared_ptr<NExpression>>;

class BaseNode {
public:
    virtual ~BaseNode() = default;

    virtual void accept(IVisitor& v, void* usr = nullptr) = 0;
};

class NExpression : public BaseNode { };

class NStatement : public BaseNode { };

class NIntegerConst : public NExpression {
public:
    std::string mVal;

    NIntegerConst(const std::string& value) : mVal(value) {
        Log::info(LOG_TAG, "[NIntegerConst] created");
    }

    virtual void accept(IVisitor& v, void* usr = nullptr) override { v.visit(this, usr); }
};

class NDoubleConst : public NExpression {
public:
    std::string mVal;

    NDoubleConst(const std::string& value) : mVal(value) {
        Log::info(LOG_TAG, "[NDoubleConst] created");
    }

    virtual void accept(IVisitor& v, void* usr = nullptr) override { v.visit(this, usr); }
};

class NIdentifier : public NExpression {
public:
    std::string mVal;

    NIdentifier(const std::string& name) : mVal(name) {
        Log::info(LOG_TAG, "[NIdentifier] created");
    }

    virtual void accept(IVisitor& v, void* usr = nullptr) override { v.visit(this, usr); }
};

class NVariableDeclaration : public NStatement {
public:
    std::shared_ptr<NIdentifier> mType;
    std::shared_ptr<NIdentifier> mId;
    std::shared_ptr<NExpression> mAsignExpr;

    NVariableDeclaration(std::shared_ptr<NIdentifier>& type, std::shared_ptr<NIdentifier>& id) :
            mType(type), mId(id) {
        Log::info(LOG_TAG, "[NVariableDeclaration] created");
    }

    NVariableDeclaration(std::shared_ptr<NIdentifier>& type, std::shared_ptr<NIdentifier>& id,
                         std::shared_ptr<NExpression>& assignmentExpr) :
            mType(type), mId(id), mAsignExpr(assignmentExpr) {
        Log::info(LOG_TAG, "[NVariableDeclaration] created");
    }

    virtual void accept(IVisitor& v, void* usr = nullptr) override { v.visit(this, usr); }
};

class NBlock : public NExpression {
public:
    StatementList mStatements;

    NBlock() { Log::info(LOG_TAG, "[NBlock] created"); }

    virtual void accept(IVisitor& v, void* usr = nullptr) override { v.visit(this, usr); }

};

class NAssignment : public NExpression {
public:
    int mOp;
    std::shared_ptr<NIdentifier> mLhs;
    std::shared_ptr<NExpression> mRhs;

    NAssignment(std::shared_ptr<NIdentifier>& lhs, std::shared_ptr<NExpression>& rhs) :
            mLhs(lhs), mRhs(rhs) {
        Log::info(LOG_TAG, "[NAssignment] created");
    }

    virtual void accept(IVisitor& v, void* usr = nullptr) override { v.visit(this, usr); }

};

class NFunctionDeclaration : public NStatement {
public:
    std::shared_ptr<NIdentifier> mType;
    std::shared_ptr<NIdentifier> mId;
    std::shared_ptr<VariableList> mArguments;
    std::shared_ptr<NBlock> mBlock;

    NFunctionDeclaration(std::shared_ptr<NIdentifier>& type, std::shared_ptr<NIdentifier>& id,
                         std::shared_ptr<VariableList>& args, std::shared_ptr<NBlock>& block) :
            mType(type), mId(id), mArguments(args), mBlock(block) {

        Log::info(LOG_TAG, "[NFunctionDeclaration] created");
    }

    virtual void accept(IVisitor& v, void* usr = nullptr) override { v.visit(this, usr); }

};

class NFunctionCall : public NExpression {
public:
    std::shared_ptr<NIdentifier> mId;
    std::shared_ptr<ExpressionList> mArguments;

    NFunctionCall(std::shared_ptr<NIdentifier>& id, std::shared_ptr<ExpressionList>& arguments) :
            mId(id), mArguments(arguments) {
        Log::info(LOG_TAG, "[NFunctionCall] created");
    }

    NFunctionCall(std::shared_ptr<NIdentifier>& id) :
            mId(id) {
        Log::info(LOG_TAG, "[NFunctionCall] created");
    }

    virtual void accept(IVisitor& v, void* usr = nullptr) override { v.visit(this, usr); }
};

class NExpressionStatement : public NStatement {
public:
    std::shared_ptr<NExpression> mExpression;

    NExpressionStatement(std::shared_ptr<NExpression>& expression) :
            mExpression(expression) {
        Log::info(LOG_TAG, "[NExpressionStatement] created");
    }

    virtual void accept(IVisitor& v, void* usr = nullptr) override { v.visit(this, usr); }
};

class NBinaryOp : public NExpression {
public:
    int mOp;
    std::shared_ptr<NExpression> mLhs;
    std::shared_ptr<NExpression> mRhs;

    NBinaryOp(std::shared_ptr<NExpression>& lhs, int op, std::shared_ptr<NExpression>& rhs) :
            mLhs(lhs), mRhs(rhs), mOp(op) {
        Log::info(LOG_TAG, "[NBinaryOp] created");
    }

    virtual void accept(IVisitor& v, void* usr = nullptr) override { v.visit(this, usr); }
};

class NReturnStatement : public NStatement {
public:
    // std::shared_ptr<NIdentifier> mReturnIdent;
    std::shared_ptr<NExpression> mExpression;

//    NReturnStatement(std::shared_ptr<NIdentifier>& returnIdent) :
//            mReturnIdent(returnIdent) {
//        Log::info(LOG_TAG, "[NReturnStatement] created");
//    }

    NReturnStatement(std::shared_ptr<NExpression>& expression) :
            mExpression(expression) {
        Log::info(LOG_TAG, "[NReturnStatement] created");
    }

    virtual void accept(IVisitor& v, void* usr = nullptr) override { v.visit(this, usr); }
};

#endif //DUMMYCOMPILER_NODE_H
