#pragma once

#include <string>

struct Token
{
	Token() { }
	Token(int _number, int _line, const std::string& _value)
		: number(_number), line(_line), value(_value) { }
	Token(const Token& Other)
		: number(Other.number), line(Other.line), value(Other.value) { }

	Token& operator=(const Token& Other)
	{
		number = Other.number;
		line = Other.line;
		value = Other.value;

		return *this;
	}

	bool operator==(const Token& token) const
	{
		return ((number == token.number) && (line == token.line)
			&& (value == token.value));
	}

	bool operator!=(const Token& token) const
	{
		return !(*this == token);
	}

	int number;
	int line;

	std::string value;
};

enum TokenType
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
	sym_void, sym_byte, sym_char, sym_short,
	sym_int, sym_long, sym_intptr, sym_ubyte,
	sym_uchar, sym_ushort, sym_uint, sym_ulong,
	sym_uintptr, sym_bool,

	sym_for, sym_while, sym_do,

	sym_if, sym_else, sym_switch, sym_break,
	sym_continue, sym_default,

	sym_internal, sym_private, sym_public, sym_protected,
	sym_abstract, sym_sealed, sym_static, sym_interface,
	sym_import, sym_class, sym_virtual, sym_return,

	sym_COUNT
};

#define IS_KEYWORD(t) ((sym_void <= t) && (sym_return >= t))
#define IS_TYPE(t) ((sym_void <= t) && (sym_bool >= t))
#define IS_ITERATOR(it) ((sym_for <= it) && (sym_do >= it))
#define IS_CONTROL(ct) ((sym_if <= ct) && (ct == sym_switch))
#define IS_SWITCH(sw) ((sym_switch <= sw) && (sym_default >= sw))
#define IS_ACESS_TYPE(at) ((sym_internal <= at) && (sym_protected >= at))
#define IS_CLASS_TYPE(ct) ((sym_abstract <= ct) && (sym_interface >= ct))

#define IS_CALC(c) ((tplus <= c) && (tdiv >= c))

const std::string keywords[] = {
	"void", "byte", "char", "short",
	"int", "long", "intptr", "ubyte",
	"uchar", "ushort", "uint", "ulong",
	"uintptr", "bool",

	"for", "while", "do",

	"if", "else", "switch", "break",
	"continue", "default",

	"internal", "private", "public", "protected",
	"abstarct", "sealed", "static", "interface",
	"import", "class", "virtual", "return",
};

const size_t NUM_KEYWORDS = sym_COUNT - sym_void;