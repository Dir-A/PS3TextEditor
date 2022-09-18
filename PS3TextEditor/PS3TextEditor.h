#pragma once
#include <Windows.h>
#include <iostream>
#include <vector>

class PS3TextEditor
{
private:
	struct PS3HEADER
	{
		DWORD signature;
		DWORD header_length;
		DWORD unknown;
		DWORD key;
		DWORD text_count;
		DWORD code_block_length;
		DWORD unknown2;
		DWORD text_block_length;
		DWORD unknown3;
		DWORD compress_size;
		DWORD decompress_size;
		DWORD unknown4;
	};

	PS3HEADER m_Header;
	BYTE m_abFlagPushStr[4];
	DWORD m_pCodeBlock;
	DWORD m_pTextBlock;
	DWORD m_dwFileSize;
	HANDLE m_hPS3File;
	DWORD m_pPS3File;
	FILE* fp;
	std::wstring m_wsFilePath;
	std::vector<DWORD> m_vdOffsetPushStr;
	//std::vector<DWORD> m_vsText;
public:
	PS3TextEditor(std::wstring& wsFilePath);
	BOOL DumpPS3Text();
	BOOL ReadPS3File();
	BOOL GetPS3Info();
	VOID SearchOffset();
	VOID ReadPS3Text();
	VOID CleanPS3File();
	BOOL CreateDumpFile();
	~PS3TextEditor();

};

