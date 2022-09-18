#pragma once
#include <Windows.h>
#include <string>
#include <vector>
#include <queue>

//网上抄的，不要bb
class EnumFilesA
{
public:
	EnumFilesA(std::string& strPath);
	VOID FindFiles(std::string);
	std::vector<std::string>& GetFilesPath();
	std::vector<std::string>& GetFilesName();
private:
	std::string rootPath;
	std::vector<std::string> filesPath;
	std::vector<std::string> filesName;
};

class EnumFilesW
{
public:
	EnumFilesW(std::wstring& strPath);
	VOID FindFiles(std::wstring);
	std::vector<std::wstring>& GetFilesPath();
	std::vector<std::wstring>& GetFilesName();
private:
	std::wstring rootPath;
	std::vector<std::wstring> filesPath;
	std::vector<std::wstring> filesName;
};
