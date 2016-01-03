#include "stdafx.h"

#include <ahabin/ReadFileStream.h>
#include <ahabin/AhaModule.h>
using ahabin::RESULT_FAIL;
using ahabin::RESULT_SUCS;

void OutputStrings(ahabin::AhaStrings& strings);
void OutputRefer(ahabin::AhaRefer& refer);
void OutputNativeRefer(ahabin::AhaNativeRefer& refer);
void OutputBody(ahabin::AhaBody& body);

void OutputStrArray(ahabin::ArrayList<ahabin::StringUTF16>& ar);
void OutputClass(ahabin::AhaClass& cls);
void OutputClsMember(ahabin::AhaClsMember& clsmem);

const char* AccessStr(ahabin::AhaAccess access);
const char* ClassTypeStr(ahabin::AhaClassType type);
const char* MemberTypeStr(ahabin::AhaClsMemberType type);
const char* MemberStorageStr(ahabin::AhaClsMemberStorage ckind);
const char* TypeStr(ahabin::AhaType type);

void BinaryDump(const char* memory, unsigned size);

int main(int argc, char *argv[])
{
#if defined(_MSC_VER) && defined(_DEBUG)
	BOOST_SCOPE_EXIT(void)
	{
		_CrtDumpMemoryLeaks();
	}
	BOOST_SCOPE_EXIT_END;
#endif

	if (argc != 2)
	{
		fprintf(stderr, "Usage : %s [abf file]", argv[0]);
		return -1;
	}

	ahabin::ReadFileStream strm;
	ahabin::AhaModule module;

	if (RESULT_FAIL(strm.Create(argv[1])))
	{
		fprintf(stderr, "fail to read file '%s'\n", argv[1]);
		return -2;
	}

	if (RESULT_FAIL(module.Read(strm)))
	{
		fprintf(stderr, "fail to load module.\n", argv[1]);
		return -2;
	}

	OutputStrings(module.GetStrings());
	OutputRefer(module.GetRefer());
	OutputNativeRefer(module.GetNativeRefer());
	OutputBody(module.GetBody());

	return 0;
}

void OutputStrings(ahabin::AhaStrings& strings)
{
	printf("strings\n");

	for (ahabin::aha_i32 i = 0; i < strings.Get().GetLength(); ++i)
	{
		putchar('\"');

		const ahabin::StringUTF16& str = strings.Get()[i];

		for (ahabin::aha_i32 j = 0; j < str.GetLength(); ++j)
		{
			if (str[j] == '\"')
			{
				printf("\\\"");
			}
			if (str[j] == '\'')
			{
				printf("\\\'");
			}
			else if (str[j] == '\\')
			{
				printf("\\\\");
			}
			else if (isprint(str[j]))
			{
				putchar(str[j]);
			}
			else
			{
				;//¤·¤¤
			}
		}

		putchar('\"');
	}
}

void OutputRefer(ahabin::AhaRefer& refer)
{

}

void OutputNativeRefer(ahabin::AhaNativeRefer& refer)
{

}

void OutputBody(ahabin::AhaBody& body)
{

}

void OutputStrArray(ahabin::ArrayList<ahabin::StringUTF16>& ar)
{

}

void OutputClass(ahabin::AhaClass& cls)
{

}

void OutputClsMember(ahabin::AhaClsMember& clsmem)
{

}

const char* AccessStr(ahabin::AhaAccess access)
{
	static const char* str[] = {
		"public",
		"protected",
		"protected_internal",
		"internal",
		"private"
	};
	return str[access];
}

const char* ClassTypeStr(ahabin::AhaClassType type)
{
	static const char* str[] = {
		"normal",
		"abstract",
		"sealed",
		"static",
		"interface"
	};
	return str[type];
}

const char* MemberTypeStr(ahabin::AhaClsMemberType type)
{
	static const char* str[] = {
		"func",
		"var",
	};
	return str[type];
}

const char* MemberStorageStr(ahabin::AhaClsMemberStorage ckind)
{
	static const char* str[] = {
		"classof",
		"instof",
	};
	return str[ckind];
}

const char* TypeStr(ahabin::AhaType type)
{
	static const char* str[] = {
		"void",
		"bool",
		"byte",
		"int8",
		"int16",
		"int32",
		"int64",
		"float32",
		"float64",
	};
	return str[type];
}

void BinaryDump(const char* memory, unsigned size)
{
	const char* p;

	for (p = memory; p < memory + size; p++)
	{
		if (p != memory)
			putchar(' ');
		printf("%02x", *((unsigned char *)p));
	}

	putchar('\n');
}
