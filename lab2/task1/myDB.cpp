#include "stdafx.h"
#include "myDB.h"

HANDLE openFile(LPCTSTR lpFileName, Header* pH)
{
	HANDLE hFile = CreateFile(lpFileName, GENERIC_READ | GENERIC_WRITE, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if ((INVALID_HANDLE_VALUE != hFile) && hFile)
	{
		readHeader(hFile, pH);
	}
	return hFile;
}

HANDLE createNewFile(LPCTSTR lpFileName, DWORD dwNumberOfNotes, Header* pH)
{
	HANDLE hFile;
	if (INVALID_HANDLE_VALUE != (hFile = CreateFile(lpFileName, GENERIC_READ | GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)))
	{
		pH->dwCountNotes = 0;
		pH->dwFileSize = dwNumberOfNotes * sizeof(Note);
		writeHeader(hFile, pH);
		SetFilePointer(hFile, dwNumberOfNotes * sizeof(Note) + sizeof(Header), 0, FILE_BEGIN);
		SetEndOfFile(hFile);
	}
	return hFile;
}

BOOL readHeader(HANDLE hF, Header* pH)
{
	if (SetFilePointer(hF, 0, 0, FILE_BEGIN) == INVALID_SET_FILE_POINTER) {
		return FALSE;
	}
	if (!ReadFile(hF, &(pH->dwFileSize), sizeof(DWORD), NULL, NULL)) {
		return FALSE;
	}
	return ReadFile(hF, &(pH->dwCountNotes), sizeof(DWORD), NULL, NULL);
}

BOOL writeHeader(HANDLE hF, Header* pH)
{
	if (SetFilePointer(hF, 0, 0, FILE_BEGIN) == INVALID_SET_FILE_POINTER) {
		return FALSE;
	}
	if (!WriteFile(hF, &pH->dwFileSize, sizeof(DWORD), NULL, NULL)) {
		return FALSE;
	}
	return WriteFile(hF, &pH->dwCountNotes, sizeof(DWORD), NULL, NULL);
}

BOOL readNote(HANDLE hF, Header* pH, DWORD dwID, Note* pNote)
{
	if (dwID * sizeof(Note) > pH->dwFileSize) {
		return FALSE;
	}

	if (SetFilePointer(hF, sizeof(Header) + sizeof(Note) * (dwID - 1), 0, FILE_BEGIN) == INVALID_SET_FILE_POINTER) {
		return FALSE;
	}
	return ReadFile(hF, pNote, sizeof(Note), NULL, NULL);
}

BOOL writeNote(HANDLE hF, Header* pH, DWORD dwID, CHAR* szStr)
{
	Note* pNote;
	if (dwID * sizeof(Note) > pH->dwFileSize) {
		return FALSE;
	}
	if (!(pNote = (Note*)LocalAlloc(LMEM_FIXED, sizeof(Note)))) {
		return FALSE;
	}
	if (!readNote(hF, pH, dwID, pNote)) {
		return FALSE;
	}
	if (pNote->dwId == 0) {
		pNote->dwId = dwID;
		pH->dwCountNotes++;
	}
	FileTimeToCurrentSystemTime(&pNote->lpLastChangeTime);
	pNote->dwCountChanges++;
	strcpy(pNote->szNote, szStr);
	if (SetFilePointer(hF, sizeof(Header) + sizeof(Note) * (dwID - 1), 0, FILE_BEGIN) == INVALID_SET_FILE_POINTER) {
		return FALSE;
	}
	if (!WriteFile(hF, pNote, sizeof(Note), NULL, NULL)) {
		return FALSE;
	}
	LocalFree(pNote);
	return writeHeader(hF, pH);
}

BOOL removeNote(HANDLE hF, Header* pH, DWORD dwID)
{
	Note* pNote;
	if (dwID * sizeof(Note) > pH->dwFileSize) {
		return FALSE;
	}
	if (!(pNote = (Note*)LocalAlloc(LMEM_FIXED, sizeof(Note)))) {
		return FALSE;
	}
	ZeroMemory(pNote, sizeof(Note));
	if (SetFilePointer(hF, sizeof(Header) + sizeof(Note) * (dwID - 1), 0, FILE_BEGIN) == INVALID_SET_FILE_POINTER) {
		return FALSE;
	}
	if (!WriteFile(hF, pNote, sizeof(Note), NULL, NULL)) {
		return FALSE;
	}
	LocalFree(pNote);
	return writeHeader(hF, pH);
}

void printNoteInfo(HANDLE hF, Header* pH, DWORD dwID)
{
	setlocale(LC_ALL, "RUS");
	Note* pNote;
	SYSTEMTIME st;
	if (!(pNote = (Note*)LocalAlloc(LMEM_FIXED, sizeof(Note)))) {
		printf("Невозможно выделить память!\n");
		return;
	}
	if (!readNote(hF, pH, dwID, pNote)) {
		printf("Невозможно прочитать запись!\n");
		return;
	}
	if (pNote->dwId == 0) {
		printf("Запись #%u - пустая!\n", dwID);
		return;
	}
	printf("ID: %u\n", pNote->dwId);
	printf("Количество изменений: %u\n", pNote->dwCountChanges);
	FileTimeToSystemTime(&pNote->lpLastChangeTime, &st);
	printf("Последнее изменение: %d-%02d-%02d %02d:%02d:%02d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	printf("Текст записи: %s\n", pNote->szNote);
	LocalFree(pNote);
}

void FileTimeToCurrentSystemTime(PFILETIME ft)
{
	SYSTEMTIME st;
	GetSystemTime(&st);
	SystemTimeToFileTime(&st, ft);
}

BOOL isExistFile(LPCTSTR szPath)
{
	DWORD dwAttrib = GetFileAttributes(szPath);
	return (dwAttrib != INVALID_FILE_ATTRIBUTES && !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}