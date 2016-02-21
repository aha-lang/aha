#pragma once

#include <vector>
#include <string>
#include <fstream>

#include "AhaParser.h"

using namespace std;

class CAhaCompiler
{
public:
	CAhaCompiler(CAhaParser _parser);
	~CAhaCompiler();

	void MakeAHSM(const char* filename);

private:
	void _writeAttribute();
	void _writeRefer(vector<wstring>& refers);
	void _writeNativeRefer();
	void _writeData(vector<wstring>& datas);
	void _writeTypeinfo(vector<sClass>& classes);

private:
	wofstream file;

	CAhaParser parser;
};