#pragma once

void printCurDir();
void setCurDir(LPCTSTR szPathName);
void printFiles();
void copyFile(LPCTSTR szExistingFileName, LPCTSTR szNewFileName);
void createDir(LPCTSTR szPathName);
void removeFile(LPCTSTR lpPathName);
void printFileInfo(LPCTSTR lpPathName);