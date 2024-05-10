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
      mov    ebx, 0  // ��ebx����Ϊ�ǻ�����VMXH��������ֵ
      mov    ecx, 10 // ָ�����ܺţ����ڻ�ȡVMWare�汾������Ϊ0x14ʱ���ڻ�ȡVMware�ڴ��С
      mov    edx, 'VX' // �˿ں�
      in     eax, dx // �Ӷ˿�dx��ȡVMware�汾��eax
//������ָ�����ܺ�Ϊ0x14ʱ����ͨ���ж�eax�е�ֵ�Ƿ����0��������˵�������������
      cmp    ebx, 'VMXh' // �ж�ebx���Ƿ����VMware�汾��VMXh�������������������
      setz   [rc] // ���÷���ֵ

      pop    ebx
      pop    ecx
      pop    edx
    }
  }
  __except(EXCEPTION_EXECUTE_HANDLER)  //���δ����VMware�У��򴥷����쳣
  {
    rc = false;
  }

  return rc;
}


bool IDTDetect() {

  unsigned char m[2+4], rpill[] = "\x0f\x01\x0d\x00\x00\x00\x00\xc3";  //�൱��SIDT[adrr],����addr���ڱ���IDT��ַ
  *((unsigned*)&rpill[3]) = (unsigned)m;  //��sidt[addr]�е�addr��Ϊm�ĵ�ַ
  ((void(*)())&rpill)();  //ִ��SIDTָ�������ȡ��IDT��ַ����������m��
  return (m[5]>0xd0);//��IDT��ַ����0xd0xxxxxxʱ��˵��������VMware��
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



