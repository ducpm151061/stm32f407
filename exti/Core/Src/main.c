#include "delay.h"
#include "sys.h"
#include "led.h"
#include "key.h"
#include "exti.h"

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);     //��ʼ����ʱ����
	KEY_Init();          //��ʼ������GPIO
	EXTIX_Init();        //��ʼ���ⲿ�ж�����
	LED_Init();          //��ʼ��LED��GPIO
	while(1)
	{
		delay_ms(1000);	  //1s��ʱ
	}
}

