// task1.cpp: ���������� ����� ����� ��� ����������� ����������.
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
		printf("���������� ������� �������� ����, ������� ���...\n");
		printf("������� ���������� �������: ");
		DWORD nOfNotes;
		if (!scanf_s("%u", &nOfNotes)) {
			printf("������ ����� ��������!\n");
			scanf("%*[^\n]");
			return 1;
		}
		hF = createNewFile(FILEPATH, nOfNotes, pH);
		if (!hF || hF == INVALID_HANDLE_VALUE) {
			printf("���������� ������� ����!\n");
			return 1;
		}
	}
	for (;;)
	{
		DWORD dwID;
		printf("*** ������������ ������ � ������� � ����� ***\n");
		printf("������� ������ - 1\n������� ������ - 2\n���������� �� ������ - 3\n����� �� ��������� - 0\n�������� ��������: ");
		scanf("%i", &operation);
		switch (operation)
		{
		case 0: //����� �� ���������
			LocalFree(pH);
			CloseHandle(hF);
			return 0;
		case 1: // �������� ������
			printf("������� ID ������: ");
			scanf("%u", &dwID);
			printf("������� ����� ������: ");
			char szStr[STR_MAX];
			readline(szStr, STR_MAX);
			if (writeNote(hF, pH, dwID, szStr)) {
				printf("������ ������� ���������.\n");
			}
			else {
				printf("���������� �������� ������!\n");
			}
			system("pause");
			break;
		case 2: // �������� ������
			printf("������� ID ������: ");
			DWORD dwId;
			scanf("%u", &dwId);
			if (removeNote(hF, pH, dwId)) {
				printf("������ ������� �������.\n");
			}
			else {
				printf("���������� ������� ������!\n");
			}
			system("pause");
			break;
		case 3: // ����� ���������� �� ������
			printf("������� ID ������: ");
			scanf("%u", &dwId);
			printNoteInfo(hF, pH, dwId);
			system("pause");
			break;
		default: //����������� ��������
			printf("������� ����������� ��������!\n");
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