#include <stdio.h>

__declspec(dllexport) void bar(int i)
{
	printf("%d", i);
}
