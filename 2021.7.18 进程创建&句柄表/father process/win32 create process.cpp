// win32 create process.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>				
					
				
int main(int argc, char* argv[])
{
	char szBuffer[256] = {0};
	char szHandle[8] = {0};		


	SECURITY_ATTRIBUTES sa;			
	sa.nLength = sizeof(sa);			
	sa.lpSecurityDescriptor = NULL;			
	sa.bInheritHandle = TRUE; 			

	//����һ�����Ա��̳е��ں˶���						
	HANDLE g_hEvent = CreateEvent(&sa, TRUE, FALSE, NULL);						
							
	//��֯�����в���						
	sprintf(szHandle,"%x",g_hEvent);						
	sprintf(szBuffer,"C:/project/win32_create_process2/Debug/win32_create_process2.exe %s",szHandle);						
							
	//���崴��������Ҫ�õĽṹ��						
	STARTUPINFO si = {0};   						
	PROCESS_INFORMATION pi;						
	si.cb = sizeof(si);	
	
						
	//�����ӽ���						
	BOOL res = CreateProcess(						
		NULL, 					
		szBuffer, 					
		NULL, 					
		NULL, 					
		TRUE, 				
		CREATE_NEW_CONSOLE, 					
		NULL, 					
		NULL, &si, &pi); 	
	
	//�����¼�Ϊ��֪ͨ			
	SetEvent(g_hEvent);			
				
	//�رվ�� �ں˶��󲻻ᱻ����		
	CloseHandle(g_hEvent);  			

	getchar();

	return 0;
}

