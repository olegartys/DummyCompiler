//
// Created by olegartys on 01.04.17.
//

#ifndef DUMMYCOMPILER_IVISITOR_H
#define DUMMYCOMPILER_IVISITOR_H

class IVisitor {
public:
    virtual ~IVisitor() = default;

    virtual void visit(class NIntegerConst*, void* = nullptr) = 0;
    virtual void visit(class NDoubleConst*, void* = nullptr) = 0;
    virtual void visit(class NIdentifier*, void* = nullptr) = 0;
    virtual void visit(class NVariableDeclaration*, void* = nullptr) = 0;
    virtual void visit(class NBlock*, void* = nullptr) = 0;
    virtual void visit(class NAssignment*, void* = nullptr) = 0;
    virtual void visit(class NFunctionDeclaration*, void* = nullptr) = 0;
    virtual void visit(class NFunctionCall*, void* = nullptr) = 0;
    virtual void visit(class NExpressionStatement*, void* = nullptr) = 0;
    virtual void visit(class NBinaryOp*, void* = nullptr) = 0;
    virtual void visit(class NReturnStatement*, void* = nullptr) = 0;
    virtual void visit(class NIfElseStatement*, void* = nullptr) = 0;
    virtual void visit(class NForLoop*, void* = nullptr) = 0;
};

#endif //DUMMYCOMPILER_IVISITOR_H
