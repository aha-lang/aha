#ifndef AhaToken_h__
#define AhaToken_h__

#include <string>

struct TokenType
{
	int number;
	unsigned long line;

	std::wstring value;

	TokenType() { }
	TokenType(int _number, unsigned long _line, const std::wstring& _value)
		: number(_number), line(_line), value(_value)
	{
	}

	bool operator==(const TokenType& token) const
	{
		return ((number == token.number) && (line == token.line)
			&& (value == token.value));
	}

	bool operator!=(const TokenType& token) const
	{
		return !(*this == token);
	}
};

const int NUM_KEYWORD = 23;
enum eSymbol
{
	tnull = -1,
	tident,
	tint,
	tfloat,
	tstring,

	// 사칙 연산
	tplus,
	tminus,
	tmul,
	tdiv,
	tequal,

	// 문법 기호
	tmlparan,
	tmrparan,

	tddot,
	tddot2,
	tnot,
	tcomma,
	tsemicolon,
	tlparan,
	trparan,

	tblparan,
	tbrparan,

	// 예약어
	sym_integer, sym_short, sym_long, sym_float, sym_string, sym_boolean,
	sym_void, sym_for, sym_while, sym_do,
	sym_switch, sym_case, sym_default,
	sym_if, sym_else, sym_break,
	sym_continue, sym_sizeof, sym_const,
	sym_public, sym_private, sym_protected, sym_internal,
	sym_abstract, sym_sealed, sym_static, sym_interface,
	sym_import, sym_true, sym_false, 
	sym_class, sym_virtual, sym_operator, sym_return
};

inline bool is_keyword(int a)
{
	return ((a >= sym_integer) && (a <= sym_return));
}

inline bool is_type(int a)
{
	return ((a >= sym_integer) && (a <= sym_void));
}

inline bool is_iterator(int a)
{
	return ((a >= sym_for) && (a <= sym_do));
}

inline bool is_control(int a)
{
	return ((a == sym_switch) || ((a >= sym_if) && (a <= sym_else)));
}

inline bool is_switch(int a)
{
	return ((a >= sym_case) && a <= sym_default);
}

inline bool is_access(int a)
{
	return ((a >= sym_public) && (a <= sym_internal));
}

inline bool is_class_type(int a)
{
	return ((a >= sym_abstract) && (a <= sym_interface));
}

#endif // AhaToken_h__