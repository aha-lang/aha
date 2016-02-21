#pragma once

#include <boost/format.hpp>
#include <utility>
#include <iostream>

#include "AhaLangPack.h"

template <typename ...Args> struct PrintError_Impl;

template <typename T, typename ...Args> struct PrintError_Impl<T, Args...>
{
	static std::wstring func(boost::wformat &format, T&& t, Args&&... args)
	{
		return PrintError_Impl<Args...>::func(format % std::forward<T>(t), std::forward<Args>(args)...);
	}
};
template <> struct PrintError_Impl<>
{
	static std::wstring func(boost::wformat &format)
	{
		return format.str();
	}
};

template <typename ...Args>
inline void PrintError(const std::string& err_code, const std::string& src, long line, Args&&... args)
{
	extern AhaLangPack err_lang;

	const wchar_t* err = err_lang.GetValue(err_code).c_str();
	std::wstring str = PrintError_Impl<Args...>::func(boost::wformat(err_lang.GetValue(err_code)), std::forward<Args>(args)...);

	std::cout << src;
	std::wcout << L"(" << line << L") : " << str.c_str() << std::endl;
}