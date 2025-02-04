#pragma once
#include <string>
#include <stringapiset.h>

static std::string WStringToString(const std::wstring& someString)
{
	const int sLength = static_cast<int>(someString.length());
	const int len = WideCharToMultiByte(CP_ACP, 0, someString.c_str(), sLength, 0, 0, 0, 0);
	std::string result(len, L'\0');
	WideCharToMultiByte(CP_ACP, 0, someString.c_str(), sLength, &result[0], len, 0, 0);
	return result;
}

static std::wstring StringToWString(const std::string& someString)
{
	const int sLength = static_cast<int>(someString.length());
	const int len = MultiByteToWideChar(CP_ACP, 0, someString.c_str(), sLength, 0, 0);
	std::wstring result(len, L'\0');
	MultiByteToWideChar(CP_ACP, 0, someString.c_str(), sLength, &result[0], len);
	return result;
}