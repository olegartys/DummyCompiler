#include <StreamLexer.h>
#include <Log.h>

#define TAG "LangTokenHolder"

LangTokenHolder::Token StreamLexer::getNextToken() {

	throw std::runtime_error("Not implemented!");

	LangTokenHolder::Token retTok;

	// Skip whitespaces
	while (isspace(mCurChar)) {
		mInputStream->get(mCurChar);
	}

	// identifier: [a-zA-Z][a-zA-Z0-9]*
	std::string sIdentifier;
	sIdentifier = mCurChar;
	if (isalpha(mCurChar)) {
		mInputStream->get(mCurChar); 
		while (isalnum(mCurChar) && !mInputStream->eof()) {
			sIdentifier += mCurChar;
			mInputStream->get(mCurChar); 
		}

		Log::debug(TAG, sIdentifier);

		return LangTokenHolder::get(sIdentifier);
	}

	return retTok;
}
