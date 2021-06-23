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
//STM32F407VxT6��Сϵͳ������������Ź�ʾ��
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
#include "iwdg.h"

/*****************************************************************************
 * @name       :void IWDG_Init(u8 prer,u16 rlr)
 * @date       :2020-05-08 
 * @function   :Initialize independent watchdog
 * @parameters :prer:Prer: frequency division:0~7(only the lower 3 bits are valid!)
								rlr:auto reload load value,0~0xfff(The lower 11 bits are valid)
								Time calculation (approximate):tout=((4*2^prer)*rlr)/32(ms)
 * @retvalue   :None
******************************************************************************/	
void IWDG_Init(u8 prer,u16 rlr)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); //ʹ�ܶ�IWDG->PR IWDG->RLR��д	
	IWDG_SetPrescaler(prer); //����IWDG��Ƶϵ��,��Ƶ����=4*2^prer.�����ֵֻ����256!
	IWDG_SetReload(rlr);   //����IWDGװ��ֵ
	IWDG_ReloadCounter(); //reload
	IWDG_Enable();       //ʹ�ܿ��Ź�
}

/*****************************************************************************
 * @name       :void IWDG_Feed(void)
 * @date       :2020-05-08 
 * @function   :Feeding independent watchdog
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	 
void IWDG_Feed(void)
{
	IWDG_ReloadCounter();//reload
}
