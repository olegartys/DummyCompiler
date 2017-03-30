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

};


#endif //DUMMYCOMPILER_COMPILERCONTEXT_H
