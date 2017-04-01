//
// Created by olegartys on 01.04.17.
//

#ifndef DUMMYCOMPILER_IVISITOR_H
#define DUMMYCOMPILER_IVISITOR_H

class IVisitor {
public:
    virtual ~IVisitor() = default;

    virtual void visit(class NIntegerConst*) = 0;
    virtual void visit(class NDoubleConst*) = 0;
    virtual void visit(class NIdentifier*) = 0;
    virtual void visit(class NVariableDeclaration*) = 0;
    virtual void visit(class NBlock*) = 0;
    virtual void visit(class NAssignment*) = 0;
    virtual void visit(class NFunctionDeclaration*) = 0;
    virtual void visit(class NFunctionCall*) = 0;
    virtual void visit(class NExpressionStatement*) = 0;
    virtual void visit(class NBinaryOp*) = 0;
    virtual void visit(class NReturnStatement*) = 0;

};

#endif //DUMMYCOMPILER_IVISITOR_H
