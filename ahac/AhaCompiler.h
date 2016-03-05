#pragma once

#include <vector>
#include <string>

#include "AhaType.h"

struct source
{
	source(const std::string filename);

	std::string file, code;
};

struct compile_context
{
	std::vector<std::string> refers;
	std::vector<Class> Classes;

	std::vector<std::string> Strings;

	void add_refer(const std::string& refer)
	{
		auto it = std::find(refers.begin(), refers.end(), refer);

		if (it == refers.end()) refers.push_back(refer);
	}
};

struct code_position {
	code_position(const std::string& fn, int line) : file(fn), line(line) { }
	code_position(const code_position& Other)
		: file(Other.file), line(Other.line) { }

	code_position& operator=(const code_position& Other)
	{
		file = Other.file;
		line = Other.line;
		return *this;
	}

	std::string file;
	int line;
};

struct compiler_error : public std::runtime_error
{
	compiler_error(const std::string& msg, const code_position& _pos)
		: std::runtime_error(msg), pos(_pos) { }

	code_position pos;
};

std::string compile(const std::vector<source>& src);