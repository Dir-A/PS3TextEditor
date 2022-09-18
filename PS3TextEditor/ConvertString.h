#pragma once
#include <Windows.h>
#include <string>

std::string WstrToStr(std::wstring& wStr);
std::wstring StrToWstr(std::string& sStr);