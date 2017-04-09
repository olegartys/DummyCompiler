//
// Created by oleglevin on 04.04.17.
//

#ifndef DUMMYCOMPILER_IRCODEGENCONTEXT_H
#define DUMMYCOMPILER_IRCODEGENCONTEXT_H


#include <string>
#include <memory>
#include <map>
#include <list>

#include <AST.h>

#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>

// #include "VisitorIRCodeGen.h"

/**
 * Logical block of code with it's own local namespace
 */
struct IRCodeGenBlock {
    inline bool isInLocalSymTable(const std::string& val) const {
        bool isDeclared = true;
        try {
            mSymTable.at(val);
        } catch (std::out_of_range& e) {
            isDeclared = false;
        }
        return isDeclared;
    }

    llvm::BasicBlock* mLlvmBlock;
    std::map<std::string, llvm::Value*> mSymTable;
};

class IRCodeGenContext {
public:
    template <typename T> using Ptr = std::shared_ptr<T>;

public:
    IRCodeGenContext(const FILE* outputFile = stdout);
    virtual ~IRCodeGenContext() = default;

    IRCodeGenContext(const IRCodeGenContext&) = delete;
    IRCodeGenContext& operator= (const IRCodeGenContext&) = delete;

    /** Interface **/
    Ptr<IRCodeGenBlock>& topBlock() { return mBlockList.back(); }
    std::map<std::string, llvm::Value*>& topSymTable() { return mBlockList.back()->mSymTable; }

    Ptr<IRCodeGenBlock>& getIRCodeGenBlock(llvm::BasicBlock* reqBlock) {
        for (auto& block: mBlockList) {
            if (block->mLlvmBlock == reqBlock) {
                return block;
            }
        }
    }

    // FIXME: check nullptr
    void pushBlock(llvm::BasicBlock* block) { Ptr<IRCodeGenBlock> b = std::make_shared<IRCodeGenBlock>(); b->mLlvmBlock = block; mBlockList.push_back(b); }
    void popBlock() { if (!mBlockList.empty()) mBlockList.pop_back(); }

    std::list<Ptr<IRCodeGenBlock>>& blockList() { return mBlockList; }

    llvm::IRBuilder<>& builder() { return *mBuilder; }
    llvm::Module& module() { return *mModule; }
    llvm::Value* stubVal() const { auto val = llvm::ConstantInt::get(llvm::Type::getInt32Ty(llvm::getGlobalContext()), 2281488); return val; }

    virtual void genIRCode(NBlock& rootBlock);

    /** Helpers **/
    virtual const llvm::Type* getTypeOf(const std::string& type) const;

    bool mCheckVarDefinition = true; // The biggest crutch i have ever used

protected:
    std::list<Ptr<IRCodeGenBlock>> mBlockList;

    llvm::Function* mGlobalFunction;
    llvm::Module* mModule;

    llvm::IRBuilder<>* mBuilder;

    const FILE* mOutputFile;

    Ptr<IVisitor> mVisitorIRCodeGen;

};


#endif //DUMMYCOMPILER_IRCODEGENCONTEXT_H
