#include "AhaLangPack.h"

#include <stdio.h>

const char Mark[4] = { 'A', 'H', 'A', 'L' };

bool AhaLangPack::Load(const std::string& filename)
{
	FILE* fp = fopen(filename.c_str(), "rb");
	
	if (!fp) return false;

	char v[4];
	fread(&v, 4, 1, fp);

	if (memcmp(v, Mark, 4) != 0)
	{
		fclose(fp);
		return false;
	}

	int Count = 0;
	fread(&Count, 4, 1, fp);

	char a;
	fread(&a, 1, 1, fp);

	for (int i = 0; i < Count; i++)
	{
		int len1, len2;
		fread(&len1, 4, 1, fp);
		fread(&len2, 4, 1, fp);

		std::string key; key.resize(len1);
		std::wstring val; val.resize(len2);

		fread(&key[0], len1, 1, fp);
		fread(&val[0], len2, 1, fp);

		strs[key] = val;
	}

	fclose(fp);
	return true;
}

std::wstring AhaLangPack::GetValue(const std::string& key)
{
	auto it = strs.find(key);
	if (it != strs.end()) return it->second;
	else return L"Error";
}