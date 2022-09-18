#include <iostream>
#include "PS3TextEditor.h"
#include "EnumFiles.h"

int main()
{
	std::vector<std::wstring> filesPathW;
	std::wstring basePathW = L".\\";
	EnumFilesW enumFileW(basePathW);
	filesPathW = enumFileW.GetFilesPath();

	char flag = 0;
	flag = getchar();

	switch (flag)
	{
	case 'd':
		for (std::wstring& fw : filesPathW)
		{
			if (fw.find(L".ps3") != std::wstring::npos)
			{
				PS3TextDump dump(fw);
			}
		}
		break;
	case 'i':
		for (std::wstring& fw : filesPathW)
		{
			if (fw.find(L".ps3.txt") != std::wstring::npos)
			{
				PS3TextInset inset(fw);
			}
		}
		break;
	}

}