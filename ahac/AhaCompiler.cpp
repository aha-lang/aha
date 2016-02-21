#include "AhaCompiler.h"

#include <locale>
#include <codecvt>


CAhaCompiler::CAhaCompiler(CAhaParser _parser) : parser(_parser)
{
}

CAhaCompiler::~CAhaCompiler()
{
}

void CAhaCompiler::MakeAHSM(const char* filename)
{
	file.open(filename);
	file.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>()));

	if (file.is_open())
	{
		_writeAttribute();
		file << endl;
		_writeRefer(parser.refers);
		file << endl;
		_writeNativeRefer();
		file << endl;
		_writeData(parser.datas);
		file << endl;
		_writeTypeinfo(parser.classes);
	}

	file.close();
}

void CAhaCompiler::_writeAttribute()
{
	file << L"attribute" << endl;
	file << L".module test" << endl;
	file << L"endattr" << endl;
}

void CAhaCompiler::_writeRefer(vector<wstring>& refers)
{
	file << L"refer" << endl;

	for (const wstring& refer : refers)
		file << "\"" << refer << "\"" << endl;

	file << L"endrefer" << endl;
}

void CAhaCompiler::_writeNativeRefer()
{
	file << L"nativerefer" << endl;
	file << L"endnativerefer" << endl;
}

void CAhaCompiler::_writeData(vector<wstring>& datas)
{
	file << L"data" << endl;

	for (const wstring& data : datas)
		file << "\"" << data << "\"" << endl;

	file << L"enddata" << endl;
}

void CAhaCompiler::_writeTypeinfo(vector<sClass>& classes)
{
	file << L"typeinfo" << endl;

	for (const sClass& cls : classes)
	{
		file << AccessTypeStr[cls.at] << L" "
			<< ClassTypeStr[cls.ct] << L" " 
			<< L"class" << L" "
			<< cls.name << endl;

		for (const sVar& var : cls.vars)
		{
			file << L"\t" << AccessTypeStr[var.at] << L" "
				<< L"var classof " << var.tn << L" "
				<< var.name << L" { " << var.init << L" }"
				<< endl;
		}

		file << endl;

		for (const sFunction& func : cls.funcs)
		{
			file << L"\t" << AccessTypeStr[func.at] << L" "
				<< L"func classof " << func.tn << L" "
				<< func.name << L"( ";

			for (const sVar& parm : func.params)
			{
				file << parm.tn << L" "
					<< parm.name;

				if (parm.init.size() > 0)
				{
					file << " = " << parm.init;
				}

				if (parm != *(func.params.end() - 1))
					file << L", ";
			}

			file << L" )" << endl
				<< "\t" << L"{" << endl;



			file << "\t" << L"}" << endl;
		}

		file << L"endclass" << endl;
	}

	file << L"endtype" << endl;
}