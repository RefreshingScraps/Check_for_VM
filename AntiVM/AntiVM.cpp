// e.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include <stdio.h>
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    
}

bool INCommand()
{
  bool rc = true;

  __try
  {
    __asm
    {
      push   edx
      push   ecx
      push   ebx

      mov    eax, 'VMXh'
      mov    ebx, 0  // 将ebx设置为非幻数’VMXH’的其它值
      mov    ecx, 10 // 指定功能号，用于获取VMWare版本，当它为0x14时用于获取VMware内存大小
      mov    edx, 'VX' // 端口号
      in     eax, dx // 从端口dx读取VMware版本到eax
//若上面指定功能号为0x14时，可通过判断eax中的值是否大于0，若是则说明处于虚拟机中
      cmp    ebx, 'VMXh' // 判断ebx中是否包含VMware版本’VMXh’，若是则在虚拟机中
      setz   [rc] // 设置返回值

      pop    ebx
      pop    ecx
      pop    edx
    }
  }
  __except(EXCEPTION_EXECUTE_HANDLER)  //如果未处于VMware中，则触发此异常
  {
    rc = false;
  }

  return rc;
}


bool IDTDetect() {

  unsigned char m[2+4], rpill[] = "\x0f\x01\x0d\x00\x00\x00\x00\xc3";  //相当于SIDT[adrr],其中addr用于保存IDT地址
  *((unsigned*)&rpill[3]) = (unsigned)m;  //将sidt[addr]中的addr设为m的地址
  ((void(*)())&rpill)();  //执行SIDT指令，并将读取后IDT地址保存在数组m中
  return (m[5]>0xd0);//当IDT基址大于0xd0xxxxxx时则说明程序处于VMware中
}


bool LDTDetect(void)
{
    unsigned short ldt_addr = 0;
    unsigned char ldtr[2];

    _asm sldt ldtr
    ldt_addr = *((unsigned short *)&ldtr);

    return (ldt_addr == 0x0000);
}

bool GDTDetect(void)
{
    unsigned int gdt_addr = 0;
    unsigned char gdtr[4];

    _asm sgdt gdtr
    gdt_addr = *((unsigned int *)&gdtr[2]);
    printf("GDT BaseAddr:0x%x\n", gdt_addr);

    return ((gdt_addr >> 24) == 0xff);
}

bool STRDetect(void)
{
    unsigned char mem[4] = {0};
    int i;

    __asm str mem;

    return ( (mem[0]==0x00) && (mem[1]==0x40));

}



