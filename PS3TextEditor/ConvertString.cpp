#include "ConvertString.h"

std::string WstrToStr(std::wstring& wstrString, DWORD dwCodePage)
{
	std::string result;

	DWORD lenStr = WideCharToMultiByte(dwCodePage, NULL, wstrString.c_str(), wstrString.size(), NULL, NULL, NULL, NULL);
	char* buffer = new char[lenStr + 1];

	WideCharToMultiByte(dwCodePage, NULL, wstrString.c_str(), wstrString.size(), buffer, lenStr, NULL, NULL);
	buffer[lenStr] = '\0';

	result.append(buffer);
	delete[] buffer;
	return result;
}

std::wstring StrToWstr(std::string& strString, DWORD dwCodePage)
{
	std::wstring result;

	DWORD lenWStr = MultiByteToWideChar(dwCodePage, NULL, strString.c_str(), strString.size(), NULL, NULL);
	wchar_t* buffer = new wchar_t[lenWStr + 1];

	MultiByteToWideChar(dwCodePage, NULL, strString.c_str(), strString.size(), buffer, lenWStr);
	buffer[lenWStr] = '\0';

	result.append(buffer);
	delete[] buffer;
	return result;
}