#include <Log.h>
#include <CompilerContext.h>

#include <VisitorPrint.h>

#undef LOG_TAG
#define LOG_TAG "main"

void printAST(VisitorPrint& vp, NBlock& rootASTblock) {
    for (auto it: rootASTblock.mStatements) {
        it->accept(vp);
    }
}

int main() {
    std::shared_ptr<CompilerContext> ctx;
    VisitorPrint visitorPrint;

    try {
        ctx = std::make_shared<CompilerContext>("test"/*, "test.out"*/);
    } catch (std::runtime_error& e) {
        Log::error(LOG_TAG, "{}", e.what());
        exit(EXIT_FAILURE);
    }

    ctx->setParserDebugLvl(0);
    ctx->parse();

    printAST(visitorPrint, *ctx->getRootNode());

    ctx->genIRLLVMCode();

    return 0;
}
