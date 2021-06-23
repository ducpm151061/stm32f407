#include "sys.h"  



//THUMBָ�֧�ֻ������
//�������·���ʵ��ִ�л��ָ��WFI  
void WFI_SET(void)
{
	__ASM volatile("WFI");		  
}
//�ر������ж�(���ǲ�����fault��NMI�ж�)
void INTX_DISABLE(void)
{
	// CPSID   I
	// BX      LR	  
	 __ASM volatile("CPSID I");
	 __ASM volatile("BX LR");
}
//���������ж�
void INTX_ENABLE(void)
{
	// CPSIE   I
	// BX      LR  
	__ASM volatile("CPSIE I");
	__ASM volatile("BX LR");
}
//����ջ����ַ
//addr:ջ����ַ
void MSR_MSP(u32 addr) 
{
	__ASM volatile("MSR MSP, r0"); 
	__ASM volatile("BX r14");
}
















