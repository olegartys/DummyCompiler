#include <LangTokenHolder.h>

std::map<LangTokenSet, std::string> LangTokenHolder::mTokenMap = {
	{ LangTokenSet::TINT, "int" },
	{ LangTokenSet::TFLOAT, "float" },
	{ LangTokenSet::TDOUBLE, "double" },
	{ LangTokenSet::TCHAR, "char" }
};

LangTokenHolder::Token LangTokenHolder::get(const std::string& str) {
	LangTokenHolder::Token retTok;

	for (const auto& it: mTokenMap) {
		if (it.second == str) {
			retTok.sTok = it.second;
			retTok.enumTok = it.first;
			break;
		}
	}

	return retTok;
}

LangTokenHolder::Token LangTokenHolder::get(const LangTokenSet& enumTok) {
	LangTokenHolder::Token retTok;

	try {
		retTok.enumTok = enumTok;
		retTok.sTok = mTokenMap.at(enumTok);
	} catch (std::out_of_range& ex) {
		retTok.sTok = "";
		retTok.enumTok = LangTokenSet::TOK_UNKNOWN;
	}

	return retTok;
}

bool LangTokenHolder::in(const std::string& str) {
	return LangTokenHolder::get(str).isValid();
}

bool LangTokenHolder::in(const LangTokenSet& enumTok) {
	return LangTokenHolder::get(enumTok).isValid();
}