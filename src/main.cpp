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

void usage(const char* progName) {
    Log::error(LOG_TAG, "Usage: {} [inputFile] [outputFile]", progName);
}

int main(int argc, char** argv) {
    std::shared_ptr<CompilerContext> ctx;
    VisitorPrint visitorPrint;

    if (argc != 3) {
        usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    try {
        ctx = std::make_shared<CompilerContext>(argv[1], argv[2]);
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
