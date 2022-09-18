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
	if (ReadPS3File())
	{
		SetPS3Info();
		SearchOffset();
		if (CreateDumpFile())
		{
			DumpText();
		}
	}
}

PS3TextDump::~PS3TextDump()
{
	VirtualFree((LPVOID)m_PS3Info.pPS3File, NULL, MEM_RELEASE);
	fclose(m_fpTextFile);
}

BOOL PS3TextDump::ReadPS3File()
{
	HANDLE hFile = CreateFileW(m_wsPath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	else
	{
		m_PS3Info.pPS3File = (DWORD)VirtualAlloc(NULL, GetFileSize(hFile, NULL), MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	}

	if (ReadFile(hFile, (LPVOID)m_PS3Info.pPS3File, GetFileSize(hFile, NULL), NULL, NULL))
	{
		CloseHandle(hFile);
		return TRUE;
	}
	else
	{
		CloseHandle(hFile);
		return FALSE;
	}
}

VOID PS3TextDump::SetPS3Info()
{
	memcpy(&m_Header, (void*)m_PS3Info.pPS3File, 0x30);
	m_PS3Info.pCodeBlock = m_PS3Info.pPS3File + m_Header.dwHeaderLen;
	m_PS3Info.pTextBlock = m_PS3Info.pPS3File + m_Header.dwHeaderLen + (4 * m_Header.dwTextCount) + m_Header.dwCodeBlockLen;
}

VOID PS3TextDump::SearchOffset()
{
	for (size_t i = 0; i < m_Header.dwCodeBlockLen; i++)
	{
		if (!memcmp(m_abFlagPushStr, (void*)(m_PS3Info.pCodeBlock + i), sizeof(m_abFlagPushStr)))
		{
			m_vppStr.push_back(m_PS3Info.pCodeBlock + i + sizeof(m_abFlagPushStr));
		}
	}
}

BOOL PS3TextDump::CreateDumpFile()
{
	std::wstring dumpFileName = m_wsPath + L".txt";
	errno_t err = _wfopen_s(&m_fpTextFile, dumpFileName.c_str(), L"w+");
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
		strAddr = m_PS3Info.pTextBlock + *(DWORD*)p;
		strFilter = (char*)strAddr;

		if (strFilter.empty() ||
			strFilter.find(".ogg") != std::string::npos ||
			strFilter.find(".pb3") != std::string::npos ||
			strFilter.find(".ps3") != std::string::npos ||
			strFilter.find(".cmv") != std::string::npos)
		{
			continue;
		}
		else
		{
			textFileOffset = strAddr - m_PS3Info.pPS3File;
			codeFileOffset = p - m_PS3Info.pPS3File;
			fprintf_s(m_fpTextFile, "[Text:0x%08X Code:0x%08X]\nRaw:%s\nTra:\n\n", textFileOffset, codeFileOffset, (char*)strAddr);
			fflush(m_fpTextFile);
		}
	}
}



PS3TextInset::PS3TextInset(std::wstring& wsPath) : m_wsPath(wsPath)
{
	ReadPS3File();
	SetPS3Info();
	InsetTextFile();
}

PS3TextInset::~PS3TextInset()
{
	free((void*)m_PS3Info.pPS3File);
	fclose(m_fpPS3File);
}

BOOL PS3TextInset::ReadPS3File()
{
	std::wstring ps3file;
	ps3file = m_wsPath;
	ps3file.erase(ps3file.length() - 4, 4);

	errno_t err = _wfopen_s(&m_fpPS3File, ps3file.c_str(), L"r+");
	if (!err && m_fpPS3File)
	{
		m_PS3Info.pPS3File = (DWORD)malloc(0x30);
		if (m_PS3Info.pPS3File != NULL)
		{
			fread_s((void*)m_PS3Info.pPS3File, 0x30, 1, 0x30, m_fpPS3File);
			return TRUE;
		}
		return FALSE;
	}
	else
	{
		return FALSE;
	}

}

VOID PS3TextInset::SetPS3Info()
{
	memcpy(&m_Header, (void*)m_PS3Info.pPS3File, 0x30);
	m_PS3Info.pCodeBlock = m_PS3Info.pPS3File + m_Header.dwHeaderLen;
	m_PS3Info.pTextBlock = m_PS3Info.pPS3File + m_Header.dwHeaderLen + (4 * m_Header.dwTextCount) + m_Header.dwCodeBlockLen;
}

BOOL PS3TextInset::InsetTextFile()
{
	char newText[0xFF] = { 0 };
	DWORD offsetCode = 0;
	DWORD strLen = 0;
	FILE* fpTextFile;

	errno_t err = _wfopen_s(&fpTextFile, m_wsPath.c_str(), L"r+");
	if (!err && fpTextFile != NULL)
	{
		while (feof(fpTextFile) == 0)
		{
			fscanf_s(fpTextFile, "[Text:0x%*x Code:0x%x]\n\r", &offsetCode);
			fscanf_s(fpTextFile, "Raw:%*[^\n]\r");
			if (!fscanf_s(fpTextFile, "Tra:%[^\n]\r", newText, sizeof(newText)))
			{
				//If Tra: is null
				fscanf_s(fpTextFile, "\n\r\n\r");
				continue;
			}
			strLen = strlen(newText) + 1;

			//Append text at the end of the file
			fseek(m_fpPS3File, 0, SEEK_END);
			fwrite(newText, sizeof(char), strLen, m_fpPS3File);

			//Modify the codeblock PushStr address
			fseek(m_fpPS3File, offsetCode, SEEK_SET);
			fwrite(&m_Header.dwTextBlockLen, sizeof(char), 4, m_fpPS3File);

			//Fix the textblock size
			m_Header.dwTextBlockLen += strLen;
			fflush(m_fpPS3File);
		}

		//Write back to .ps3 file header
		fseek(m_fpPS3File, 0, SEEK_SET);
		fwrite(&m_Header, sizeof(char), sizeof(m_Header), m_fpPS3File);
		fflush(m_fpPS3File);
		fclose(fpTextFile);
		return TRUE;
	}

	return FALSE;
}

