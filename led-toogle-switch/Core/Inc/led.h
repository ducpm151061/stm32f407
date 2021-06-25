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
#ifndef __LED_H
#define __LED_H
#include "sys.h"	
//LED�˿ڲ�����ʽ�л�
#define LED_IO_DEF 0   //0-λ������1-�⺯��������2-�Ĵ�������

//LED�˿ڶ���
#if LED_IO_DEF==0  //ʹ��λ��������LED�ƶ˿�
#define LED0 PAout(1)	// LED0
#define LED1 PCout(5)	// LED1
#define LED0_SET LED0=1  //����PA1
#define LED0_CLR LED0=0  //����PA1
#define LED1_SET LED1=1  //����PC5
#define LED1_CLR LED1=0  //����PC5
#elif LED_IO_DEF==1   //ʹ�ÿ⺯������LED�ƶ˿�
#define LED0 GPIO_Pin_1    //PA1
#define LED1 GPIO_Pin_5    //PC5
#define LED0_SET GPIO_SetBits(GPIOA,LED0)   //����PA1
#define LED0_CLR GPIO_ResetBits(GPIOA,LED0) //����PA1
#define LED1_SET GPIO_SetBits(GPIOC,LED1)   //����PC5
#define LED1_CLR GPIO_ResetBits(GPIOC,LED1) //����PC5
#else                  //ʹ�üĴ�������LED�ƶ˿�
#define LED0 GPIO_Pin_1    //PA1
#define LED1 GPIO_Pin_5    //PC5
#define LED0_SET GPIOA->BSRRL=LED0  //����PA1
#define LED0_CLR GPIOA->BSRRH=LED0  //����PA1
#define LED1_SET GPIOC->BSRRL=LED1  //����PC5
#define LED1_CLR GPIOC->BSRRH=LED1  //����PC5
#endif
void LED_Init(void);  //��ʼ��GPIO		 				    
#endif
