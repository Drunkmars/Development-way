// win32 create process3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>

int main(int argc, char* argv[])
{
	STARTUPINFO ie_si = {0};   				
	PROCESS_INFORMATION ie_pi;				
	ie_si.cb = sizeof(ie_si);				
					
	TCHAR szBuffer[256] = "C:\\Documents and Settings\\Administrator\\����\\notepad.exe";				
	CreateProcess(				
		NULL,                  			
		szBuffer,                			
		NULL, 			
		NULL,  			
		FALSE,                   			
		CREATE_SUSPENDED,     			
		NULL,                    			
		NULL,                    			
		&ie_si,                  			
		&ie_pi                  			
		);			
					
	//��ȡ��ǳ������ڵ�(OEP)

	CONTEXT contx;  				
	contx.ContextFlags = CONTEXT_FULL;  				
	GetThreadContext(ie_pi.hThread, &contx);			
	
	printf("%x\n", contx.Eax);

	//��ȡImageBase						
	char* baseAddress = (CHAR *) contx.Ebx+8;						
							
	memset(szBuffer,0,256);						
							
	ReadProcessMemory(ie_pi.hProcess,baseAddress,szBuffer,4,NULL);						



	//�ָ�ִ��				
	ResumeThread(ie_pi.hThread);				

	

	return 0;
}

