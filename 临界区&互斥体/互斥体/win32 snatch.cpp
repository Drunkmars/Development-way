// win32 snatch.cpp : Defines the entry point for the application.
//

#include "stdafx.h"

HWND hSum, hEditA, hEditB, hEditC;
HANDLE hThreadArr[4], hMoniter;

HANDLE hMutex;

DWORD WINAPI ThreadProc1(LPVOID lpParameter)					
{					
	::WaitForMultipleObjects(3, hThreadArr, TRUE, -1);	//ʹ��WaitForMultipleObjects�����߳�
	
	for (int i = 0; i < 3 ; i++)
	{
		::CloseHandle(hThreadArr[i]);		//����CloseHandle�رվ��
	}

	MessageBox(0, TEXT("���������!"), TEXT("Snatch"), 0);
					
	return 0;				
}

DWORD WINAPI ThreadProc2(LPVOID lpParameter)					
{			
	TCHAR szBuffer[10];
	DWORD SumNum;
	DWORD Num;

	memset(szBuffer, 0 , sizeof(szBuffer));

	while (true)
	{
		WaitForSingleObject(hMutex, INFINITE);	

		GetWindowText(hSum, szBuffer ,10);		//��ȡSum���ڵ���ֵ
		sscanf(szBuffer, "%d", &SumNum );		//�ַ���ת����	


		if (SumNum < 50)
		{
			   ReleaseMutex(hMutex);		//��С��50���뿪�ٽ���
			   break;
		}

		SumNum -= 50;

		//Sleep(200);

		memset(szBuffer, 0 , sizeof(szBuffer));		//��ջ�����
		sprintf(szBuffer, "%d", SumNum);			//����ת�ַ���
		SetWindowText(hSum, szBuffer);				//д���߳�

		//A B C�����е���ֵ+50
		memset(szBuffer, 0 ,sizeof(szBuffer));		//��ջ�����

		GetWindowText((HWND)lpParameter, szBuffer, 10);		//��ȡ�Ӵ��ڵ���ֵ
		sscanf(szBuffer, "%d", &Num);				//�ַ���ת����

		Num += 50;

		memset(szBuffer, 0 ,sizeof(szBuffer));		//��ջ�����
		sprintf(szBuffer, "%d", Num);				//����ת�ַ���
		SetWindowText((HWND)lpParameter, szBuffer);	//д���߳�

		ReleaseMutex(hMutex);				//�뿪�ٽ���

		Sleep(200);

	}

	OutputDebugStringF("%d", (DWORD)lpParameter);

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
			hSum = GetDlgItem(hDlg,IDC_EDIT1);
			hEditA = GetDlgItem(hDlg,IDC_EDIT2);
			hEditB = GetDlgItem(hDlg,IDC_EDIT3);
			hEditC = GetDlgItem(hDlg,IDC_EDIT4);

			
			SetWindowText(hSum, "1000");
			SetWindowText(hEditA, "0");
			SetWindowText(hEditB, "0");
			SetWindowText(hEditC, "0");
					
			break;		
		}			
	case WM_COMMAND:				
					
		switch (LOWORD (wParam))			
		{			
		case IDC_BUTTON1:			
			{		
				//�����߳�
				hThreadArr[0] = ::CreateThread(NULL, 0, ThreadProc2, hEditA, 0, NULL);
				hThreadArr[1] = ::CreateThread(NULL, 0, ThreadProc2, hEditB, 0, NULL);
				hThreadArr[2] = ::CreateThread(NULL, 0, ThreadProc2, hEditC, 0, NULL);
				hThreadArr[3] = ::CreateThread(NULL, 0, ThreadProc1, NULL, 0, NULL);

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
	hMutex = CreateMutex(NULL, false, TEXT("my mutex"));

	DialogBox(hInstance,MAKEINTRESOURCE(IDD_DIALOG_MAIN),NULL,MainDlgProc);	

	return 0;
}



