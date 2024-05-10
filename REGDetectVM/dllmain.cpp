/* Replace "dll.h" with the name of your header */
#include <windows.h>
#include "dll.h"


DllClass::DllClass()
{

}

DllClass::~DllClass()
{

}


BOOL DllClass::REGDetect() {  
	HKEY hKey;  	  
	char szBuffer[64];    
	unsigned long hSize= sizeof(szBuffer) - 1;    
	if( RegOpenKeyEx( HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\BIOS\\", 0, KEY_READ, &hKey )==ERROR_SUCCESS ) {  
		RegQueryValueEx( hKey, "SystemManufacturer", NULL, NULL, (unsigned char *)szBuffer, &hSize );  
		if( strstr( szBuffer, "VMWARE" )) {       
			RegCloseKey( hKey );  
			return TRUE;  
		}    
		RegCloseKey( hKey );  
	}   
	return FALSE;  
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL,DWORD fdwReason,LPVOID lpvReserved)
{
	switch(fdwReason)
	{
		case DLL_PROCESS_ATTACH:
		{
			break;
		}
		case DLL_PROCESS_DETACH:
		{
			break;
		}
		case DLL_THREAD_ATTACH:
		{
			break;
		}
		case DLL_THREAD_DETACH:
		{
			break;
		}
	}
	
	/* Return TRUE on success, FALSE on failure */
	return TRUE;
}
