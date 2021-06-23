//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ���������ɣ���������������ҵ��;
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
#include "key.h"
#include "delay.h"  

/*****************************************************************************
 * @name       :void KEY_Init(void)
 * @date       :2020-05-08 
 * @function   :Initialize KEY GPIO
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	 
void KEY_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOE, ENABLE);//ʹ��GPIOA,GPIOEʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;          //KEY0 ��Ӧ����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;       //��ͨ����ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //100M
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;      //����
	GPIO_Init(GPIOE, &GPIO_InitStructure);             //��ʼ��PE4
	 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;          //KEY_UP��Ӧ����PA0
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN ;    //����
	GPIO_Init(GPIOA, &GPIO_InitStructure);             //��ʼ��PA0
} 

/*****************************************************************************
 * @name       :u8 KEY_Scan(u8 mode)
 * @date       :2020-05-08 
 * @function   :Scan whether key is pressed or not(Key response priority:KEY0>KEY_UP)
 * @parameters :mode:0-Continuous pressing is not supported
                     1-Support continuous press
 * @retvalue   :0-no key is pressed
                1-KEY0 is pressed
								2-KEY_UP is pressed
******************************************************************************/	
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//�������ɿ���־
	if(mode)key_up=1;  //֧������		  
	if(key_up&&(KEY0_VALUE==0||KEY_UP_VALUE==1))
	{
		delay_ms(10);//ȥ���� 
		key_up=0;
		if(KEY0_VALUE==0)
		{
			return KEY0_PRES;
		}
		else if(KEY_UP_VALUE==1)
		{
			return KEY_UP_PRES;
		}
	}
	else if(KEY0_VALUE==1&&KEY_UP_VALUE==0)
	{
		key_up=1; 	    
	}
 	return 0;// �ް�������
}