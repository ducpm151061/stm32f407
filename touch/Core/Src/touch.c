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
//STM32F407VxT6最小系统开发板触摸屏示例
//     LED0                 --->            PA1
//     KEY0                 --->            PE4
//     24C02_IIC_SCL        --->            PB8
//     24C02_IIC_SDA        --->            PB9
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
//      WR/CLK            --->            PD5             //液晶屏写数据控制信号
//      RD            --->            PD4             //液晶屏读数据控制信号
//      RS            --->            PD11            //液晶屏数据/命令控制信号
//      RST           --->          复位引脚（默认）  //液晶屏复位控制信号（也可选择PD13）
//      CS            --->            PD7             //液晶屏片选控制信号
//      BL/LED            --->            PB15            //液晶屏背光控制信号
//===============================触摸屏触接线=====================================//
//	   LCD模块                    STM32连接引脚 
//      PEN           --->            PB1             //电容或电阻触摸屏触摸中断信号
//      MI            --->            PB2             //电阻触摸屏SPI总线读信号
//      MO            --->            PC4             //电阻触摸屏SPI总线写信号或电容触摸屏IIC总线数据信号
//      TCS           --->            PC13            //电阻触摸屏片选控制信号或电容触摸屏复位信号
//      TCK           --->            PB0             //电阻触摸屏SPI总线或电容触摸屏IIC总线时钟信号
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
#include "touch.h" 
#include "ctpiic.h"
#include "GT911.h"
#include "ft5426.h"
#include "rtp.h"	
    
//初始化触摸屏控制器参数
_m_tp_dev tp_dev=
{
	GT911_Scan, //默认值为GT911_Scan
 	0,
	0,
	0,
	0,
	0,
	0,	  	 		
	0,	  	 		
};					

/*****************************************************************************
 * @name       :u8 TP_Init(void)
 * @date       :2020-06-02 
 * @function   :Initialization resistance or capacity touch screen
 * @parameters :None
 * @retvalue   :0-successfully
                1-failed
******************************************************************************/ 
u16 CTP_Read_ID(void)
{
	u8 id_buf[4]={0};
	CTP_IIC_Init();
	GT911_gpio_init();
	GT911_int_sync(50);
	GT9XX_ReadData(GT9XX_ID_ADDR,4,id_buf);
	if((id_buf[0]==0x39)&&(id_buf[1]==0x31)&&(id_buf[2]==0x31))
	{
		return 0x911;
	}
	FT5426_RD_Reg(FT_ID_G_LIB_VERSION,id_buf,2);
	if((id_buf[0]==0x00)&&(id_buf[1]==0x02))
	{
		return 0x5426;
	}
	return 0xFFFF;
}

/*****************************************************************************
 * @name       :u8 TP_Init(void)
 * @date       :2020-06-02 
 * @function   :Initialization resistance or capacity touch screen
 * @parameters :None
 * @retvalue   :0-successfully
                1-failed
******************************************************************************/ 
u8 TP_Init(void)
{
	u8 ret=0;
#if TP_TYPE
	if(CTP_Read_ID()==0x911)
	{
		ret=GT911_Init();
		tp_dev.CPT_Scan=GT911_Scan;
	}
	else if(CTP_Read_ID()==0x5426)
	{
		ret=FT5426_Init();
		tp_dev.CPT_Scan=FT5426_Scan;
	}
	tp_dev.touchtype|=0x80;			//电容屏 
#else	
	ret=RTP_Init();
	tp_dev.touchtype|=0x00;			//电阻屏
#endif
	return ret;
}
