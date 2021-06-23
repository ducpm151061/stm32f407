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
//STM32F407VxT6��Сϵͳ������USB Slaveʾ��
//     USB_D+         --->            PA12
//     USB_D-         --->            PA11
//     LED0           --->            PA1
//     SD_D0          --->            PC8
//     SD_D1          --->            PC9
//     SD_D2          --->            PC10
//     SD_D3          --->            PC11
//     SD_CLK         --->            PC12
//     SD_CMD         --->            PD2
//     SPI_CLK        --->            PB3
//     SPI_MOSI       --->            PB5
//     SPI_MISO       --->            PB4
//     USART1_TX      --->            PA9
//     USART1_RX      --->            PA10
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
#include <stdio.h>
#include <string.h>
#include "lcd.h"
#include "delay.h"
#include "gui.h"
#include "test.h"
#include "led.h"
#include "usart.h"
#include "w25q64.h"
#include "sdio_sdcard.h"
#include "usbd_msc_core.h"
#include "usbd_usr.h"
#include "usbd_desc.h"
#include "usb_conf.h"

USB_OTG_CORE_HANDLE USB_OTG_dev;
extern vu8 USB_STATUS_REG; //USB״̬
extern vu8 bDeviceState;   //USB���� ���

void Check_SDCard(void)
{
	POINT_COLOR = BLUE;
	BACK_COLOR = WHITE;
	if (SD_Init()) //���SD��
	{
		POINT_COLOR = RED;
		LCD_ShowString(lcddev.width / 2 - 56, 70, 16, "SDCard Failed!", 1);
	}
	else
	{
		LCD_ShowString(lcddev.width / 2 - 76, 70, 16, "SDCard Size:     MB", 1);
		LCD_ShowNum(lcddev.width / 2 + 20, 70, SDCardInfo.CardCapacity >> 20, 5, 16); //��ʾSD������
	}
}

void Check_W25Q64(void)
{
	POINT_COLOR = BLUE;
	BACK_COLOR = WHITE;
	if (W25QXX_ReadID() != W25Q64)
	{
		POINT_COLOR = RED;
		LCD_ShowString(lcddev.width / 2 - 56, 90, 16, "W25Q64 Failed!", 1);
	}
	else //SPI FLASH ����
	{
		LCD_ShowString(lcddev.width / 2 - 76, 90, 16, "SPI FLASH Size:12MB", 1);
	}
}

/*****************************************************************************
 * @name       :void USB_Slave_test(void)
 * @date       :2018-08-09 
 * @function   :show USB Slave test
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void USB_Slave_test(void)
{
	USART1_Send("USB Slave test\r\n");
	printf("USB Slave test\r\n");
	Gui_StrCenter(0, 20, RED, BLUE, "STM32F407VxT6��Сϵͳ������", 16, 1);	 //������ʾ
	Gui_StrCenter(0, 45, RED, BLUE, "USB Slave ���Գ���", 16, 1);		 //������ʾ
	Gui_StrCenter(0, 195, BLUE, BLUE, "2020-06-05", 16, 1);				 //������ʾ
	Gui_StrCenter(0, 215, GREEN, BLUE, "http://www.lcdwiki.com", 16, 1); //������ʾ
}

void USB_Slave_Read_Write(void)
{
	u8 offline_cnt = 0;
	u8 tct = 0;
	u8 USB_STA;
	u8 Divece_STA;
	POINT_COLOR = BLUE;
	BACK_COLOR = WHITE;
	LCD_ShowString(lcddev.width / 2 - 68, 115, 16, "USB Connecting...", 0);
	USBD_Init(&USB_OTG_dev, USB_OTG_FS_CORE_ID, &USR_desc, &USBD_MSC_cb, &USR_cb);
	delay_ms(1800);
	while (1)
	{
		delay_ms(1);
		if (USB_STA != USB_STATUS_REG) //״̬�ı���
		{
			LCD_Fill(0, 135, lcddev.width - 1, 151, WHITE); //�����ʾ
			if (USB_STATUS_REG & 0x01)						//����д
			{
				LED1 = 0;
				LCD_ShowString(lcddev.width / 2 - 56, 135, 16, "USB Writing...", 0); //��ʾUSB����д������
			}
			if (USB_STATUS_REG & 0x02) //���ڶ�
			{
				LED1 = 0;
				LCD_ShowString(lcddev.width / 2 - 56, 135, 16, "USB Reading...", 0); //��ʾUSB���ڶ�������
			}
			if (USB_STATUS_REG & 0x04)
			{
				LCD_ShowString(lcddev.width / 2 - 56, 155, 16, "USB Write Err ", 0); //��ʾд�����
			}
			else
			{
				LCD_Fill(0, 155, lcddev.width - 1, 171, WHITE); //�����ʾ
			}
			if (USB_STATUS_REG & 0x08)
			{
				LCD_ShowString(lcddev.width / 2 - 56, 175, 16, "USB Read Err  ", 0); //��ʾд�����
			}
			else
			{
				LCD_Fill(0, 175, lcddev.width - 1, 191, WHITE); //�����ʾ
			}
			USB_STA = USB_STATUS_REG; //��¼����״̬
		}
		if (Divece_STA != bDeviceState)
		{
			if (bDeviceState == 1)
			{
				LCD_ShowString(lcddev.width / 2 - 68, 115, 16, "USB Connected    ", 0); //��ʾUSB�����Ѿ�����
			}
			else
			{
				LCD_ShowString(lcddev.width / 2 - 68, 115, 16, "USB DisConnected ", 0); //��ʾUSB���γ���
			}
			Divece_STA = bDeviceState;
		}
		tct++;
		if (tct == 200)
		{
			tct = 0;
			LED1 = 1;
			LED0 = !LED0; //��ʾϵͳ������
			if (USB_STATUS_REG & 0x10)
			{
				offline_cnt = 0; //USB������,�����offline������
				bDeviceState = 1;
			}
			else //û�еõ���ѯ
			{
				offline_cnt++;
				if (offline_cnt > 10)
					bDeviceState = 0; //2s��û�յ����߱��,����USB���γ���
			}
			USB_STATUS_REG = 0;
		}
	}
}
