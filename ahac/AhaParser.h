#pragma once

#include <vector>
#include "AhaToken.h"
#include "AhaType.h"

using namespace std;

class CAhaParser
{
public:
	CAhaParser(const string& src, vector<TokenType> _tokens);
	~CAhaParser();

	void Parse();

	void SaveToFile(const char* filename);

private:
	vector<TokenType> tokens;

	// result
	vector<wstring> refers;
	vector<wstring> datas;
	vector<sClass> classes;

	std::string src;

	friend class CAhaCompiler;
};