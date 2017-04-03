// task2.cpp: ���������� ����� ����� ��� ����������� ����������.
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
		printf("*** �������� �������� ***\n");
		printf("������� ���������� - 1\n");
		printf("������� ���������� - 2\n");
		printf("������� ������� ���������� - 3\n");
		printf("������ ������ ������� ���������� - 4\n");
		printf("���������� � ����� - 5\n");
		printf("����������� ���� - 6\n");
		printf("������� ����(��� ������ �����)- 7\n");
		printf("����� �� �������� - 0\n");
		printf("�������� ��������: ");
		scanf_s("%d", &operation);
		switch (operation)
		{
		case 0:
			return 0;
		case 1:
			printf("������� �������� ����������: ");
			readline(szBuffer, MAX_PATH);
			createDir(szBuffer);
			break;
		case 2:
			printf("�������� ���������� ��: ");
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
			printf("������� �������� ����������: ");
			readline(szBuffer, MAX_PATH);
			printFileInfo(szBuffer);
			break;
		case 6:
			printf("������� ������������ ��� �����: ");
			readline(szBuffer, MAX_PATH);
			printf("������� ����� ��� �����: ");
			readline(szBuffer2, MAX_PATH);
			copyFile(szBuffer, szBuffer2);
			break;
		case 7:
			printf("������� ��� �����: ");
			readline(szBuffer, MAX_PATH);
			removeFile(szBuffer);
			break;
		default:
			printf("����������� ��������!\n");
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