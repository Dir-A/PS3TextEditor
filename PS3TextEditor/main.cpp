#include <iostream>
#include "PS3TextEditor.h"
#include "EnumFiles.h"

int main()
{
	std::vector<std::wstring> filesNameW;
	std::wstring basePathW = L".\\";
	EnumFilesW enumFileW(basePathW);
	filesNameW = enumFileW.GetFilesName();

	std::cout << "Type 'd' to extract all the text in the .ps3 file and save it as .txt\n"
			  << "Type 'i' to insert the text of all .txt files into the .ps3 file\n" << std::endl;
	std::cout << "Input:";

	char flag = 0;
	std::cin >> flag;
	switch (flag)
	{
	case 'd':
		for (std::wstring& fw : filesNameW)
		{

			if (fw.find(L".ps3") != std::wstring::npos && fw.find(L".ps3.txt") == std::wstring::npos)
			{
				std::wcout << "Dump:" << fw << std::endl;
				PS3TextDump dump(fw);
			}
		}
		break;
	case 'i':
		for (std::wstring& fw : filesNameW)
		{
			if (fw.find(L".ps3.txt") != std::wstring::npos)
			{
				std::wcout << "Inset   :" << fw << std::endl;
				PS3TextInset inset(fw);
			}
		}
		break;
	}

	system("pause");

}