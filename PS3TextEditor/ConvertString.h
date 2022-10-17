#pragma once
#include <Windows.h>
#include <string>

std::string WstrToStr(std::wstring& wstrString, DWORD dwCodePage);
std::wstring StrToWstr(std::string& strString, DWORD dwCodePage);