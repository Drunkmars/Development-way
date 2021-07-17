// win32 event.cpp : Defines the entry point for the application.
//

#include "stdafx.h"

HANDLE g_hEvent;				
				
HWND hEdit1;				
HWND hEdit2;				
HWND hEdit3;				
HWND hEdit4;				
HANDLE hThread1;				
HANDLE hThread2;				
HANDLE hThread3;				
HANDLE hThread4;				
				

DWORD WINAPI ThreadProc2(LPVOID lpParameter)				
{				
	TCHAR szBuffer[10] = {0};			
				
	//���¼������֪ͨʱ 			
	WaitForSingleObject(g_hEvent, INFINITE);			
				
	//��ȡ����			
	GetWindowText(hEdit1,szBuffer,10);			
				
	SetWindowText(hEdit2,szBuffer);	
	
	SetEvent(g_hEvent);
				
	return 0;			
}				
DWORD WINAPI ThreadProc3(LPVOID lpParameter)				
{				
	TCHAR szBuffer[10] = {0};			
				
	//���¼������֪ͨʱ 			
	WaitForSingleObject(g_hEvent, INFINITE);			
				
	//��ȡ����			
	GetWindowText(hEdit1,szBuffer,10);			
				
	SetWindowText(hEdit3,szBuffer);	
	
	SetEvent(g_hEvent);
				
	return 0;			
}				
DWORD WINAPI ThreadProc4(LPVOID lpParameter)				
{				
	TCHAR szBuffer[10] = {0};			
				
	//���¼������֪ͨʱ 			
	WaitForSingleObject(g_hEvent, INFINITE);			
				
	//��ȡ����			
	GetWindowText(hEdit1,szBuffer,10);			
				
	SetWindowText(hEdit4,szBuffer);	
	
	SetEvent(g_hEvent);
				
	return 0;			
}				
		
DWORD WINAPI ThreadMain(LPVOID lpParameter)				
{				
	//�����¼�			
	//Ĭ�ϰ�ȫ����  �ֶ�����δ֪ͨ״̬(TRUE)  ��ʼ״̬δ֪ͨ û������ 			
	g_hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);			
	HANDLE hThread[3];			
	//����3���߳�			
	hThread[0] = ::CreateThread(NULL, 0, ThreadProc2, NULL, 0, NULL);			
	hThread[1] = ::CreateThread(NULL, 0, ThreadProc3, NULL, 0, NULL);			
	hThread[2] = ::CreateThread(NULL, 0, ThreadProc4, NULL, 0, NULL);			
				
	//�����ı����ֵ			
	SetWindowText(hEdit1,"1000");			
				
	//�����¼�Ϊ��֪ͨ			
	SetEvent(g_hEvent);			
				
	//�ȴ��߳̽��� �����ں˶���			
	WaitForMultipleObjects(3, hThread, TRUE, INFINITE);  			
	CloseHandle(hThread[0]);  			
	CloseHandle(hThread[1]);			
	CloseHandle(hThread[2]);			
	CloseHandle(g_hEvent);  			
				
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
			hEdit1 = GetDlgItem(hDlg,IDC_EDIT1);
			hEdit2 = GetDlgItem(hDlg,IDC_EDIT2);
			hEdit3 = GetDlgItem(hDlg,IDC_EDIT3);
			hEdit4 = GetDlgItem(hDlg,IDC_EDIT4);

			
			SetWindowText(hEdit1, "0");
			SetWindowText(hEdit2, "0");
			SetWindowText(hEdit3, "0");
			SetWindowText(hEdit4, "0");
					
			break;		
		}			
	case WM_COMMAND:				
					
		switch (LOWORD (wParam))			
		{			
		case IDC_BUTTON_BEGIN:			
			{		
				::CreateThread(NULL, 0, ThreadMain, NULL, 0, NULL);			

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
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG_MAIN), NULL, MainDlgProc);

	return 0;
}



