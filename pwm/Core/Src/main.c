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
//STM32F407VxT6最小系统开发板PWM示例
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
#include "delay.h"
#include "sys.h"
#include "led.h"
#include "pwm.h"

int main(void)
{
	u8 dir=1;
	u16 PWM_Value=0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);     //初始化延时函数
	LED_Init();          //初始化LED灯GPIO
	//tout=((499+1)*(83+1))/84(us)=500us,PWM频率为2KHz
	TIM5_PWM_Init(499,83);	//定时器5 PWM初始化，时钟84M，分频系数84，所以84M/84=1Mhz的计数频率，计数500次为500us
	LED1=0;              //点亮LED1
	while(1)
	{
		delay_ms(10);	 
		if(dir)
		{
			PWM_Value++;   //dir==1 led0pwmval递增
		}
		else 
		{
			PWM_Value--;	//dir==0 led0pwmval递减 
 		}
		if(PWM_Value>300)
		{
			dir=0;        //led0pwmval到达300后，方向为递减
		}
		if(PWM_Value==0)
		{
			dir=1;	      //led0pwmval递减到0后，方向改为递增
		}
		TIM_SetCompare2(TIM5,PWM_Value);	//修改比较值，修改占空比
	}
}

