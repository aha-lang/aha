#pragma once

#include "AhaTokens.h"
#include "AhaCompiler.h"

#include <vector>

class scanner
{
public:
	const int ID_LENGTH = 256;

public:
	scanner(const source& src);

	void Scan();

	void SaveToFile(const std::string& filename);
	std::vector<Token>& Get();

private:
	source src;
	std::vector<Token> tokens;
};