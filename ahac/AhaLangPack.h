#pragma once

#include <iostream>
#include <map>
#include <string>
#include <xstring>

typedef std::map<std::string, std::wstring> AhaLangMap;

class AhaLangPack
{
public:
	bool Load(const std::string& filename);

	std::wstring GetValue(const std::string& key);

private:
	AhaLangMap strs;
};