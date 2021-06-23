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
//STM32F407VxT6最小系统开发板串口示例
//LED0       --->   PA1
//LED1       --->   PC5
//KEY0       --->   PE4
//KEY_UP     --->   PA0
//USART1_TX  --->   PA9
//USART1_RX  --->   PA10
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

//KEY端口读取方式切换
#define KEY_IO_DEF 0   //0-位操作，1-库函数操作，2-寄存器操作

#if KEY_IO_DEF==0    //位操作方式读取按键端口值
#define KEY0_VALUE   PEin(4)  //PE4
#define KEY_UP_VALUE PAin(0)  //PA0
#elif KEY_IO_DEF==1  //库函数方式读取按键端口值
#define KEY0_VALUE 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4) //PE4
#define KEY_UP_VALUE 	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)	//PA0
#else  //寄存器方式读取按键端口值
#define KEY0_VALUE    ((GPIOE->IDR&GPIO_Pin_4)?1:0)
#define KEY_UP_VALUE  ((GPIOA->IDR&GPIO_Pin_0)?1:0)
#endif

#define KEY0_PRES 	  1	//KEY0按下
#define KEY_UP_PRES   2	//KEY_UP按下

void KEY_Init(void);	//IO初始化
u8 KEY_Scan(u8);  		//按键扫描函数	

#endif
