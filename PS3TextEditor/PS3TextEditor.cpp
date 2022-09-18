#include "PS3TextEditor.h"

PS3TextEditor::PS3TextEditor(std::wstring& wsFilePath) :
	m_Header{ 0 },
	m_abFlagPushStr{ 0x01,0x02,0x20,0x01 },
	m_pCodeBlock(0),
	m_pTextBlock(0),
	m_dwFileSize(0),
	m_hPS3File(0),
	m_pPS3File(0),
	m_wsFilePath(wsFilePath)

{
	DumpPS3Text();
}

PS3TextEditor::~PS3TextEditor()
{

}

BOOL PS3TextEditor::DumpPS3Text()
{
	if (ReadPS3File() && GetPS3Info())
	{
		SearchOffset();
		if (CreateDumpFile())
		{
			ReadPS3Text();
			CleanPS3File();
			return TRUE;
		}
	}
	return FALSE;
}

VOID PS3TextEditor::CleanPS3File()
{
	VirtualFree((LPVOID)m_pPS3File, NULL, MEM_RELEASE);
	fclose(fp);
}

BOOL PS3TextEditor::ReadPS3File()
{
	m_hPS3File = CreateFileW(m_wsFilePath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (m_hPS3File == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	else
	{
		m_dwFileSize = GetFileSize(m_hPS3File, NULL);
		m_pPS3File = (DWORD)VirtualAlloc(NULL, m_dwFileSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	}

	if (!ReadFile(m_hPS3File, (LPVOID)m_pPS3File, GetFileSize(m_hPS3File, NULL), NULL, NULL))
	{
		CloseHandle(m_hPS3File);
		return FALSE;
	}
	else
	{
		CloseHandle(m_hPS3File);
		return TRUE;
	}
}

BOOL PS3TextEditor::GetPS3Info()
{
	memcpy(&m_Header, (void*)m_pPS3File, 0x30);
	m_pCodeBlock = m_pPS3File + m_Header.header_length;
	m_pTextBlock = m_pPS3File + m_Header.header_length + (4 * m_Header.text_count) + m_Header.code_block_length;

	return TRUE;
}

VOID PS3TextEditor::SearchOffset()
{
	for (size_t i = 0; i < m_Header.code_block_length; i++)
	{
		if (!memcmp(m_abFlagPushStr, (void*)(m_pCodeBlock + i), 0x4))
		{
			m_vdOffsetPushStr.push_back(m_pCodeBlock + i + sizeof(m_abFlagPushStr));
		}
	}
}

BOOL PS3TextEditor::CreateDumpFile()
{
	std::wstring dumpFileName = m_wsFilePath + L".txt";
	errno_t err = _wfopen_s(&fp, dumpFileName.c_str(), L"w+");
	return ~err;
}

VOID PS3TextEditor::ReadPS3Text()
{
	DWORD strAddr = 0;
	DWORD textFileOffset = 0;
	DWORD codeFileOffset = 0;
	std::string strFilter;

	for (auto& p : m_vdOffsetPushStr)
	{
		strAddr = m_pTextBlock + *(DWORD*)p;
		strFilter = (char*)strAddr;

		if (strFilter.empty() ||
			strFilter.find(".ogg") != std::string::npos ||
			strFilter.find(".pb3") != std::string::npos ||
			strFilter.find(".cmv") != std::string::npos ||
			strFilter.find(".ps3") != std::string::npos)
		{
			continue;
		}
		else
		{
			textFileOffset = strAddr - m_pPS3File;
			codeFileOffset = p - m_pPS3File;
			fprintf_s(fp, "[Text:0x%04X Code:0x%04X]\nRaw:%s\nTra:\n\n", textFileOffset, codeFileOffset, (char*)strAddr);
			fflush(fp);
		}
	}
}

