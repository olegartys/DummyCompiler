//
// Created by olegartys on 30.03.17.
//

#include "CompilerContext.h"

#undef LOG_TAG
#define LOG_TAG "CommpilerContext"

extern FILE* yyin; // nothing to do with this mess as we are using C-style scanner

CompilerContext::CompilerContext(const std::string &inputPath, const std::string& outputPath) :
        mInputPath(inputPath), mOutputPath(outputPath), mBisonParser(*this),
        mInputFile(nullptr), mOutputFile(nullptr) {
    // Deal with input file
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

    // Deal with output file
    Log::info("HERE", "");
    if (mOutputPath.empty()) {
        mOutputFile = stdout;
    } else {
        mOutputFile = fopen(outputPath.c_str(), "w");
        if (!mOutputFile) {
            throw std::runtime_error("Can't open output file for write!");
        }

        Log::info(LOG_TAG, "Using {} as output", mOutputPath);
    }

    mCodeGenCtx = std::make_shared<IRCodeGenContext>(mOutputFile);
}

CompilerContext::~CompilerContext() {
    if (mInputFile)
        fclose(mInputFile);

    if (mOutputFile)
        fclose(mOutputFile);
}