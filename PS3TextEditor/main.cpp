#include <iostream>
#include "PS3TextEditor.h"
#include "EnumFiles.h"

int main()
{
	std::cout << "Input 'd' to extract text from .ps3 .ps2 files and save it as .txt\n";
	std::cout << "Input 'i' to insert text from .txt files save it as .ps3.new\n" << std::endl;
	std::cout << "Input:";

	char flag = 0;
	std::cin >> flag;
	std::cout << std::endl;

	std::vector<std::wstring> filesNameW;
	std::wstring basePathW = L".\\";
	EnumFilesW enumFileW(basePathW);
	filesNameW = enumFileW.GetFilesNameBasePath();

	switch (flag)
	{
	case 'd':
		for (std::wstring& fw : filesNameW)
		{

			if (fw.find(L".ps3", fw.length() - 0x4) != std::wstring::npos ||
				fw.find(L".ps2", fw.length() - 0x4) != std::wstring::npos)
			{
				std::wcout << "Dump:" << fw << std::endl;
				PS3TextDump dump(fw);
			}
		}
		break;
	case 'i':
		for (std::wstring& fw : filesNameW)
		{
			if (fw.find(L".ps3.txt", fw.length() - 0x9) != std::wstring::npos || 
				fw.find(L".ps2.txt", fw.length() - 0x9) != std::wstring::npos)
			{
				std::wcout << "Inset     :" << fw << std::endl;
				PS3TextInset inset(fw);
			}
		}
		break;
	}

	system("pause");

}