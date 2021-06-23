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
//STM32F407VxT6��Сϵͳ�����崥����ʾ��
//     LED0                 --->            PA1
//     KEY0                 --->            PE4
//     24C02_IIC_SCL        --->            PB8
//     24C02_IIC_SDA        --->            PB9
//֧��ILI9341/ILI9486/ILI9488/ST7793/ST7796S/R61509/NT35310/NT35510/SSD1963
//TFT LCD����ֱ�Ӳ���STM32F407VxT6��Сϵͳ������TFTLCD��ۻ���ͨ�����߽���TFTLCD����
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
//===============================������������=====================================//
//	   LCDģ��                    STM32�������� 
//      PEN           --->            PB1             //���ݻ���败���������ж��ź�
//      MI            --->            PB2             //���败����SPI���߶��ź�
//      MO            --->            PC4             //���败����SPI����д�źŻ���ݴ�����IIC���������ź�
//      TCS           --->            PC13            //���败����Ƭѡ�����źŻ���ݴ�������λ�ź�
//      TCK           --->            PB0             //���败����SPI���߻���ݴ�����IIC����ʱ���ź�
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
#ifndef __CTPIIC_H
#define __CTPIIC_H
#include "sys.h"	    
  	   		   
//IO��������
#define CTP_SDA_IN()  {GPIOC->MODER&=~(3<<(4*2));GPIOC->MODER|=0<<4*2;}	 //PC4����ģʽ
#define CTP_SDA_OUT() {GPIOC->MODER&=~(3<<(4*2));GPIOC->MODER|=1<<4*2;}  //PC4���ģʽ

//IO��������	 
#define CTP_IIC_SCL    PBout(0) 			//SCL     
#define CTP_IIC_SDA    PCout(4) 			//SDA	 
#define CTP_READ_SDA   PCin(4)  			//����SDA 

//IIC���в�������
void CTP_IIC_Init(void);                	//��ʼ��IIC��IO��				 
void CTP_IIC_Start(void);				//����IIC��ʼ�ź�
void CTP_IIC_Stop(void);	  				//����IICֹͣ�ź�
void CTP_IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 CTP_IIC_Read_Byte(unsigned char ack);	//IIC��ȡһ���ֽ�
u8 CTP_IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void CTP_IIC_Ack(void);					//IIC����ACK�ź�
void CTP_IIC_NAck(void);					//IIC������ACK�ź�

#endif







