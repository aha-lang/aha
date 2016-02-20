#include "stdafx.h"

#include <aharun/Context.h>

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
}
