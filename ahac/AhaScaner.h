#ifndef AhaScaner_h__
#define AhaScaner_h__

#include <stdio.h>
#include <xstring>
#include <vector>

#include "AhaToken.h"

const int ID_LENGTH = 256;

class CAhaScaner
{
private:
	std::vector<std::wstring> keywords;
	std::vector<eSymbol>     tnum;

private:
	std::wstring source;
	std::string src;

	std::vector<TokenType> tokens;

public:
	CAhaScaner(const std::string& src, std::wstring& sourcef);
	~CAhaScaner(void);

	void Scan();

	void SaveToFile(const char* filename);
	std::vector<TokenType>& get();
};

#endif // AhaScaner_h__