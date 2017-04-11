//
// Created by oleglevin on 10.04.17.
//

#ifndef DUMMYCOMPILER_STRUCTTYPEWRAPPER_H
#define DUMMYCOMPILER_STRUCTTYPEWRAPPER_H

#include <llvm/IR/DerivedTypes.h>
#include "AST.h"

struct StructTypeWrapper {

    StructTypeWrapper(const std::string& name, llvm::StructType* structType) :
        mStructTypeName(name), mLlvmStructType(structType) {

    }

    void addField(const std::string& name, const std::string& accessSpecifier, llvm::Value* val) {
        mFieldsList[name] = std::make_pair(accessSpecifier, val);
        if (name == mStructTypeName && val->getType()->getTypeID() == llvm::Type::FunctionTyID) {
            constructorFunction = val;
        }
    }

    std::string mStructTypeName;

    llvm::Value* constructorFunction;
    std::map<std::string, std::pair<std::string, llvm::Value*>> mFieldsList;

    std::vector<NStructField> mPrivateFieldsList;
    std::vector<NStructField> mPublicFieldsList;

    llvm::StructType* mLlvmStructType;

};

#endif //DUMMYCOMPILER_STRUCTTYPEWRAPPER_H
