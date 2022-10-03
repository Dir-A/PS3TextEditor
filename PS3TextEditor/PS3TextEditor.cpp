#include "PS3TextEditor.h"

PS3TextEditor::PS3TextEditor() :
	m_Header{ 0 },
	m_PS3Info{ 0 },
	m_abFlagPushStr{ 0x01,0x02,0x20,0x01 }
{

}

PS3TextEditor::~PS3TextEditor()
{

}


PS3TextDump::PS3TextDump(std::wstring& wsPath) :
	m_wsPath(wsPath),
	m_fpTextFile(0)
{
	if (GetPS3FileInfo())
	{
		SearchOffset();
		if (CreateDumpFile())
		{
			DumpText();
			std::wcout << "Save:" << m_wsPath << ".txt\n" << std::endl;
		}
		else
		{
			std::wcout << "Create DumpFile Failed!!!\n" << std::endl;
		}
	}
	else
	{
		std::wcout << "Read PS3File Failed!!!\n" << std::endl;
	}

}

PS3TextDump::~PS3TextDump()
{
	VirtualFree((LPVOID)m_PS3Info.pPS3File, NULL, MEM_RELEASE);
	fclose(m_fpTextFile);
}

BOOL PS3TextDump::GetPS3FileInfo()
{
	HANDLE hFile = CreateFileW(m_wsPath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	else
	{
		m_PS3Info.pPS3File = (DWORD)VirtualAlloc(NULL, GetFileSize(hFile, NULL), MEM_COMMIT, PAGE_EXECUTE_READWRITE);
		if (m_PS3Info.pPS3File != NULL)
		{
			if (ReadFile(hFile, (LPVOID)m_PS3Info.pPS3File, GetFileSize(hFile, NULL), NULL, NULL))
			{
				memcpy(&m_Header, (PVOID)m_PS3Info.pPS3File, sizeof(m_Header));
				m_PS3Info.pCodeBlock = m_PS3Info.pPS3File + m_Header.dwHeaderLen;
				m_PS3Info.pTextBlock = m_PS3Info.pPS3File + m_Header.dwHeaderLen + (4 * m_Header.dwTextCount) + m_Header.dwCodeBlockLen;
				CloseHandle(hFile);
				return TRUE;
			}
			else
			{
				CloseHandle(hFile);
				return FALSE;
			}
		}
		else
		{
			CloseHandle(hFile);
			return FALSE;
		}
	}

}

VOID PS3TextDump::SearchOffset()
{
	for (size_t i = 0; i < m_Header.dwCodeBlockLen; i++)
	{
		if (!memcmp(m_abFlagPushStr, (PVOID)(m_PS3Info.pCodeBlock + i), sizeof(m_abFlagPushStr)))
		{
			m_vppStr.push_back(m_PS3Info.pCodeBlock + i + sizeof(m_abFlagPushStr));
		}
	}
}

BOOL PS3TextDump::CreateDumpFile()
{
	errno_t err = 0;
	std::wstring dumpFileName = m_wsPath + L".txt";
	err = _wfopen_s(&m_fpTextFile, dumpFileName.c_str(), L"rt");
	if (!err && m_fpTextFile)
	{
		char flag = 0;
		std::wcout << L"The text file already exist and need to be overwritten?(Y/N)";
		std::cin >> flag;
		if (flag == 'N')
		{
			return FALSE;
		}
		fclose(m_fpTextFile);
	}
	err = _wfopen_s(&m_fpTextFile, dumpFileName.c_str(), L"wt+,ccs=UTF-16LE");
	return ~err;
}

VOID PS3TextDump::DumpText()
{
	DWORD strAddr = 0;
	DWORD textFileOffset = 0;
	DWORD codeFileOffset = 0;
	std::string strFilter;

	for (auto& p : m_vppStr)
	{
		strAddr = m_PS3Info.pTextBlock + *(PDWORD)p;
		strFilter = (PCHAR)strAddr;

		//Filter Files Name String Line
		if (strFilter.empty() ||
			strFilter.find(".ogg") != std::string::npos ||
			strFilter.find(".wav") != std::string::npos ||
			strFilter.find(".mv2") != std::string::npos ||
			strFilter.find(".pb3") != std::string::npos ||
			strFilter.find(".pb2") != std::string::npos ||
			strFilter.find(".ps3") != std::string::npos ||
			strFilter.find(".ps2") != std::string::npos ||
			strFilter.find(".cur") != std::string::npos ||
			strFilter.find(".cmv") != std::string::npos)
		{
			continue;
		}
		else
		{
			textFileOffset = strAddr - m_PS3Info.pPS3File;
			codeFileOffset = p - m_PS3Info.pPS3File;
			std::wstring wText = StrToWstr(strFilter);
			fwprintf_s(m_fpTextFile, L"[Text:0x%08X Code:0x%08X]\nRaw:%s\nTra:\n\n", textFileOffset, codeFileOffset, wText.c_str());
			fflush(m_fpTextFile);
		}
	}
}


PS3TextInset::PS3TextInset(std::wstring& wsPath) :
	m_wsTextPath(wsPath),
	m_countInset(0),
	m_fpPS3File(0)
{
	if (GetPS3FileInfo())
	{
		if (InsetTextFile())
		{
			std::wcout << "Save      :" << m_wsPS3FilePath << L".new" << std::endl;
			std::wcout << "InsetCount:" << m_countInset << '\n' << std::endl;
		}
		else
		{
			std::wcout << "Inset Text Failed!!!\n" << std::endl;
		}
	}
	else
	{
		std::wcout << "Read PS3File Failed!!!\n" << std::endl;
	}

}

PS3TextInset::~PS3TextInset()
{
	free((PVOID)m_PS3Info.pPS3File);
	fclose(m_fpPS3File);
}

BOOL PS3TextInset::GetPS3FileInfo()
{
	m_wsPS3FilePath = m_wsTextPath;
	m_wsPS3FilePath.erase(m_wsPS3FilePath.length() - 4, 4);//delete end of path string .txt
	CopyFileW(m_wsPS3FilePath.c_str(), (m_wsPS3FilePath + L".new").c_str(), FALSE);

	errno_t err = _wfopen_s(&m_fpPS3File, (m_wsPS3FilePath + L".new").c_str(), L"rb+");
	if (!err && m_fpPS3File)
	{
		fread_s((PVOID)&m_Header, 0x30, 1, 0x30, m_fpPS3File);
		fflush(m_fpPS3File);
		return TRUE;
	}
	else
	{
		return FALSE;
	}

}

BOOL PS3TextInset::InsetTextFile()
{
	std::string mText;
	std::wstring wText;
	WCHAR newText[0xFF] = { 0 };
	DWORD offsetCode = 0;
	DWORD strLen = 0;
	FILE* fpTextFile;

	errno_t err = _wfopen_s(&fpTextFile, m_wsTextPath.c_str(), L"rt+,ccs=UTF-16LE");
	if (!err && fpTextFile != NULL)
	{
		while (feof(fpTextFile) == 0)
		{
			if (fwscanf_s(fpTextFile, L"[Text:0x%*x Code:0x%x]\n\r", &offsetCode))
			{
				continue;
			}

			fwscanf_s(fpTextFile, L"Raw:%*[^\n]\r");

			if (!fwscanf_s(fpTextFile, L"Tra:%[^\n]\r", newText, sizeof(newText)))
			{
				//If Tra: is null
				fwscanf_s(fpTextFile, L"\n\r\n\r");
				continue;
			}

			//Append text at the end of the file
			wText = newText;
			mText = WstrToStr(wText);
			strLen = mText.length() + 1;

			fseek(m_fpPS3File, 0, SEEK_END);
			fwrite(mText.c_str(), sizeof(BYTE), strLen, m_fpPS3File);

			//Modify the codeblock PushStr address
			fseek(m_fpPS3File, offsetCode, SEEK_SET);
			fwrite(&m_Header.dwTextBlockLen, sizeof(BYTE), sizeof(m_Header.dwTextBlockLen), m_fpPS3File);

			//Fix the textblock size
			m_Header.dwTextBlockLen += strLen;
			fflush(m_fpPS3File);

			//Count the number of inserted texts
			m_countInset++;
		}

		if (m_countInset)
		{
			//Write back to .ps3 file header
			fseek(m_fpPS3File, 0, SEEK_SET);
			fwrite(&m_Header, sizeof(BYTE), sizeof(m_Header), m_fpPS3File);
			fflush(m_fpPS3File);
			fclose(fpTextFile);

			return TRUE;
		}

		fclose(fpTextFile);
		return FALSE;
	}

	return FALSE;
}

