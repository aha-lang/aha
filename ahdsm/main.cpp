#include "stdafx.h"
#include "Utf16StrBuilder.h"

#include <ahabin/ReadFileStream.h>
#include <ahabin/AhaModule.h>
using ahabin::RESULT_FAIL;
using ahabin::RESULT_SUCS;

// opcode.cpp
std::wstring DisasOpcode(const ahabin::ArrayList<ahabin::aha_u8>& opcodes, const ahabin::AhaStrings& strings);

void OutputRefer(const ahabin::AhaRefer& refer, const ahabin::AhaStrings& strings);
void OutputNativeRefer(const ahabin::AhaNativeRefer& nrefer, const ahabin::AhaStrings& strings);
void OutputBody(const ahabin::AhaBody& body, const ahabin::AhaStrings& strings);

void OutputClass(const ahabin::AhaClass& cls, const ahabin::AhaStrings& strings);
void OutputClsMember(const ahabin::AhaClsMember& clsmem, const ahabin::AhaStrings& strings);

void OutputString(const ahabin::StringUTF16& str);

void OutputTypeStr(ahabin::AhaType type, const ahabin::AhaStrings& strings);

const char16_t* AccessStr(ahabin::AhaAccess access);
const char16_t* ClassTypeStr(ahabin::AhaClassType type);
const char16_t* MemberTypeStr(ahabin::AhaClsMemberType type);
const char16_t* MemberStorageStr(ahabin::AhaClsMemberStorage ckind);

void BinaryDump(const void* memory, unsigned size);

Utf16StrBuilder output_builder;

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
		fprintf(stderr, "fail to load module.\n");
		return -2;
	}

	OutputRefer(module.GetRefer(), module.GetStrings());
	OutputNativeRefer(module.GetNativeRefer(), module.GetStrings());
	OutputBody(module.GetBody(), module.GetStrings());
	output_builder.write(u"end\n");

	std::wstring result_str = output_builder.make();
	std::wcout << result_str;

	return 0;
}

void OutputRefer(const ahabin::AhaRefer& refer, const ahabin::AhaStrings& strings)
{
	output_builder.write(u"refer\n");

	auto& lst = refer.Get();

	for (ahabin::aha_i32 i = 0; (size_t)i < lst.GetLength(); ++i)
	{
		OutputString(strings.Get()[lst[i]]);
	}

	output_builder.write(u"endrefer\n\n");
}

void OutputNativeRefer(const ahabin::AhaNativeRefer& nrefer, const ahabin::AhaStrings& strings)
{
	output_builder.write(u"nrefer\n");

	auto& lst = nrefer.Get();

	for (ahabin::aha_i32 i = 0; (size_t)i < lst.GetLength(); ++i)
	{
		OutputString(strings.Get()[lst[i]]);
	}

	output_builder.write(u"endnrefer\n\n");
}

void OutputBody(const ahabin::AhaBody& body, const ahabin::AhaStrings& strings)
{
	output_builder.write(u"body\n\n");

	auto& lst = body.Get();

	for (ahabin::aha_i32 i = 0; (size_t)i < lst.GetLength(); ++i)
	{
		OutputClass(lst[i], strings);
	}

	output_builder.write(u"endbody\n\n");
}

void OutputClass(const ahabin::AhaClass& cls, const ahabin::AhaStrings& strings)
{
	output_builder.write_u16(u'\t');
	output_builder.write(AccessStr(cls.GetRaw().access));
	output_builder.write_u16(u' ');
	output_builder.write(ClassTypeStr(cls.GetRaw().type));
	output_builder.write(u" class ");
	output_builder.write(strings.Get()[cls.GetRaw().name]);

	if (cls.GetRaw().base != -1)
	{
		output_builder.write(u" \\\n\t\t\t extends ");
		output_builder.write(strings.Get()[cls.GetRaw().name]);
	}
	if (cls.GetRaw().interfaces != -1)
	{
		auto& strInterfaces = strings.Get()[cls.GetRaw().interfaces];
		ahabin::aha_i32 prev = 0;

		for (ahabin::aha_i32 i = 0; ; ++i)
		{
			if (strInterfaces[i] == u'\0')
			{
				if (prev == i)
					break;

				if (prev == 0)
					output_builder.write(u" \\\n\t\t\t implements ");
				else
					output_builder.write(u" \\\n\t\t\t\t, ");

				output_builder.write(&strInterfaces[prev], &strInterfaces[i + 1]);

				prev = i + 1;
			}
		}
	}

	output_builder.write(u"\n\n");

	auto& lst =  cls.GetMembers();

	for (ahabin::aha_i32 i = 0; (size_t)i < lst.GetLength(); ++i)
	{
		OutputClsMember(lst[i], strings);
	}

	output_builder.write(u"endclass\n\n");
}

void OutputClsMember(const ahabin::AhaClsMember& clsmem, const ahabin::AhaStrings& strings)
{
	output_builder.write(u"\t\t");
	output_builder.write(AccessStr(clsmem.GetRaw().access));
	output_builder.write_u16(u' ');
	output_builder.write(MemberTypeStr(clsmem.GetRaw().type));
	output_builder.write_u16(u' ');
	output_builder.write(MemberStorageStr(clsmem.GetRaw().storage));
	output_builder.write_u16(u' ');
	OutputTypeStr(clsmem.GetRaw().function.rettype, strings);
	//OutputTypeStr(clsmem.GetRaw().variable.type, strings);
	output_builder.write_u16(u' ');
	output_builder.write(strings.Get()[clsmem.GetRaw().name]);
	output_builder.write_u16(u'\n');

	if (clsmem.GetRaw().type == ahabin::AHA_CLSMEM_TYPE_FUNC)
	{
		output_builder.write(u"\t\t\t( ");

		auto& params = clsmem.GetParams();
		ahabin::aha_i32 i;

		for (i = 0; (size_t)i < params.GetLength(); ++i)
		{
			if (i != 0)
				output_builder.write(u", ");

			OutputTypeStr(params[i], strings);
		}

		if (i != 0)
			output_builder.write_u16(u' ');
		output_builder.write(u")\n\t\t{\n");

		// TODOn
		output_builder.write(u"\t\t\t");
		BinaryDump(clsmem.GetOpcode().data(), clsmem.GetOpcode().GetLength());

		output_builder.write(u"\t\t}\n");
	}
	else
	{
		output_builder.write(u"\t\t\t( ");

		// TODO
		output_builder.write(u"**not-implemented**");

		output_builder.write(u")\n");
	}

	output_builder.write_u16(u'\n');
}

void OutputString(const ahabin::StringUTF16& str)
{
	output_builder.write_u16(u'\"');

	for (ahabin::aha_i32 j = 0; j < str.GetLength(); ++j)
	{
		if (str[j] == '\"')
		{
			output_builder.write(u"\\\"");
		}
		if (str[j] == '\'')
		{
			output_builder.write(u"\\\'");
		}
		else if (str[j] == '\\')
		{
			output_builder.write(u"\\\\");
		}
		else
		{
			output_builder.write_u16(str[j]);
		}
	}

	output_builder.write_u16(u'\"');
}

void OutputTypeStr(ahabin::AhaType type, const ahabin::AhaStrings& strings)
{
	static const char16_t* str[] = {
		u"void",
		u"bool",
		u"int8",
		u"uint8",
		u"int16",
		u"uint16",
		u"int32",
		u"uint32",
		u"int64",
		u"uint64",
		u"float32",
		u"float64",
		u"intptr",
		u"uintptr",
	};

	if (type & 0x80000000)
	{
		output_builder.write(str[type & 0x7fffffff]);
	}
	else
	{
		output_builder.write(strings.Get()[type]);
	}
}

const char16_t* AccessStr(ahabin::AhaAccess access)
{
	static const char16_t* str[] = {
		u"public",
		u"protected",
		u"protected_internal",
		u"internal",
		u"private"
	};
	return str[access];
}

const char16_t* ClassTypeStr(ahabin::AhaClassType type)
{
	static const char16_t* str[] = {
		u"normal",
		u"abstract",
		u"sealed",
		u"static",
		u"interface"
	};
	return str[type];
}

const char16_t* MemberTypeStr(ahabin::AhaClsMemberType type)
{
	static const char16_t* str[] = {
		u"func",
		u"var",
	};
	return str[type];
}

const char16_t* MemberStorageStr(ahabin::AhaClsMemberStorage ckind)
{
	static const char16_t* str[] = {
		u"classof",
		u"instof",
	};
	return str[ckind];
}

void BinaryDump(const void* memory, unsigned size)
{
	const uint8_t* mem = (const uint8_t*)memory;
	const uint8_t* p;

	for (p = mem; p < mem + size; p++)
	{
		if (p != mem)
			putchar(' ');
		printf("%02x", *p);
	}

	putchar('\n');
}
