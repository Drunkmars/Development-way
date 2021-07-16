// deadlock plus.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>

CRITICAL_SECTION g_cs1;
CRITICAL_SECTION g_cs2;
HANDLE hThread[8];

DWORD WINAPI Worker(LPVOID lpParam)
{
	if ((int)lpParam == 7)
	{
		while(1)
		{
			printf("Worker[%x]: ���һ���\n", GetCurrentThreadId());
			Sleep(1000);
		}
	}

	else
	{
		printf("Worker[%x]:׼�������ٽ��g_cs2\n", GetCurrentThreadId());
		EnterCriticalSection(&g_cs2);
		printf("Worker[%x]:�ɹ������ٽ��g_cs2\n", GetCurrentThreadId());

		printf("Worker[%x]:׼�������ٽ��g_cs1\n", GetCurrentThreadId());
		EnterCriticalSection(&g_cs1);
		printf("Worker[%x]:�ɹ������ٽ��g_cs1\n", GetCurrentThreadId());

	}

	return 0;
}



int main(int argc, char* argv[])
{
	InitializeCriticalSection(&g_cs1);
	InitializeCriticalSection(&g_cs2);

	printf("Worker[%x]:׼�������ٽ��g_cs1\n", GetCurrentThreadId());
	EnterCriticalSection(&g_cs1);
	printf("Worker[%x]:�ɹ������ٽ��g_cs1\n", GetCurrentThreadId());

	for(int i=0; i<8; i++)
    {
        hThread[i] = CreateThread(NULL, 0, Worker, (LPVOID)i, 0, NULL);
    }

    printf("main[%x]: ׼�������ٽ�� g_cs2\n", GetCurrentThreadId());
    EnterCriticalSection(&g_cs2);
    printf("main[%x]: �ɹ������ٽ�� g_cs2\n", GetCurrentThreadId());

    return 0;

}

