#include "AhaCompiler.h"

#include "AhaScanner.h"
#include "AhaParser.hpp"

#include <stdio.h>

source::source(const std::string filename)
{
	FILE* fp = 0;
	fopen_s(&fp, filename.c_str(), "rt");

	if (!fp)
	{
		throw compiler_error("파일을 불러올 수 없습니다.", code_position(filename, -1));
	}

	fseek(fp, 0, SEEK_END);
	size_t len = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	code.resize(len);
	fread(&code[0], len, 1, fp);

	fclose(fp);
}

std::string compile(const std::vector<source>& src)
{
	compile_context ctx;

	for (auto& it : src)
	{
		scanner scan(it);
		scan.Scan();

		parser parse(ctx, it, scan.Get());
		parse.Parse();
		parse.SaveToFile("test.txt");
	}



	return "";
}