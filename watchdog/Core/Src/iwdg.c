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
//STM32F407VxT6最小系统开发板独立看门狗示例
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
#include "iwdg.h"

/*****************************************************************************
 * @name       :void IWDG_Init(u8 prer,u16 rlr)
 * @date       :2020-05-08 
 * @function   :Initialize independent watchdog
 * @parameters :prer:Prer: frequency division:0~7(only the lower 3 bits are valid!)
								rlr:auto reload load value,0~0xfff(The lower 11 bits are valid)
								Time calculation (approximate):tout=((4*2^prer)*rlr)/32(ms)
 * @retvalue   :None
******************************************************************************/	
void IWDG_Init(u8 prer,u16 rlr)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); //使能对IWDG->PR IWDG->RLR的写	
	IWDG_SetPrescaler(prer); //设置IWDG分频系数,分频因子=4*2^prer.但最大值只能是256!
	IWDG_SetReload(rlr);   //设置IWDG装载值
	IWDG_ReloadCounter(); //reload
	IWDG_Enable();       //使能看门狗
}

/*****************************************************************************
 * @name       :void IWDG_Feed(void)
 * @date       :2020-05-08 
 * @function   :Feeding independent watchdog
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	 
void IWDG_Feed(void)
{
	IWDG_ReloadCounter();//reload
}
