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
#include "led.h" 

/*****************************************************************************
 * @name       :void LED_Init(void)
 * @date       :2020-05-08 
 * @function   :Initialize LED GPIO
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	 
void LED_Init(void)
{    	 
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOC, ENABLE);//使能GPIOA、GPIOC时钟
  //PA1,PC5初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;          //LED0对应IO口
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;      //普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;     //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;       //上拉
  GPIO_Init(GPIOA, &GPIO_InitStructure);             //初始化GPIOA
	GPIO_SetBits(GPIOA,GPIO_Pin_1);                    //PA1设置高，LED0灯灭
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;          //LED1对应IO口
	GPIO_Init(GPIOC, &GPIO_InitStructure);             //初始化GPIOC	
	GPIO_SetBits(GPIOC,GPIO_Pin_5);                    //PC5设置高，LED1灯灭
}
