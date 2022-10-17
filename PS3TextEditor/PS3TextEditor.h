#pragma once
#include <Windows.h>
#include <iostream>
#include <vector>
#include "ConvertString.h"


class PS3TextEditor
{
private:
	struct PS3HEADER
	{
		DWORD dwSignature;
		DWORD dwHeaderLen;
		DWORD dwUnknown0;
		DWORD dwKey;
		DWORD dwTextCount;
		DWORD dwCodeBlockLen;
		DWORD dwUnknown1;
		DWORD dwTextBlockLen;
		DWORD dwUnknown2;
		DWORD dwCompressSize;
		DWORD dwDecompressSize;
		DWORD dwUnknown3;
	};

	struct PS3INFO
	{
		DWORD pPS3File;
		DWORD pCodeBlock;
		DWORD pTextBlock;
		DWORD dwCodeBlockLen;
	};

public:
	PS3TextEditor();
	~PS3TextEditor();

	PS3INFO m_PS3Info;
	PS3HEADER m_Header;
	BYTE m_abFlagPushStr[4];

	virtual BOOL GetPS3FileInfo() = 0;

};

class PS3TextDump : public PS3TextEditor
{
private:
	BOOL m_isFullText;
	DWORD m_dwCodePage;
	FILE* m_fpTextFile;
	std::wstring m_wsPath;
	std::vector<DWORD> m_vppStr;

	BOOL GetPS3FileInfo();
	VOID SearchOffset();
	BOOL CreateDumpFile();
	VOID DumpText();

public:
	PS3TextDump(std::wstring& wsPath, DWORD dwCodePage, BOOL isFullText);
	~PS3TextDump();

};

class PS3TextInset : public PS3TextEditor
{
private:
	DWORD m_dwCodePage;
	FILE* m_fpPS3File;
	DWORD m_countInset;
	std::wstring& m_wsTextPath;
	std::wstring m_wsPS3FilePath;

	BOOL GetPS3FileInfo();
	BOOL InsetTextFile();

public:
	PS3TextInset(std::wstring& wsPath, DWORD dwCodePage);
	~PS3TextInset();

};
