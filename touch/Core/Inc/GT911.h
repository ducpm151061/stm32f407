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

#ifndef __GT911_DRIVER_H
#define __GT911_DRIVER_H

#include "sys.h"


#define RST_OUT()	{	GPIOC->MODER&=~(3<<(13*2));GPIOC->MODER|=1<<13*2;}		//set RSSET pin to output
#define INT_OUT()	{	GPIOB->MODER&=~(3<<(1*2));GPIOB->MODER|=1<<1*2;}		//set RSSET pin to output
#define INT_IN()	{	GPIOB->MODER&=~(3<<(1*2));GPIOB->MODER|=0<<1*2;}		//set RSSET pin to output

//RST--PC13
//INT--PB1
#define RST_CTRL   		PCout(13)	//GT911 RESET pin out high or low
#define INT_CTRL   		PBout(1) 	//GT911 INT pin out high or low
#define INT_GET   		PBin(1) 	//Get GT911 INT pin status


#define GT9XX_IIC_RADDR 0xBB	//IIC read address
#define GT9XX_IIC_WADDR 0xBA	//IIC write address

#define GT9XX_READ_ADDR 0x814E	//touch point information
#define GT9XX_ID_ADDR 0x8140		//ID of touch IC


void GT911_int_sync(u16 ms);
void GT911_reset_guitar(u8 addr);
void GT911_gpio_init(void);
u8 GT9XX_WriteHandle (u16 addr);
u8 GT9XX_WriteData (u16 addr,u8 value);
u8 GT9XX_ReadData (u16 addr, u8 cnt, u8 *value);
u8 GT911_Init(void);
u8 Touch_Get_Count(void);
u8 GT911_Scan(void);
void GT9xx_send_config(void);
void GT9xx_Eint_Init(void);


#endif
