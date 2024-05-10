.386
.model flat, stdcall
option casemap:none

   include  windows.inc
   include  user32.inc
   include  kernel32.inc
   include  advapi32.inc

   includelib  user32.lib
   includelib  kernel32.lib
   includelib  advapi32.lib

.data
szSubKey      db "software\VMWare, Inc.\VMware tools",0
hKey              dd      ?

.code

DllEntry        proc    _hInstance,_dwReason,_dwReserved
                mov     eax,TRUE
                ret
DllEntry        Endp

REGDetect proc
  invoke RegOpenKeyEx, HKEY_LOCAL_MACHINE, addr szSubKey, 0,\
                 KEY_WRITE or KEY_READ, addr hKey
  .if eax == ERROR_SUCCESS
  invoke RegCloseKey,hKey
  ret TRUE
  .else
  invoke RegCloseKey,hKey
  ret FALSE
  .endif
REGDetect endp

TimeDetect proc
	
	RDTSC
	xchg     ecx, eax
	RDTSC  
	sub    eax, ecx
	cmp    eax, 0FFh
	jg    Detected
  
	ret TRUE
Detected:
	ret FALSE
TimeDetect endp
End     DllEntry
