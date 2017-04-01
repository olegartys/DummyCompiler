#include <Log.h>
#include <CompilerContext.h>

#include <VisitorPrint.h>

#define LOG_TAG "main"

int main() {
    std::shared_ptr<CompilerContext> ctx;
    VisitorPrint visitorPrint;

    try {
        ctx = std::make_shared<CompilerContext>("test");
    } catch (std::runtime_error& e) {
        Log::error(LOG_TAG, "{}", e.what());
        exit(EXIT_FAILURE);
    }

    ctx->parse();
    ctx->setParserDebugLvl(0);

    auto rootASTblock = ctx->getRootNode();
    if (!rootASTblock) {
        Log::error(LOG_TAG, "Parser failed!");
        exit(EXIT_FAILURE);
    } else {
        for (auto it: rootASTblock->mStatements) {
            it->accept(visitorPrint);
        }
    }

	return 0;
}
