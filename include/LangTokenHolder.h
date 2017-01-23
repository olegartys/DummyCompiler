#ifndef LANGTOKENHOLDER_H
#define LANGTOKENHOLDER_H

#include <string>
#include <map>

#include <LangTokenSet.h>

class LangTokenHolder {
public:

	struct Token final {
		Token() : sTok(""), enumTok(LangTokenSet::TOK_UNKNOWN) { }

		bool isValid() const { return enumTok != LangTokenSet::TOK_UNKNOWN && !sTok.empty(); }

		operator std::string() const { return sTok; } // FIXME: convert enumclass to the string

		std::string sTok;
		LangTokenSet enumTok;
	};

	LangTokenHolder() = delete; // Should be used as static class only

	static Token get(const std::string&);
	static Token get(const LangTokenSet&);

	static bool in(const std::string&);
	static bool in(const LangTokenSet&);

protected:
	static std::map<LangTokenSet, std::string> mTokenMap;

};

#endif // LANGTOKENHOLDER_H