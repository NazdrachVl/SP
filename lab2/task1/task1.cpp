// task1.cpp: определяет точку входа для консольного приложения.
//
/*	TODO:
*/

#include "stdafx.h"
#include "myDB.h"

#define FILEPATH TEXT("myNotes.ftw")

void readline(char str[], int str_len);

int _tmain(int argc, _TCHAR** argv)
{
	setlocale(LC_ALL, "RUS");
	Header* pH = (Header*)LocalAlloc(LMEM_FIXED, sizeof(Header));
	INT operation;
	HANDLE hF = openFile(FILEPATH, pH);
	if (!hF || hF == INVALID_HANDLE_VALUE)
	{
		printf("Невозможно открыть указаный файл, создаем его...\n");
		printf("Введите количество записей: ");
		DWORD nOfNotes;
		if (!scanf_s("%u", &nOfNotes)) {
			printf("Ошибка ввода значения!\n");
			scanf("%*[^\n]");
			return 1;
		}
		hF = createNewFile(FILEPATH, nOfNotes, pH);
		if (!hF || hF == INVALID_HANDLE_VALUE) {
			printf("Невозможно создать файл!\n");
			return 1;
		}
	}
	for (;;)
	{
		DWORD dwID;
		printf("*** Произвольный доступ к записям в файле ***\n");
		printf("Создать запись - 1\nУдалить запись - 2\nИнформация по записи - 3\nВыход из программы - 0\nВыберите операцию: ");
		scanf("%i", &operation);
		switch (operation)
		{
		case 0: //Выход из программы
			LocalFree(pH);
			CloseHandle(hF);
			return 0;
		case 1: // Создание записи
			printf("Введите ID записи: ");
			scanf("%u", &dwID);
			printf("Введите текст записи: ");
			char szStr[STR_MAX];
			readline(szStr, STR_MAX);
			if (writeNote(hF, pH, dwID, szStr)) {
				printf("Запись успешно добавлена.\n");
			}
			else {
				printf("Невозможно добавить запись!\n");
			}
			system("pause");
			break;
		case 2: // Удаление записи
			printf("Введите ID записи: ");
			DWORD dwId;
			scanf("%u", &dwId);
			if (removeNote(hF, pH, dwId)) {
				printf("Запись успешно удалена.\n");
			}
			else {
				printf("Невозможно удалить запись!\n");
			}
			system("pause");
			break;
		case 3: // Вывод информации по записи
			printf("Введите ID записи: ");
			scanf("%u", &dwId);
			printNoteInfo(hF, pH, dwId);
			system("pause");
			break;
		default: //Неизвестная операция
			printf("Выбрана неизвестная операция!\n");
			system("pause");
		}
		system("cls");
	}
	CloseHandle(hF);
	return 0;
}

void readline(char str[], int str_len)
{
	int i = 0, ch;
	while (isspace(ch = getchar()));
	while (ch != '\n')
	{
		if (i < str_len) {
			str[i++] = ch;
		}
		ch = getchar();
	}
	str[i] = '\0';
}