//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ���������������ҵ��;
//����Ӳ������Ƭ��STM32F407VGT6,STM32F407VxT6��Сϵͳ������,��Ƶ168MHZ������8MHZ
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
//STM32F407VxT6��Сϵͳ�����尴��ʾ��
//LED0   --->   PA1
//LED1   --->   PC5
//KEY0   --->   PE4
//KEY_UP --->   PA0
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
#include "delay.h"
#include "sys.h"
#include "led.h"
#include "key.h"

int main(void)
{	
	u8 key_value,key0_flag=0,key_up_flag=0;
	delay_init(168);     //��ʼ����ʱ����
	KEY_Init();          //��ʼ������GPIO
	LED_Init();          //��ʼ��LED��GPIO
	while(1)
	{
		key_value=KEY_Scan(0);  //ɨ�谴��
		if(key_value==KEY0_PRES)  //KEY0������
		{
			key0_flag=!key0_flag;
			if(key0_flag)
			{
				LED0_CLR;      //LED0��
			}
			else
			{
				LED0_SET;      //LED0��
			}
		}
		else if(key_value==KEY_UP_PRES) //KEY_UP������
		{
			key_up_flag=!key_up_flag;
			if(key_up_flag)
			{
				LED1_CLR;      //LED0��
			}
			else
			{
				LED1_SET;      //LED0��
			}
		}
		else  //�ް�������
		{
			delay_ms(10);
		}
	}
}

