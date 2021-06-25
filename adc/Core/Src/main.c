#include "delay.h"
#include "sys.h"
#include "lcd.h"
#include "gui.h"
#include "test.h"
#include "usart.h"
#include "led.h"
#include "adc.h"

int main(void)
{	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);             //��ʼ����ʱ����
	USART1_Init(115200);         //����1��ʼ��
	LED_Init();                  //��ʼ��LED
	LCD_Init();	                 //Һ������ʼ��
	ADC1_Init(); 				     	   //ADCͨ����ʼ��
	ADC_test();                  //��ʾADC��Ϣ
  //ѭ������
	while(1)
	{
		Show_ADC_Value();           //��ʾ��ȡ����ADCֵ
		LED0=!LED0;
		delay_ms(250);//��ʱ250ms
	}
}

