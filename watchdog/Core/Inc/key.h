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
#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h" 

//KEY�˿ڶ�ȡ��ʽ�л�
#define KEY_IO_DEF 1   //0-λ������1-�⺯��������2-�Ĵ�������

#if KEY_IO_DEF==0    //λ������ʽ��ȡ�����˿�ֵ
#define KEY0_VALUE   PEin(4)  //PE4
#define KEY_UP_VALUE PAin(0)  //PA0
#elif KEY_IO_DEF==1  //�⺯����ʽ��ȡ�����˿�ֵ
#define KEY0_VALUE 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4) //PE4
#define KEY_UP_VALUE 	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)	//PA0
#else  //�Ĵ�����ʽ��ȡ�����˿�ֵ
#define KEY0_VALUE    ((GPIOE->IDR&GPIO_Pin_4)?1:0)
#define KEY_UP_VALUE  ((GPIOA->IDR&GPIO_Pin_0)?1:0)
#endif

void KEY_Init(void);	//IO��ʼ��
#endif
