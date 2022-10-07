#pragma once
#include <Windows.h>
#include <iostream>
#include <vector>
#include "ConvertString.h"

extern BOOL g_isFullText;

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
		DWORD dwUnknown2;
		DWORD dwTextBlockLen;
		DWORD dwUnknown3;
		DWORD dwCompressSize;
		DWORD dwDecompressSize;
		DWORD dwUnknown4;
	};

	struct PS3INFO
	{
		DWORD pPS3File;
		DWORD pCodeBlock;
		DWORD pTextBlock;
	};

public:
	PS3TextEditor();
	~PS3TextEditor();

	BYTE		m_abFlagPushStr[4];
	PS3INFO		m_PS3Info;
	PS3HEADER	m_Header;

	virtual BOOL GetPS3FileInfo() = 0;

};

class PS3TextDump : public PS3TextEditor
{
private:
	FILE*				m_fpTextFile;
	std::wstring		m_wsPath;
	std::vector<DWORD>	m_vppStr;

	BOOL GetPS3FileInfo();
	VOID SearchOffset();
	BOOL CreateDumpFile();
	VOID DumpText();

public:
	PS3TextDump(std::wstring& wsPath);
	~PS3TextDump();

};

class PS3TextInset : public PS3TextEditor
{
private:
	FILE*			m_fpPS3File;
	DWORD			m_countInset;
	std::wstring	m_wsPS3FilePath;
	std::wstring&	m_wsTextPath;

	BOOL GetPS3FileInfo();
	BOOL InsetTextFile();

public:
	PS3TextInset(std::wstring& TextPath);
	~PS3TextInset();

};
