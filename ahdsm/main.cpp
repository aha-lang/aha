#include "stdafx.h"
#include "StrBuilder.h"

#include <ahabin/AhaModule.h>
#include <ahabin/exceptions.h>

// opcode.cpp
std::wstring DisasOpcode(const std::vector<aha::aha_u8>& opcodes, const aha::AhaStrings& strings, const wchar_t* prefix);

void OutputStrings(const aha::AhaStrings& strings);
void OutputRefer(const aha::AhaRefer& refer, const aha::AhaStrings& strings);
void OutputNativeRefer(const aha::AhaNativeRefer& nrefer, const aha::AhaStrings& strings);
void OutputBody(const aha::AhaBody& body, const aha::AhaStrings& strings);

void OutputClass(const aha::AhaClass& cls, const aha::AhaStrings& strings);
void OutputClsMember(const aha::AhaClsMember& clsmem, const aha::AhaStrings& strings);

void OutputString(const std::u16string& str);

void OutputTypeStr(aha::AhaType type, const aha::AhaStrings& strings);

std::wstring InitialValStr(aha::AhaType type, aha::AhaVariable initial);

const char16_t* AccessStr(aha::AhaAccess access);
const char16_t* ClassTypeStr(aha::AhaClassType type);
const char16_t* MemberTypeStr(aha::AhaClsMemberType type);
const char16_t* MemberStorageStr(aha::AhaClsMemberStorage ckind);

StrBuilder output_builder;

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		fprintf(stderr, "Usage : %s [abf file]", argv[0]);
		return -1;
	}

	std::ifstream strm(argv[1], std::ios::in | std::ios::binary);
	aha::AhaModule module;

	try
	{
		module.Read(strm);
	}
	catch (aha::BadModuleError& e)
	{
		fprintf(stderr, "%s", e.what());
		return -2;
	}
	catch (std::ios::failure& e)
	{
		fprintf(stderr, "I/O Error : %s", e.what());
		return -2;
	}

	output_builder.write(u".module ");
	output_builder.write(module.GetStrings().Get()[module.GetModuleName()]);
	output_builder.write(u"\n\n");

	OutputStrings(module.GetStrings());
	OutputRefer(module.GetRefer(), module.GetStrings());
	OutputNativeRefer(module.GetNativeRefer(), module.GetStrings());
	OutputBody(module.GetBody(), module.GetStrings());

	output_builder.write(u"end\n");

	std::wstring result_str = output_builder.make();
	std::wcout << result_str;

	getchar();

	return 0;
}

void OutputStrings(const aha::AhaStrings& strings)
{
	output_builder.write(u"strings\n");

	for (const std::u16string& str : strings.Get())
	{
		OutputString(str);
		output_builder.write(u"\n");
	}

	output_builder.write(u"endstrings\n\n");
}

void OutputRefer(const aha::AhaRefer& refer, const aha::AhaStrings& strings)
{
	output_builder.write(u"refer\n");

	for (aha::aha_i32 idx : refer.Get())
	{
		output_builder.write(std::to_wstring(idx));
		output_builder.write(u" # ");
		OutputString(strings.Get()[idx]);
		output_builder.write(u"\n");
	}

	output_builder.write(u"endrefer\n\n");
}

void OutputNativeRefer(const aha::AhaNativeRefer& nrefer, const aha::AhaStrings& strings)
{
	output_builder.write(u"nrefer\n");

	for (aha::aha_i32 idx : nrefer.Get())
	{
		output_builder.write(std::to_wstring(idx));
		output_builder.write(u" # ");
		OutputString(strings.Get()[idx]);
		output_builder.write(u"\n");
	}

	output_builder.write(u"endnrefer\n\n");
}

void OutputBody(const aha::AhaBody& body, const aha::AhaStrings& strings)
{
	output_builder.write(u"body\n\n");

	for (const aha::AhaClass& cls : body.Get())
	{
		OutputClass(cls, strings);
		output_builder.write(u"\n");
	}

	output_builder.write(u"endbody\n\n");
}

void OutputClass(const aha::AhaClass& cls, const aha::AhaStrings& strings)
{
	output_builder.write(u"\t");
	output_builder.write(AccessStr(cls.GetRaw().access));
	output_builder.write(u" ");
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
		aha::aha_i32 prev = 0;

		for (aha::aha_i32 i = 0; ; ++i)
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

	for (const aha::AhaClsMember& clsmem : cls.GetMembers())
	{
		OutputClsMember(clsmem, strings);
	}

	output_builder.write(u"\tendclass\n");
}

void OutputClsMember(const aha::AhaClsMember& clsmem, const aha::AhaStrings& strings)
{
	output_builder.write(u"\t\t");
	output_builder.write(AccessStr(clsmem.GetRaw().access));
	output_builder.write(u" ");
	output_builder.write(MemberTypeStr(clsmem.GetRaw().type));
	output_builder.write(u" ");
	output_builder.write(MemberStorageStr(clsmem.GetRaw().storage));
	output_builder.write(u" ");
	OutputTypeStr(clsmem.GetRaw().function.rettype, strings);
	//OutputTypeStr(clsmem.GetRaw().variable.vartype, strings);
	output_builder.write(u" ");
	output_builder.write(strings.Get()[clsmem.GetRaw().name]);
	output_builder.write(u"\n");

	if (clsmem.GetRaw().type == aha::AHA_CLSMEM_TYPE_FUNC)
	{
		size_t i;

		output_builder.write(u"\t\t\t( ");

		for (i = 0; i < clsmem.GetParams().size(); ++i)
		{
			if (i != 0)
				output_builder.write(u", ");

			OutputTypeStr(clsmem.GetParams()[i], strings);
		}
		if (i != 0)
			output_builder.write(u" ");

		output_builder.write(u")\n\t\t{\n");

		if (!clsmem.GetLocals().empty())
		{
			output_builder.write(u"\t\t\t.locals (\n");
			for (i = 0; i < clsmem.GetLocals().size(); ++i)
			{
				if (i != 0)
					output_builder.write(u",\n");

				output_builder.write(u"\t\t\t\t");
				OutputTypeStr(clsmem.GetLocals()[i], strings);
			}
			if (i != 0)
				output_builder.write(u"\n");
			output_builder.write(u"\t\t\t\t)\n");
		}

		output_builder.write(DisasOpcode(clsmem.GetOpcode(), strings, L"\t\t\t"));

		output_builder.write(u"\t\t}\n");
	}
	else
	{
		output_builder.write(u"\t\t\t( ");
		output_builder.write(InitialValStr(clsmem.GetRaw().variable.vartype, clsmem.GetRaw().variable.initial));
		output_builder.write(u" )\n");
	}

	output_builder.write(u"\n");
}

void OutputString(const std::u16string& str)
{
	output_builder.write(u"\"");

	for (size_t j = 0; j < str.size(); ++j)
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
			char16_t s[2] = { str[j], u'\0' };
			output_builder.write(s);
		}
	}

	output_builder.write(u"\"");
}

void OutputTypeStr(aha::AhaType type, const aha::AhaStrings& strings)
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

	if (type & aha::AHA_TYPE_FLG_PRIMITIVE)
	{
		output_builder.write(str[type & aha::AHA_TYPE_MASK_NONFLG]);
	}
	else
	{
		output_builder.write(strings.Get()[type]);
	}
}

std::wstring InitialValStr(aha::AhaType type, aha::AhaVariable initial)
{
	std::wstring ret;

	if (type & aha::AHA_TYPE_FLG_PRIMITIVE)
	{
		switch (type & aha::AHA_TYPE_MASK_NONFLG)
		{
			case aha::AHA_TYPE_BOOL:
				ret = initial.v_bool ? L"true" : L"false";
				break;
			case aha::AHA_TYPE_INT8:
				ret = std::to_wstring(initial.v_int8);
				break;
			case aha::AHA_TYPE_UINT8:
				ret = std::to_wstring(initial.v_uint8);
				break;
			case aha::AHA_TYPE_INT16:
				ret = std::to_wstring(initial.v_int16);
				break;
			case aha::AHA_TYPE_UINT16:
				ret = std::to_wstring(initial.v_uint16);
				break;
			case aha::AHA_TYPE_INT32:
				ret = std::to_wstring(initial.v_int32);
				break;
			case aha::AHA_TYPE_UINT32:
				ret = std::to_wstring(initial.v_uint32);
				break;
			case aha::AHA_TYPE_INT64:
				ret = std::to_wstring(initial.v_int64);
				break;
			case aha::AHA_TYPE_UINT64:
				ret = std::to_wstring(initial.v_uint64);
				break;
			case aha::AHA_TYPE_FLOAT32:
				ret = std::to_wstring(initial.v_float32);
				break;
			case aha::AHA_TYPE_FLOAT64:
				ret = std::to_wstring(initial.v_float64);
				break;
			case aha::AHA_TYPE_INTPTR:
				ret = std::to_wstring(initial.v_intptr);
				break;
			case aha::AHA_TYPE_UINTPTR:
				ret = std::to_wstring(initial.v_uintptr);
				break;
			case aha::AHA_TYPE_CHAR:
				// TODO
				ret = std::wstring((wchar_t)initial.v_char, 1);
				break;
		}

		if (type & aha::AHA_TYPE_FLG_POINTER)
			ret += L'*';

		return ret;
	}
	else
	{
		return L"null";
	}

	abort();
}

const char16_t* AccessStr(aha::AhaAccess access)
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

const char16_t* ClassTypeStr(aha::AhaClassType type)
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

const char16_t* MemberTypeStr(aha::AhaClsMemberType type)
{
	static const char16_t* str[] = {
		u"func",
		u"var",
	};
	return str[type];
}

const char16_t* MemberStorageStr(aha::AhaClsMemberStorage ckind)
{
	static const char16_t* str[] = {
		u"classof",
		u"instof",
	};
	return str[ckind];
}
