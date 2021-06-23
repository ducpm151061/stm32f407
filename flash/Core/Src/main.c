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
//STM32F407VxT6最小系统开发板内部flash示例
//     LED0           --->            PA1
//     KEY0           --->            PE4
//     KEY_UP         --->            PA0
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
#include "delay.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "flash.h"
#include "gui.h"


#define FLASH_SAVE_ADDR  0X0800C004 //设置FLASH 保存地址(必须为偶数，且所在扇区,要大于本代码所占用到的扇区.
																		//否则,写操作的时候,可能会导致擦除整个扇区,从而引起部分程序丢失.引起死机.
const u8 send_test[]={"STM32F407VxT6 FLASH TEST"};   //写入数组
#define SEND_SIZE sizeof(send_test)
#define DATA_SIZE SEND_SIZE/4+((SEND_SIZE%4)?1:0)
u8 rece_buf[DATA_SIZE];

/*****************************************************************************
 * @name       :void FLASH_test(void)
 * @date       :2018-08-09 
 * @function   :show FLASH test
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void FLASH_test(void)
{
	printf("STM32 Internal Flash test\r\n");
	Gui_StrCenter(0,20,RED,BLUE,"STM32F407VxT6最小系统开发板",16,1);//居中显示
	Gui_StrCenter(0,45,RED,BLUE,"Internal Flash 测试程序",16,1);//居中显示	
	Gui_StrCenter(0,70,BLUE,BLUE,"KEY_UP:Write  KEY0:Read",16,1);//居中显示	
	Gui_StrCenter(0,140,BLUE,BLUE,"2020-06-05",16,1);//居中显示
	Gui_StrCenter(0,165,GREEN,BLUE,"http://www.lcdwiki.com",16,1);//居中显示
}

/*****************************************************************************
 * @name       :void Show_FLASH_Write(void)
 * @date       :2018-08-09 
 * @function   :show FLASH write
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Show_FLASH_Write(void)
{
	LCD_Fill(0,115,lcddev.width-1,131,WHITE);
	Show_Str(lcddev.width/2-84,95,RED,WHITE,"Start Write FLASH....",16,0);
	FLASH_Write(FLASH_SAVE_ADDR,(u32*)send_test,DATA_SIZE);	
	Show_Str(lcddev.width/2-84,95,RED,WHITE,"FLASH Write Finished!",16,0);//提示传送完成
}

/*****************************************************************************
 * @name       :void Show_FLASH_Read(void)
 * @date       :2018-08-09 
 * @function   :show FLASH read
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Show_FLASH_Read(void)
{

	Show_Str(lcddev.width/2-84,95,RED,WHITE,"Start Read FLASH.... ",16,0);
	FLASH_Read(FLASH_SAVE_ADDR,(u32*)rece_buf,DATA_SIZE); 
	Show_Str(lcddev.width/2-84,95,RED,WHITE,"The Data Readed Is:   ",16,0);
	Gui_StrCenter(lcddev.width/2-84,115,BLUE,WHITE,rece_buf,16,0);//显示读到的字符串
}

int main(void)
{
	u8 i;
	u8 key_value;	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);             //初始化延时函数
	USART1_Init(115200);         //串口1初始化
	LED_Init();                  //初始化LED
	LCD_Init();	                 //液晶屏初始化
	KEY_Init();                  //按键初始化	
	FLASH_test();                //FLASH测试         
  //循环测试
	while(1)
	{
		key_value=KEY_Scan(0);
		if(key_value==KEY_UP_PRES)  //KEY_UP按下
		{ 
			Show_FLASH_Write();       //写入flash
		}
		else if(key_value==KEY0_PRES) //KEY0按下
		{
			Show_FLASH_Read();		   //读取flash
		}
		if(i%20==0)
		{
			LED0=!LED0;
			i=0;
		}
		i++;
		delay_ms(10);//延时100ms
	}
}

