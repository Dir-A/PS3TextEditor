#include <iostream>
#include "PS3TextEditor.h"
#include "EnumFiles.h"

int main()
{
	std::vector<std::wstring> filesPathW;
	std::wstring basePathW = L".\\";
	EnumFilesW enumFileW(basePathW);
	filesPathW = enumFileW.GetFilesPath();

	for (std::wstring fw : filesPathW)
	{
		if (fw.find(L".ps3") != std::wstring::npos)
		{
			PS3TextEditor p(fw);
		}
	}
}