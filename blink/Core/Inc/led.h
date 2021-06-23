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
//STM32F407VxT6最小系统开发板LED灯示例
//LED0   --->   PA1
//LED1   --->   PC5
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
//LED端口操作方式切换
#define LED_IO_DEF 0   //0-位操作，1-库函数操作，2-寄存器操作

//LED端口定义
#if LED_IO_DEF==0  //使用位操作控制LED灯端口
#define LED0 PAout(1)	// LED0
#define LED1 PCout(5)	// LED1
#define LED0_SET LED0=1  //拉高PA1
#define LED0_CLR LED0=0  //拉低PA1
#define LED1_SET LED1=1  //拉高PC5
#define LED1_CLR LED1=0  //拉低PC5
#elif LED_IO_DEF==1   //使用库函数控制LED灯端口
#define LED0 GPIO_Pin_1    //PA1
#define LED1 GPIO_Pin_5    //PC5
#define LED0_SET GPIO_SetBits(GPIOA,LED0)   //拉高PA1
#define LED0_CLR GPIO_ResetBits(GPIOA,LED0) //拉低PA1
#define LED1_SET GPIO_SetBits(GPIOC,LED1)   //拉高PC5
#define LED1_CLR GPIO_ResetBits(GPIOC,LED1) //拉低PC5
#else                  //使用寄存器控制LED灯端口
#define LED0 GPIO_Pin_1    //PA1
#define LED1 GPIO_Pin_5    //PC5
#define LED0_SET GPIOA->BSRRL=LED0  //拉高PA1
#define LED0_CLR GPIOA->BSRRH=LED0  //拉低PA1
#define LED1_SET GPIOC->BSRRL=LED1  //拉高PC5
#define LED1_CLR GPIOC->BSRRH=LED1  //拉低PC5
#endif
void LED_Init(void);  //初始化GPIO		 				    
#endif
