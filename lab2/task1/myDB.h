#pragma once

#define STR_MAX 81

struct Header
{
	DWORD dwFileSize;
	DWORD dwCountNotes;
};

struct Note
{
	DWORD dwId;
	FILETIME lpLastChangeTime;
	CHAR szNote[STR_MAX];
	DWORD dwCountChanges;
};

void FileTimeToCurrentSystemTime(PFILETIME ft);

BOOL isExistFile(LPCTSTR szPath);

BOOL readHeader(HANDLE hF, Header* pH);

BOOL writeHeader(HANDLE hF, Header* pH);

HANDLE createNewFile(LPCTSTR lpFileName, DWORD dwNumberOfNotes, Header* pHeader);

HANDLE openFile(LPCTSTR lpFileName, Header* pHeader);

BOOL readNote(HANDLE hF, Header* pH, DWORD dwID, Note* pNote);

BOOL writeNote(HANDLE hF, Header* pH, DWORD dwID, CHAR* szString);

BOOL removeNote(HANDLE hF, Header* pH, DWORD dwID);

void printNoteInfo(HANDLE hF, Header* pH, DWORD dwID);