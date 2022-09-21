#include "EnumFiles.h"

//EnumFilesA
EnumFilesA::EnumFilesA(std::string& strPath) :rootPath(strPath)
{
	FindFiles(rootPath);
}

VOID EnumFilesA::FindFiles(std::string strPath)
{
	WIN32_FIND_DATAA fd;
	std::queue<std::string> directory;

	if (strPath[strPath.size() - 1] != '\\')
	{
		strPath += "\\";
	}
	directory.push(strPath);

	while (!directory.empty())
	{
		std::string everyPath = directory.front();
		std::string tempPath = everyPath + "*";

		HANDLE hFind = FindFirstFileA(tempPath.c_str(), &fd);
		if (hFind == INVALID_HANDLE_VALUE)
		{
			FindClose(hFind);
			return;
		}
		do
		{
			std::string isFile = fd.cFileName;
			if ((fd.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE) != 0)
			{
				filesPath.push_back(everyPath + isFile);
			}
			else if ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0 && (isFile != "." && isFile != ".."))
			{
				std::string Rp = everyPath + fd.cFileName + "\\";
				directory.push(Rp);
			}
		} while (FindNextFileA(hFind, &fd));
		directory.pop();
		FindClose(hFind);
	}
}

std::vector<std::string>& EnumFilesA::GetFilesPath()
{
	return filesPath;
}

std::vector<std::string>& EnumFilesA::GetFilesName()
{
	SIZE_T len = 0;
	std::string name;

	for (std::string p : filesPath)
	{
		len = p.find_last_of("\\") + 1;
		name = p.substr(len, p.length() - len);
		filesName.push_back(name);
	}
	return filesName;
}

std::vector<std::string>& EnumFilesA::GetFilesNameBasePath()
{
	for (std::string p : filesPath)
	{
		if (p.find("\\", 2) == std::string::npos)
		{
			filesNameBasePath.push_back(p);
		}
	}
	return filesNameBasePath;
}

//EnumFilesW
EnumFilesW::EnumFilesW(std::wstring& strPath) :rootPath(strPath)
{
	FindFiles(rootPath);
}

VOID EnumFilesW::FindFiles(std::wstring strPath)
{
	WIN32_FIND_DATAW fd;
	std::queue<std::wstring> directory;

	if (strPath[strPath.size() - 1] != L'\\')
	{
		strPath += L"\\";
	}
	directory.push(strPath);

	while (!directory.empty())
	{
		std::wstring everyPath = directory.front();
		std::wstring tempPath = everyPath + L"*";

		HANDLE hFind = FindFirstFileW(tempPath.c_str(), &fd);
		if (hFind == INVALID_HANDLE_VALUE)
		{
			FindClose(hFind);
			return;
		}
		do
		{
			std::wstring isFile = fd.cFileName;
			if ((fd.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE) != 0)
			{
				filesPath.push_back(everyPath + isFile);
			}
			else if ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0 && (isFile != L"." && isFile != L".."))
			{
				std::wstring Rp = everyPath + fd.cFileName + L"\\";
				directory.push(Rp);
			}
		} while (FindNextFileW(hFind, &fd));
		directory.pop();
		FindClose(hFind);
	}
}

std::vector<std::wstring>& EnumFilesW::GetFilesPath()
{
	return filesPath;
}

std::vector<std::wstring>& EnumFilesW::GetFilesName()
{
	SIZE_T len = 0;
	std::wstring name;

	for (std::wstring p : filesPath)
	{
		len = p.find_last_of(L"\\") + 1;
		name = p.substr(len, p.length() - len);
		filesName.push_back(name);
	}
	return filesName;
}

std::vector<std::wstring>& EnumFilesW::GetFilesNameBasePath()
{
	for (std::wstring p : filesPath)
	{
		if (p.find(L"\\", 2) == std::wstring::npos)
		{
			filesNameBasePath.push_back(p);
		}
	}
	return filesNameBasePath;
}