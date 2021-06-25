#include "delay.h"
#include "sys.h"
#include "led.h"
#include "timer.h"

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);     //��ʼ����ʱ����
	LED_Init();          //��ʼ��LED��GPIO
	//tout=((9999+1)*(8399+1))/84(us)=1s
	TIM3_Int_Init(9999,8399);	//��ʱ��3��ʼ����ʱ��84M����Ƶϵ��8400������84M/8400=10Khz�ļ���Ƶ�ʣ�����10000��Ϊ1s
	while(1)
	{
		LED0=!LED0;       //LED0��ת
		delay_ms(200);	  //200ms��ʱ
	}
}

