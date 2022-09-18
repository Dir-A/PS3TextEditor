#pragma once
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <vector>

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
	PS3HEADER m_Header;
	PS3INFO m_PS3Info;
	BYTE m_abFlagPushStr[4];
	virtual BOOL ReadPS3File() = 0;
	virtual VOID SetPS3Info() = 0;
	PS3TextEditor();
	~PS3TextEditor();

};

class PS3TextDump : public PS3TextEditor
{
private:
	FILE* m_fpTextFile;
	std::wstring m_wsPath;
	std::vector<DWORD> m_vppStr;

public:
	BOOL ReadPS3File();
	VOID SetPS3Info();
	PS3TextDump(std::wstring& wsPath);
	~PS3TextDump();
	VOID SearchOffset();
	BOOL CreateDumpFile();
	VOID DumpText();

};

class PS3TextInset : public PS3TextEditor
{
private:
	FILE* m_fpPS3File;
	std::wstring& m_wsPath;

public:
	PS3TextInset(std::wstring& wsPath);
	~PS3TextInset();
	BOOL ReadPS3File();
	VOID SetPS3Info();
	BOOL InsetTextFile();

};