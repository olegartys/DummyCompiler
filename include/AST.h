//
// Created by olegartys on 30.03.17.
//

#ifndef DUMMYCOMPILER_NODE_H
#define DUMMYCOMPILER_NODE_H

#include <iostream>
#include <string>
#include <Log.h>

#define LOG_TAG "AST"

class NVariableDeclaration;
class NStatement;
class NExpression;

using VariableList = std::vector<std::shared_ptr<NStatement>>;
using StatementList = std::vector<std::shared_ptr<NStatement>>;
using ExpressionList = std::vector<std::shared_ptr<NExpression>>;

class BaseNode {
public:
    virtual ~BaseNode() = default;
    virtual void codeGen() {}
};

class NExpression : public BaseNode { };

class NStatement : public BaseNode { };

class NIntegerConst : public NExpression {
public:
    long long mVal;

    NIntegerConst(long long value) : mVal(value) { Log::info(LOG_TAG, "[NIntegerConst]");}
    virtual void codeGen() override { }
};

class NDoubleConst : public NExpression {
public:
    double mVal;

    NDoubleConst(double value) : mVal(value) { Log::info(LOG_TAG, "[NDoubleConst]"); }
    virtual void codeGen() override { }
};

class NIdentifier : public NExpression {
public:
    std::string mVal;

    NIdentifier(const std::string& name) : mVal(name) { Log::info(LOG_TAG, "[NIdentifier]"); }
    virtual void codeGen() override { }
};

class NVariableDeclaration : public NStatement {
public:
    const NIdentifier& type;
    NIdentifier& id;
    std::shared_ptr<NExpression> assignmentExpr;

    NVariableDeclaration(const NIdentifier& type, NIdentifier& id) :
            type(type), id(id) { Log::info(LOG_TAG, "[NVariableDeclaration]"); }

    NVariableDeclaration(const NIdentifier& type, NIdentifier& id, std::shared_ptr<NExpression> assignmentExpr) :
            type(type), id(id), assignmentExpr(assignmentExpr) { Log::info(LOG_TAG, "[NVariableDeclaration]"); }

    virtual void codeGen() override { }
};

class NBlock : public NExpression {
public:
    StatementList statements;

    NBlock() { Log::info(LOG_TAG, "[NBlock]"); }

    virtual void codeGen() override { }

};

class NAssignment : public NExpression {
public:
    int op;
    NExpression& lhs;
    NExpression& rhs;

    NAssignment(NIdentifier& lhs, NExpression& rhs) :
            lhs(lhs), rhs(rhs) { Log::info(LOG_TAG, "[NAssignment]"); }

    virtual void codeGen() override { }

};

class NFunctionDeclaration : public NStatement {
public:
    const NIdentifier& type;
    const NIdentifier& id;
    VariableList arguments;
    NBlock& block;

    NFunctionDeclaration(const NIdentifier& type, const NIdentifier& id,
            const VariableList& args, NBlock& block) :
        type(type), id(id), arguments(args), block(block) { Log::info(LOG_TAG, "[NFunctionDeclaration]"); }

    virtual void codeGen() override { }

};

class NFunctionCall : public NExpression {
public:
    const NIdentifier& id;
    ExpressionList arguments;

    NFunctionCall(const NIdentifier& id, ExpressionList& arguments) :
            id(id), arguments(arguments) { Log::info(LOG_TAG, "[NFunctionCall]"); }
    NFunctionCall(const NIdentifier& id) : id(id) { Log::info(LOG_TAG, "[NFunctionCall]"); }

    virtual void codeGen() override { }
};

class NExpressionStatement : public NStatement {
public:
    NExpression& expression;
    NExpressionStatement(NExpression& expression) :
            expression(expression) { Log::info(LOG_TAG, "[NExpressionStatement]"); }

    virtual void codeGen() override { }
};

class NBinaryOp : public NExpression {
public:
    int op;
    NExpression& lhs;
    NExpression& rhs;
    NBinaryOp(NExpression& lhs, int op, NExpression& rhs) :
            lhs(lhs), rhs(rhs), op(op) { }

    virtual void codeGen() override { }
};

#endif //DUMMYCOMPILER_NODE_H
