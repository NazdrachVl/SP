// task2.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "manager.h"

void readline(TCHAR szBuffer[], DWORD nBufferSize);

int main()
{
	setlocale(LC_ALL, "RUS");
	TCHAR szBuffer[MAX_PATH];
	TCHAR szBuffer2[MAX_PATH];
	int operation = 0;
	while (true)
	{
		printf("*** Файловый менеджер ***\n");
		printf("Создать директорию - 1\n");
		printf("Сменить директорию - 2\n");
		printf("Вывести текущую директорию - 3\n");
		printf("Список файлов текущей директории - 4\n");
		printf("Информация о файле - 5\n");
		printf("Скопировать файл - 6\n");
		printf("Удалить файл(или пустую папку)- 7\n");
		printf("Выход из прогаммы - 0\n");
		printf("Выберите операцию: ");
		scanf_s("%d", &operation);
		switch (operation)
		{
		case 0:
			return 0;
		case 1:
			printf("Введите название директории: ");
			readline(szBuffer, MAX_PATH);
			createDir(szBuffer);
			break;
		case 2:
			printf("Изменить директорию на: ");
			readline(szBuffer, MAX_PATH);
			setCurDir(szBuffer);
			break;
		case 3:
			printCurDir();
			break;
		case 4:
			printFiles();
			break;
		case 5:
			printf("Введите название директории: ");
			readline(szBuffer, MAX_PATH);
			printFileInfo(szBuffer);
			break;
		case 6:
			printf("Введите существующее имя файла: ");
			readline(szBuffer, MAX_PATH);
			printf("Введите новое имя файла: ");
			readline(szBuffer2, MAX_PATH);
			copyFile(szBuffer, szBuffer2);
			break;
		case 7:
			printf("Введите имя файла: ");
			readline(szBuffer, MAX_PATH);
			removeFile(szBuffer);
			break;
		default:
			printf("Неизвестная операция!\n");
			break;
		}
		printf("\n");
		system("pause");
		system("cls");
	}

    return 0;
}

void readline(TCHAR szBuffer[], DWORD nBufferSize)
{
	DWORD i = 0, ch;
	while (isspace(ch = getchar()));
	while (ch != '\n')
	{
		if (i < nBufferSize)
			szBuffer[i++] = ch;
		ch = getchar();
	}
	szBuffer[i] = '\0';
}