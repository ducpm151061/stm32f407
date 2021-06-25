#include "delay.h"
#include "sys.h"
#include "lcd.h"
#include "gui.h"
#include "test.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "dma.h"

extern u8 sbuf[SEND_SIZE];

int main(void)
{
	u8 i;
	u8 key_value;	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);             //��ʼ����ʱ����
	USART1_Init(115200);         //����1��ʼ��
	LED_Init();                  //��ʼ��LED
	LCD_Init();	                 //Һ������ʼ��
	KEY_Init();                  //������ʼ��	
	DMA_Config(DMA2_Stream7,DMA_Channel_4,(u32)&USART1->DR,(u32)sbuf,SEND_SIZE);//DMA2,STEAM7,CH4,����Ϊ����1,�洢��Ϊsbuf,����Ϊ:SEND_SIZE.                
	DMA_test();                  //DMA test
	fill_sbuf();                 //��䷢�ͻ�����
  //ѭ������
	while(1)
	{
		key_value=KEY_Scan(0);
		if(key_value==KEY0_PRES)
		{
			Show_DMA_Trans();		
		}
		if(i%20==0)
		{
			LED0=!LED0;
			i=0;
		}
		i++;
		delay_ms(10);//��ʱ100ms
	}
}

