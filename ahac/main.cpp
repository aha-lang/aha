#include <stdio.h>

#include <vector>
#include <string>

#include "AhaCompiler.h"

struct param_error : public std::runtime_error {
	param_error(const std::string& msg)
		: std::runtime_error(msg) { }
};

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		printf("Usage: %s -o <out> -f <src1> <src2> ...\n", argv[0]);
		return -1;
	}

	try
	{
		printf("Aha Compiler v100\n");
		printf("Copyright�� 2016 PJY All rights reserved.\n");

		std::string outfile, asmout;
		std::vector<std::string> srcs;

		bool is_src = false;

		for (int i = 1; i < argc; i++)
		{
			if (is_src)
			{
				srcs.push_back(argv[i]);
			}
			else
			{
				if (strcmp(argv[i], "-o") == 0)
				{
					if (argc - i == 0)
					{
						throw param_error("�Ű����ڰ� �����մϴ�.");
					}

					outfile = argv[++i];
				}
				else if (strcmp(argv[i], "-a") == 0)
				{
					if (argc - i == 0)
					{
						throw param_error("�Ű����ڰ� �����մϴ�.");
					}

					asmout = argv[++i];
				}
				else if (strcmp(argv[i], "-f") == 0)
				{
					is_src = true;
				}
			}
		}

		if (outfile.empty() && asmout.empty())
		{
			throw param_error("������ ���� �̸��� �������� �ʾҽ��ϴ�.");
		}

		if (srcs.empty())
		{
			throw param_error("�������� �ҽ� ������ �����ϴ�.");
		}

		std::vector<source> src;
		for (auto& it : srcs) src.push_back(source(it));
		compile(src);
	}
	catch (const compiler_error& e)
	{
		printf("Error: %s(%d): %s\n", e.pos.file.c_str(), e.pos.line, e.what());
		return -1;
	}
	catch (const param_error& e)
	{
		printf("Error: %s\n", e.what());
		return -1;
	}

	return -1;
}