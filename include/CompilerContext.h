//
// Created by olegartys on 30.03.17.
//

#ifndef DUMMYCOMPILER_COMPILERCONTEXT_H
#define DUMMYCOMPILER_COMPILERCONTEXT_H

// Some C-typed flex compatibility mess
#ifndef YY_NULLPTR
    #define YY_NULLPTR nullptr
#endif

#include "../src/flex_bison_output/BisonParser.hpp"

#define YY_DECL \
    yy::BisonParser::symbol_type yylex (CompilerContext& d)
// ... and declare it for the parser's sake.
YY_DECL;

class CompilerContext {
public:
    CompilerContext(const std::string& inputPath = "");

    CompilerContext(const CompilerContext&) = delete;
    CompilerContext& operator= (const CompilerContext&) = delete;

    virtual ~CompilerContext();

    virtual void setParserDebugLvl(int lvl) {
        mBisonParser.set_debug_level(lvl);
    }

    virtual void setRootNode(const std::shared_ptr<NBlock>& rootBlock) {
        this->mRootASTBlock = rootBlock;
    }

    virtual std::shared_ptr<NBlock> getRootNode() const {
        return mRootASTBlock;
    }

    virtual void parse() {
        if (!mInputFile) {
            throw std::runtime_error("Input file handler is null!");
        }
        mBisonParser.parse();
    }


protected:
    yy::BisonParser mBisonParser;

    std::shared_ptr<NBlock> mRootASTBlock;
    std::string mInputPath;
    FILE* mInputFile;

};


#endif //DUMMYCOMPILER_COMPILERCONTEXT_H
