#pragma once
#include <Windows.h>
#include <string>

extern DWORD g_dwExCodePage;
extern DWORD g_dwInCodePage;

std::string WstrToStr(std::wstring& wStr);
std::wstring StrToWstr(std::string& sStr);