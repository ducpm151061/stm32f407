//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它商业用途
//测试硬件：单片机STM32F407VGT6,STM32F407VxT6最小系统开发板,主频168MHZ，晶振8MHZ
//QDtech-TFT液晶驱动 for STM32 FSMC
//Chan@ShenZhen QDtech co.,LTD
//公司网站:www.qdtft.com
//wiki技术资料网站：http://www.lcdwiki.com
//我司提供技术支持，任何技术问题欢迎随时交流学习
//固话(传真) :+86 0755-21077707 
//手机: (销售)18823372746 （技术)15989313508
//邮箱:(销售/订单) sales@qdtft.com  (售后/技术服务)service@qdtft.com
//QQ:(售前咨询)3002706772 (技术支持)3002778157
//技术交流QQ群:778679828
//创建日期:2020/06/29
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 深圳市全动电子技术有限公司 2018-2028
//All rights reserved
/************************************************************************************
//STM32F407VxT6最小系统开发板USB Slave示例
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
//支持ILI9341/ILI9486/ILI9488/ST7793/ST7796S/R61509/NT35310/NT35510/SSD1963
//TFT LCD可以直接插入STM32F407VxT6最小系统开发板TFTLCD插槽或者通过排线接入TFTLCD插座
//STM32连接引脚是指TFTLCD插槽或者插座引脚内部连接的STM32引脚
//=================================电源接线=======================================//
//     LCD模块                    STM32连接引脚
//      VDD           --->           DC5V/3.3V          //电源
//      GND           --->             GND              //电源地
//=============================液晶屏数据线接线===================================//
//     LCD模块                    STM32连接引脚
//      DB0           --->            PD14        -|   
//      DB1           --->            PD15         |  
//      DB2           --->            PD0          | 
//      DB3           --->            PD1          | 
//      DB4           --->            PE7          |
//      DB5           --->            PE8          |
//      DB6           --->            PE9          |
//      DB7           --->            PE10         |===>液晶屏16位并口数据信号
//      DB8           --->            PE11         |
//      DB9           --->            PE12         |
//      DB10          --->            PE13         |
//      DB11          --->            PE14         |
//      DB12          --->            PE15         |
//      DB13          --->            PD8          |
//      DB14          --->            PD9          |
//      DB15          --->            PD10        -|
//=============================液晶屏控制线接线===================================//
//     LCD模块 				            STM32连接引脚 
//      WR            --->            PD5             //液晶屏写数据控制信号
//      RD            --->            PD4             //液晶屏读数据控制信号
//      RS            --->            PD11            //液晶屏数据/命令控制信号
//      RST           --->          复位引脚（默认）  //液晶屏复位控制信号（也可选择PD13）
//      CS            --->            PD7             //液晶屏片选控制信号
//      BL            --->            PB15            //液晶屏背光控制信号
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
extern vu8 USB_STATUS_REG;		//USB状态
extern vu8 bDeviceState;		//USB连接 情况

void Check_SDCard(void)
{
	POINT_COLOR=BLUE;
	BACK_COLOR=WHITE;
	if(SD_Init())			//检测SD卡
	{
		POINT_COLOR=RED;
		LCD_ShowString(lcddev.width/2-56,70,16,"SDCard Failed!",1);		    
	}	
	else
	{
		LCD_ShowString(lcddev.width/2-76,70,16,"SDCard Size:     MB",1);
		LCD_ShowNum(lcddev.width/2+20,70,SDCardInfo.CardCapacity>>20,5,16);	//显示SD卡容量
	}
}

void Check_W25Q64(void)
{
	POINT_COLOR=BLUE;
	BACK_COLOR=WHITE;
	if(W25QXX_ReadID()!=W25Q64)
	{
		POINT_COLOR=RED;
		LCD_ShowString(lcddev.width/2-56,90,16,"W25Q64 Failed!",1);
	}
	else //SPI FLASH 正常
	{   														 
		LCD_ShowString(lcddev.width/2-76,90,16,"SPI FLASH Size:12MB",1);
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
	printf("USB Slave test\r\n");
	Gui_StrCenter(0,20,RED,BLUE,"STM32F407VxT6最小系统开发板",16,1);//居中显示
	Gui_StrCenter(0,45,RED,BLUE,"USB Slave 测试程序",16,1);//居中显示	
	Gui_StrCenter(0,195,BLUE,BLUE,"2020-06-05",16,1);//居中显示
	Gui_StrCenter(0,215,GREEN,BLUE,"http://www.lcdwiki.com",16,1);//居中显示
}

void USB_Slave_Read_Write(void)
{
	u8 offline_cnt=0;
	u8 tct=0;
	u8 USB_STA;
	u8 Divece_STA;
	POINT_COLOR=BLUE;
	BACK_COLOR=WHITE;
	LCD_ShowString(lcddev.width/2-68,115,16,"USB Connecting...",0);	    
	USBD_Init(&USB_OTG_dev,USB_OTG_FS_CORE_ID,&USR_desc,&USBD_MSC_cb,&USR_cb);
	delay_ms(1800);	
	while(1)
	{	
		delay_ms(1);				  
		if(USB_STA!=USB_STATUS_REG)//状态改变了 
		{	 						   
			LCD_Fill(0,135,lcddev.width-1,151,WHITE);//清除显示			  	   
			if(USB_STATUS_REG&0x01)//正在写		  
			{
				LED1=0;
				LCD_ShowString(lcddev.width/2-56,135,16,"USB Writing...",0);//提示USB正在写入数据	
			}
			if(USB_STATUS_REG&0x02)//正在读
			{
				LED1=0;
				LCD_ShowString(lcddev.width/2-56,135,16,"USB Reading...",0);//提示USB正在读出数据 					
			}	 										  
			if(USB_STATUS_REG&0x04)
			{
				LCD_ShowString(lcddev.width/2-56,155,16,"USB Write Err ",0);//提示写入错误
			}
			else 
			{
				LCD_Fill(0,155,lcddev.width-1,171,WHITE);//清除显示	  
			}
			if(USB_STATUS_REG&0x08)
			{
				LCD_ShowString(lcddev.width/2-56,175,16,"USB Read Err  ",0);//提示写入错误
			}
			else 
			{
				LCD_Fill(0,175,lcddev.width-1,191,WHITE);//清除显示    
			}
			USB_STA=USB_STATUS_REG;//记录最后的状态
		}
		if(Divece_STA!=bDeviceState) 
		{
			if(bDeviceState==1)
			{
				LCD_ShowString(lcddev.width/2-68,115,16,"USB Connected    ",0);//提示USB连接已经建立	
			}
			else 
			{
				LCD_ShowString(lcddev.width/2-68,115,16,"USB DisConnected ",0);//提示USB被拔出了	
			}
			Divece_STA=bDeviceState;
		}
		tct++;
		if(tct==200)
		{
			tct=0;
			LED1=1;
			LED0=!LED0;//提示系统在运行
			if(USB_STATUS_REG&0x10)
			{
				offline_cnt=0;//USB连接了,则清除offline计数器
				bDeviceState=1;
			}else//没有得到轮询 
			{
				offline_cnt++;  
				if(offline_cnt>10)bDeviceState=0;//2s内没收到在线标记,代表USB被拔出了
			}
			USB_STATUS_REG=0;
		}
	} 
}


