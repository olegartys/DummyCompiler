#ifndef ILEXER_H
#define ILEXER_H

#include <list>

#include <LangTokenHolder.h>

class ILexer {
public:
	virtual ~ILexer() = default; 

	virtual LangTokenHolder::Token getNextToken() = 0;
	virtual std::list<LangTokenHolder::Token> tokenize() = 0;

};

#endif // ILEXER_H