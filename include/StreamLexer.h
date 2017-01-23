#ifndef STREAMLEXER_H
#define STREAMLEXER_H

#include <istream>
#include <memory>

#include <ILexer.h>

class StreamLexer : public ILexer {
public:
	StreamLexer(std::unique_ptr<std::istream>&& inputStream) : 
			mInputStream(std::move(inputStream)), mCurChar(' ') { }

	LangTokenHolder::Token getNextToken() override;

	std::list<LangTokenHolder::Token> tokenize() { throw std::runtime_error("Unsupported method"); }

protected:
	std::unique_ptr<std::istream> mInputStream;

private:
	char mCurChar;

};

#endif // STREAMLEXER_H