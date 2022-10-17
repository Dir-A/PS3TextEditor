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

	std::vector<std::string>& GetFilesPath();
	std::vector<std::string>& GetFilesName();
	std::vector<std::string>& GetFilesNameBasePath();

private:
	VOID FindFiles(std::string);

	std::string rootPath;
	std::vector<std::string> filesPath;
	std::vector<std::string> filesName;
	std::vector<std::string> filesNameBasePath;
};

class EnumFilesW
{
public:
	EnumFilesW(std::wstring& strPath);

	std::vector<std::wstring>& GetFilesPath();
	std::vector<std::wstring>& GetFilesName(); 
	std::vector<std::wstring>& GetFilesNameBasePath();
private:
	VOID FindFiles(std::wstring);

	std::wstring rootPath;
	std::vector<std::wstring> filesPath;
	std::vector<std::wstring> filesName;
	std::vector<std::wstring> filesNameBasePath;
};
