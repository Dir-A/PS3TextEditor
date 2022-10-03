#include "ConvertString.h"

std::string WstrToStr(std::wstring& wStr)
{
	std::string result;

	int len = WideCharToMultiByte(CP_ACP, NULL, wStr.c_str(), wStr.size(), NULL, NULL, NULL, NULL);
	char* buffer = new char[len + 1];

	WideCharToMultiByte(CP_ACP, NULL, wStr.c_str(), wStr.size(), buffer, len, NULL, NULL);
	buffer[len] = '\0';

	result.append(buffer);
	delete[] buffer;
	return result;
}

std::wstring StrToWstr(std::string& sStr)
{
	std::wstring result;

	int len = MultiByteToWideChar(932, NULL, sStr.c_str(), sStr.size(), NULL, NULL);
	wchar_t* buffer = new wchar_t[len + 1];

	MultiByteToWideChar(932, NULL, sStr.c_str(), sStr.size(), buffer, len);
	buffer[len] = '\0';

	result.append(buffer);
	delete[] buffer;
	return result;
}