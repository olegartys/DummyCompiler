#ifndef LANGTOKENSET_H
#define LANGTOKENSET_H

enum class LangTokenSet {
	// Type tokens
	TINT, TFLOAT, TDOUBLE, TCHAR,

	// Parenthis tokens
	TLPAR, TRPAR, TLBRACE, TRBRACE,

	TEQUAL, TCEQ, TCNEQ,

	TRETURN,

	TOK_UNKNOWN
};

#endif // LANGTOKENSET_H