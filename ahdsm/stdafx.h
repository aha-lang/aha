#pragma once

#if defined(_MSC_VER) && defined(_DEBUG)
# define _CRTDBG_MAP_ALLOC
# include <crtdbg.h>
#endif

#include <stdio.h>
#include <uchar.h>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <locale>
#include <codecvt>

#include <boost/scope_exit.hpp>
