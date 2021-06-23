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
//STM32F407VxT6最小系统开发板按键示例
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
#include "delay.h"
#include "sys.h"
#include "led.h"
#include "key.h"

int main(void)
{	
	u8 key_value,key0_flag=0,key_up_flag=0;
	delay_init(168);     //初始化延时函数
	KEY_Init();          //初始化按键GPIO
	LED_Init();          //初始化LED灯GPIO
	while(1)
	{
		key_value=KEY_Scan(0);  //扫描按键
		if(key_value==KEY0_PRES)  //KEY0被按下
		{
			key0_flag=!key0_flag;
			if(key0_flag)
			{
				LED0_CLR;      //LED0亮
			}
			else
			{
				LED0_SET;      //LED0灭
			}
		}
		else if(key_value==KEY_UP_PRES) //KEY_UP被按下
		{
			key_up_flag=!key_up_flag;
			if(key_up_flag)
			{
				LED1_CLR;      //LED0亮
			}
			else
			{
				LED1_SET;      //LED0灭
			}
		}
		else  //无按键按下
		{
			delay_ms(10);
		}
	}
}

