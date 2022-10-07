#include <iostream>
#include "PS3TextEditor.h"
#include "EnumFiles.h"

int main()
{
	CHAR						flag = 0;
	std::vector<std::wstring>	filesNameW;
	std::wstring				basePathW = L".\\";

	std::cout << "Input [ d ] to Extract Text From .ps3 .ps2 Files and Save it as .txt\n";
	std::cout << "Input [ i ] to Insert Text From .txt Files and Save it as .ps3.new\n";
	std::cout << "Input [ t ] to Copy Raw: text to Tra: When Extracting  Text\n";
	std::cout << "Input [ c ] to Set Extract / Inset Sscript CodePage\n" << std::endl;

	while (true)
	{
		std::cout << "Input:";
		std::cin >> flag;
		std::cout << std::endl;

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

		case 't':
			g_isFullText = TRUE;
			break;

		case 'c':
			std::wcout << L"Set Extract / Inset Sscript CodePage" << std::endl;
			std::wcout << L"Insert CodePage:";
			std::wcin >> g_dwInCodePage;
			std::wcout << L"Set Insert CodePage to " << g_dwInCodePage << std::endl;
			std::wcout << L"Extract CodePage:";
			std::wcin >> g_dwExCodePage;
			std::wcout << L"Set Extract CodePage to " << g_dwInCodePage << std::endl;
			std::cout << std::endl;
			break;
		}
	}


	system("pause");

}