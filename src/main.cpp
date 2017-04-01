#include <Log.h>
#include <CompilerContext.h>
#include "flex_bison_output/BisonParser.hpp"

#include <AST.h>

std::shared_ptr<NBlock> programBlock;

int main() {
    try {
        CompilerContext ctx;
        ctx.parse();
    } catch (std::runtime_error& e) {
        std::cerr << e.what();
    }
//	yy::BisonParser parser(ctx);
//    // parser.set_debug_level(4);
//    parser.parse();
	return 42;
}
