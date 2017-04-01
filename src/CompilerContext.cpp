//
// Created by olegartys on 30.03.17.
//

#include "CompilerContext.h"

#define LOG_TAG "CommpilerContext"

extern FILE* yyin; // nothing to do with this mess as we are using C-style scanner

CompilerContext::CompilerContext(const std::string &inputPath) :
        mInputPath(inputPath), mBisonParser(*this), mInputFile(nullptr) {
    if (mInputPath.empty()) {
        Log::info(LOG_TAG, "Using stdin as input");

        yyin = stdin;
        mInputFile = yyin;

    } else {
        mInputFile = fopen(inputPath.c_str(), "r");
        if (!mInputFile) {
            throw std::runtime_error("Can't open input file for read!");
        }

        Log::info(LOG_TAG, "Using {} as input", mInputPath);
        yyin = mInputFile;
    }
}

CompilerContext::~CompilerContext() {
    if (mInputFile)
        fclose(mInputFile);
}