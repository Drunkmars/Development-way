// win32 synchronization.cpp : Defines the entry point for the application.
//


#include "stdafx.h"

HWND hEdit_RC,hEdit_B1,hEdit_B2,hEdit_C1,hEdit_C2,hEdit_C3,hEdit_C4;	//�ı�����

HANDLE hThread[5];				//�߳�����

CRITICAL_SECTION g_Buffer_CS;	//�����壺������

HANDLE hSemaphoreEmpty;			//�ź������������п���

HANDLE hSemaphoreFull;			//�ź���������������Դ

HWND hEditBuffer[3];			//�������ı�������

HWND hEditCustomer[4];			//�������ı�������

DWORD WINAPI ThreadProduct(LPVOID lpParameter)
{
	TCHAR szBuffer[256];
	TCHAR szTemp[2];
	TCHAR szTemp2[2] = { 0 };
	DWORD dwLength;
	memset(szBuffer, 0, 256);

	GetWindowText(hEdit_RC,szBuffer,256);
	dwLength = strlen(szBuffer);

	if(dwLength == 0)
		return -1;
	for(DWORD i = 0;i< dwLength ; i++)
	{
		//�ȴ���������д��Դ����Ϣ
		WaitForSingleObject(hSemaphoreEmpty, INFINITE);

		//Ѱ�ҿ�д������
		for(DWORD k = 0; k< 2; k++)
		{
			memset(szTemp, 0, 2);
			EnterCriticalSection(&g_Buffer_CS);			//�����ٽ���
			 
			GetWindowText(hEditBuffer[k], szTemp, 2);

			if(!strcmp(szTemp, "0"))
			{
				//TCHAR szT[2] = {0};
				memcpy(szTemp2, &szBuffer[i], 1);
				SetWindowText(hEditBuffer[k], szTemp2);
				LeaveCriticalSection(&g_Buffer_CS);		//�뿪�ٽ���
				
				break;
			}
			
			LeaveCriticalSection(&g_Buffer_CS);			//��szTemp = 0ʱ,�뿪������
		}

		//��������������Դ����Ϣ
		Sleep(1000);
		ReleaseSemaphore(hSemaphoreFull, 1, NULL);
	}

	return 0;
}
DWORD WINAPI ThreadCustomer(LPVOID lpParameter)
{
	TCHAR szTemp[2];
	TCHAR szBuffer[256];
	TCHAR szNewBuffer[256];
	DWORD dwExitCode;
	
	while(1)
	{
		//�ȴ�����������Դ
		dwExitCode = WaitForSingleObject(hSemaphoreFull, 10000);

		if(dwExitCode == 0x102)
			return -1;

		//Ѱ�ҿ�д��Դ�Ļ�����
		EnterCriticalSection(&g_Buffer_CS);		//�����ٽ���

		for(DWORD k = 0; k < 2; k++)
		{
			memset(szTemp, 0, 2);
			GetWindowText(hEditBuffer[k], szTemp, 2);

			if(strcmp(szTemp, "0"))
			{
				//�洢���ı���
				memset(szBuffer, 0, 256);	

				GetWindowText((HWND)lpParameter, szBuffer, 256);
				sprintf(szNewBuffer,"%s->%s", szBuffer, szTemp);
				SetWindowText((HWND)lpParameter, szNewBuffer);

				Sleep(500);
				SetWindowText(hEditBuffer[k], "0");
				break;
			}
		}
		LeaveCriticalSection(&g_Buffer_CS);		//�뿪�ٽ���

		//�����������ѿյ���Ϣ
		Sleep(1000);
		ReleaseSemaphore(hSemaphoreEmpty, 1, NULL);
	}
	return 0;
}

DWORD WINAPI ThreadMain(LPVOID lpParameter)
{
	//�����ź���
	hSemaphoreEmpty = CreateSemaphore(NULL, 2, 2,NULL);		//������Ϊ��ʱ���������߳�����
	hSemaphoreFull = CreateSemaphore(NULL, 0, 2,NULL);		//������Ϊ��ʱ�������߳�����
	
	//����������
	InitializeCriticalSection(&g_Buffer_CS);

	//�����߳�

	hThread[0] = ::CreateThread(NULL, 0, ThreadProduct,NULL, 0, NULL);
	hThread[1] = ::CreateThread(NULL, 0, ThreadCustomer,hEdit_C1, 0, NULL);
	hThread[2] = ::CreateThread(NULL, 0, ThreadCustomer,hEdit_C2, 0, NULL);
	hThread[3] = ::CreateThread(NULL, 0, ThreadCustomer,hEdit_C3, 0, NULL);
	hThread[4] = ::CreateThread(NULL, 0, ThreadCustomer,hEdit_C4, 0, NULL);
	
	//�ȴ��߳̽���

	::WaitForMultipleObjects(5, hThread, TRUE, INFINITE);	//�ȴ������߳̽���	
	::CloseHandle(hSemaphoreEmpty);							//�رն�����
	::CloseHandle(hSemaphoreFull);							//�رն�����
	::DeleteCriticalSection(&g_Buffer_CS);					//ɾ���ٽ���

	return 0;
}
BOOL CALLBACK MainDlgProc(HWND hDlg,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	BOOL bRet = FALSE;
	
	switch(uMsg)
	{
	case WM_CLOSE:
		{
			EndDialog(hDlg,0);
			break;
		}
	case WM_INITDIALOG:
		{
			hEdit_RC = GetDlgItem(hDlg,IDC_EDIT1);
			hEdit_B1 = GetDlgItem(hDlg,IDC_EDIT2);
			hEdit_B2 = GetDlgItem(hDlg,IDC_EDIT3);

			hEditBuffer[0] = hEdit_B1;
			hEditBuffer[1] = hEdit_B2;

			hEdit_C1 = GetDlgItem(hDlg,IDC_EDIT4);
			hEdit_C2 = GetDlgItem(hDlg,IDC_EDIT5);
			hEdit_C3 = GetDlgItem(hDlg,IDC_EDIT6);
			hEdit_C4 = GetDlgItem(hDlg,IDC_EDIT7);

			hEditCustomer[0] = hEdit_C1;
			hEditCustomer[1] = hEdit_C2;
			hEditCustomer[2] = hEdit_C3;
			hEditCustomer[3] = hEdit_C4;

			SetWindowText(hEdit_RC,"0");
			SetWindowText(hEdit_B1,"0");
			SetWindowText(hEdit_B2,"0");
			SetWindowText(hEdit_C1,"");
			SetWindowText(hEdit_C2,"");
			SetWindowText(hEdit_C3,"");
			SetWindowText(hEdit_C4,"");
			
			break;
		}
	case WM_COMMAND:
		
		switch (LOWORD (wParam))
		{
		case IDC_BUTTON_BEGIN:
			{

				CreateThread(NULL, 0, ThreadMain,NULL, 0, NULL);
				
				return TRUE;
			}
		}
		
		break ;
	}
	
	return bRet;
}
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.

	DialogBox(hInstance,MAKEINTRESOURCE(IDD_DIALOG_MAIN),NULL,MainDlgProc);

	return 0;
}

