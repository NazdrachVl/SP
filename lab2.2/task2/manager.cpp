#include "stdafx.h"
#include "manager.h"

int FileTimeToString(FILETIME FileTime, DWORD nBufferLength, LPTSTR lpBuffer);
void printErrorMessage(HRESULT hResult = NULL);

void printCurDir()
{
	TCHAR szBuffer[MAX_PATH];
	DWORD dwRet;
	dwRet = GetCurrentDirectory(MAX_PATH, szBuffer);
	if (dwRet == 0) {
		printErrorMessage();
		return;
	}
	if (dwRet > MAX_PATH) {
		printf("Слишком маленький буфер; требуется %d символов\n", dwRet);
		return;
	}
	_tprintf(TEXT("Текущая директория: %s\n"), szBuffer);
}

void setCurDir(LPCTSTR lpPathName)
{
	if (!SetCurrentDirectory(lpPathName)) {
		printErrorMessage();
		return;
	}
	_tprintf(TEXT("Текущая директория изменена на %s\n"), lpPathName);
}

void copyFile(LPCTSTR lpExistingFileName, LPCTSTR lpNewFileName)
{
	CopyFileEx(lpExistingFileName, lpNewFileName, NULL, NULL, NULL, NULL);
}

void createDir(LPCTSTR lpPathName)
{
	if (!CreateDirectory(lpPathName, NULL)) {
		printErrorMessage();
		return;
	}
}

void removeFile(LPCTSTR lpPathName)
{
	WIN32_FIND_DATA ffd;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	hFind = FindFirstFile(lpPathName, &ffd);
	if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
		if (!RemoveDirectory(lpPathName)) {
			printErrorMessage();
		}
	}
	else {
		if (!DeleteFile(lpPathName)) {
			printErrorMessage();
		}
	}
}

void printFiles()
{
	WIN32_FIND_DATA fileFD;
	LARGE_INTEGER filesize;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	TCHAR szDir[MAX_PATH];
	DWORD dwError = 0;
	GetCurrentDirectory(MAX_PATH, szDir);
	StringCchCat(szDir, MAX_PATH, TEXT("\\*"));
	hFind = FindFirstFile(szDir, &fileFD);
	do
	{
		if (fileFD.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			_tprintf(TEXT("  %s    <DIR>\n"), fileFD.cFileName);
		}
		else {
			filesize.LowPart = fileFD.nFileSizeLow;
			filesize.HighPart = fileFD.nFileSizeHigh;
			_tprintf(TEXT("  %s    %I64d байт\n"), fileFD.cFileName, filesize.QuadPart);
		}
	} while (FindNextFile(hFind, &fileFD) != 0);
}

void printFileInfo(LPCTSTR lpPathName)
{
	HANDLE hFile = NULL;
	if ((hFile = CreateFile(lpPathName, GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL)) == INVALID_HANDLE_VALUE)
	{
		printErrorMessage();
		return;
	}
	BY_HANDLE_FILE_INFORMATION fileInfo;
	if (!GetFileInformationByHandle(hFile, &fileInfo)) {
		printErrorMessage();
		return;
	}
	TCHAR szBuffer[MAX_PATH];
	GetFullPathName(lpPathName, MAX_PATH, szBuffer, NULL);
	_tprintf(TEXT("Полный путь: %s\n"), szBuffer);

	FileTimeToString(fileInfo.ftCreationTime, MAX_PATH, szBuffer);
	_tprintf(TEXT("Время создания: %s\n"), szBuffer);

	FileTimeToString(fileInfo.ftLastAccessTime, MAX_PATH, szBuffer);
	_tprintf(TEXT("Время последнего просмотра: %s\n"), szBuffer);

	FileTimeToString(fileInfo.ftLastWriteTime, MAX_PATH, szBuffer);
	_tprintf(TEXT("Время последнего изменения: %s\n"), szBuffer);

	printf("Размер файла: %d байт\n", fileInfo.nFileSizeLow);

	CloseHandle(hFile);
}


int FileTimeToString(FILETIME FileTime, DWORD nBufferLength, LPTSTR lpBuffer)
{
	SYSTEMTIME st;
	FileTimeToSystemTime(&FileTime, &st);
	return _stprintf_s(lpBuffer, nBufferLength, TEXT("%04d-%02d-%02d %02d:%02d:%02d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
}

void printErrorMessage(HRESULT hResult)
{
	if (hResult == NULL) {
		hResult = GetLastError();
	}
	LPTSTR errorText = NULL;
	FormatMessage(
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		hResult,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&errorText,
		0,
		NULL);
	if (NULL != errorText) {
		wprintf(L"%s", errorText);
		LocalFree(errorText);
	}
}