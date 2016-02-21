#include "stdafx.h"

#include <aharun/Context.h>
#include <aharun/Module.h>
#include <aharun/ClassInfo.h>
#include <aharun/FunctionInfo.h>

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		fprintf(stderr, "Usage : %s [abf file]\n", argv[0]);
		return -1;
	}

	aha::Context::InitGC();
	aha::Context context;

	auto pMainModule = context.LoadModule(argv[1]);
	aha::FunctionInfo* pfnMain = nullptr;

	for (auto& cls : pMainModule->GetClassList())
	{
		for (auto& fn : cls->GetFunctionList())
		{
			if (fn->GetName() == u"main")
			{
				pfnMain = fn.get();
				goto break_2;
			}
		}
	}
break_2:

	if (pfnMain == nullptr)
	{
		fprintf(stderr, "cannot find 'main()' function\n");
		return -2;
	}

	pfnMain->Call(nullptr, nullptr);
}
