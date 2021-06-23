//QDtech-TFTҺ������ for STM32 FSMC
//Chan@ShenZhen QDtech co.,LTD
//��˾��վ:www.qdtft.com
//wiki����������վ��http://www.lcdwiki.com
//��˾�ṩ����֧�֣��κμ������⻶ӭ��ʱ����ѧϰ
//�̻�(����) :+86 0755-21077707
//�ֻ�: (����)18823372746 ������)15989313508
//����:(����/����) sales@qdtft.com  (�ۺ�/��������)service@qdtft.com
//QQ:(��ǰ��ѯ)3002706772 (����֧��)3002778157
//��������QQȺ:778679828
//��������:2020/06/29
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������ȫ�����Ӽ������޹�˾ 2018-2028
//All rights reserved
/************************************************************************************
//STM32F407VxT6��Сϵͳ�������������ʾ��
//֧��ILI9341/ILI9486/ILI9488/ST7793/ST7796S/R61509/NT35310/NT35510/SSD1963
//TFT LCD����ֱ�Ӳ���STM32F407VxT6��Сϵͳ������TFTLCD��ۻ���ͨ�����߽���TFTLCD����
//     KEY_UP         --->            PA0
//     LED0           --->            PA1
//STM32����������ָTFTLCD��ۻ��߲��������ڲ����ӵ�STM32����
//=================================��Դ����=======================================//
//     LCDģ��                    STM32��������
//      VDD           --->           DC5V/3.3V          //��Դ
//      GND           --->             GND              //��Դ��
//=============================Һ���������߽���===================================//
//     LCDģ��                    STM32��������
//      DB0           --->            PD14        -|   
//      DB1           --->            PD15         |  
//      DB2           --->            PD0          | 
//      DB3           --->            PD1          | 
//      DB4           --->            PE7          |
//      DB5           --->            PE8          |
//      DB6           --->            PE9          |
//      DB7           --->            PE10         |===>Һ����16λ���������ź�
//      DB8           --->            PE11         |
//      DB9           --->            PE12         |
//      DB10          --->            PE13         |
//      DB11          --->            PE14         |
//      DB12          --->            PE15         |
//      DB13          --->            PD8          |
//      DB14          --->            PD9          |
//      DB15          --->            PD10        -|
//=============================Һ���������߽���===================================//
//     LCDģ�� 				            STM32�������� 
//      WR            --->            PD5             //Һ����д���ݿ����ź�
//      RD            --->            PD4             //Һ���������ݿ����ź�
//      RS            --->            PD11            //Һ��������/��������ź�
//      RST           --->          ��λ���ţ�Ĭ�ϣ�  //Һ������λ�����źţ�Ҳ��ѡ��PD13��
//      CS            --->            PD7             //Һ����Ƭѡ�����ź�
//      BL            --->            PB15            //Һ������������ź�
*************************************************************************************/
/* @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, QD electronic SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*************************************************************************************/
#include "wkup.h"
#include "led.h"
#include "delay.h"
#include "usart.h"

/*****************************************************************************
 * @name       :void KEY_Init(void)
 * @date       :2020-05-08 
 * @function   :The system enters standby mode
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Sys_Enter_Standby(void)
{
	//	while(WKUP_KD);			//�ȴ�WK_UP�����ɿ�(����RTC�ж�ʱ,�����WK_UP�ɿ��ٽ������)
	RCC_AHB1PeriphResetCmd(0X04FF, ENABLE);				//��λ����IO��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE); //ʹ��PWRʱ��
	PWR_BackupAccessCmd(ENABLE);						//���������ʹ��

	//�������Ǿ�ֱ�ӹر����RTC�ж�
	RTC_ITConfig(RTC_IT_TS | RTC_IT_WUT | RTC_IT_ALRB | RTC_IT_ALRA, DISABLE); //�ر�RTC����жϡ�
	RTC_ClearITPendingBit(RTC_IT_TS | RTC_IT_WUT | RTC_IT_ALRB | RTC_IT_ALRA); //���RTC����жϱ�־λ��
	PWR_ClearFlag(PWR_FLAG_WU);												   //���Wake-up ��־
	PWR_WakeUpPinCmd(ENABLE);												   //����WKUP���ڻ���
	PWR_EnterSTANDBYMode();													   //�������ģʽ
}

/*****************************************************************************
 * @name       :void KEY_Init(void)
 * @date       :2020-05-08 
 * @function   :Detect the signal of wkup pin
 * @parameters :None
 * @retvalue   :1-Press continuously for more than 3S
								0-Error triggering
******************************************************************************/
u8 Check_WKUP(void)
{
	u8 t = 0;
	u8 tx = 0; //��¼�ɿ��Ĵ���
	LED0 = 0;  //LED0��
	while (1)
	{
		if (WKUP_KD) //�Ѿ�������
		{
			t++;
			tx = 0;
		}
		else
		{
			tx++;
			if (tx > 3) //����90ms��û��WKUP�ź�
			{
				LED0 = 1;
				return 0; //����İ���,���´�������
			}
		}
		delay_ms(30);
		if (t >= 100) //���³���3����
		{
			LED0 = 0; //LED0��
			return 1; //����3s������
		}
	}
}

/*****************************************************************************
 * @name       :void EXTI0_IRQHandler(void)
 * @date       :2020-05-08 
 * @function   :EXTI0 Interrupt service function
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void EXTI0_IRQHandler(void)
{
	EXTI_ClearITPendingBit(EXTI_Line0); // ���LINE10�ϵ��жϱ�־λ
	if (Check_WKUP())					//�ػ�?
	{
		Sys_Enter_Standby(); //�������ģʽ
	}
}

/*****************************************************************************
 * @name       :void WKUP_Init(void)
 * @date       :2020-05-08 
 * @function   :wake up initialization
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void WKUP_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);  //ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE); //ʹ��SYSCFGʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;	 //PA0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; //����ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; //����
	GPIO_Init(GPIOA, &GPIO_InitStructure);		   //��ʼ��

	//(����Ƿ���������)��
	if (Check_WKUP() == 0)
	{
		Sys_Enter_Standby(); //���ǿ���,�������ģʽ
	}
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0); //PA0 ���ӵ��ж���0
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;					  //LINE0
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			  //�ж��¼�
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;		  //�����ش���
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;					  //ʹ��LINE0
	EXTI_Init(&EXTI_InitStructure);								  //����
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;			  //�ⲿ�ж�0
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;  //��ռ���ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;		  //�����ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				  //ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);								  //����NVIC
}
