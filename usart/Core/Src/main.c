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
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "stdio.h"

int main(void)
{
	u8 count;
	u16 rlen = 0, times = 0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置系统中断优先级分组2
	delay_init(168);								//初始化延时函数
	USART1_Init(115200);							//初始化串口1
	KEY_Init();										//初始化按键GPIO
	LED_Init();										//初始化LED灯GPIO
	printf("hello");
	while (1)
	{
		char send[50];
		sprintf(send, "\rcount: %d\n", count);
		USART1_Send(send);
		printf("%d", count);
		printf("hello");
		count++;
		if (USART_RX_STA & 0x8000)
		{
			rlen = USART_RX_STA & 0x3fff; //得到此次接收到的数据长度
			USART1_Send("\r\nhello Phung Minh Duc");
			// USART1_Send("%.*s\r\n\r\n", rlen, USART_RX_BUF); //发送输入的内容
			USART_RX_STA = 0;
		}
		else
		{
			times++;
			if (times % 5000 == 0)
			{
				USART1_Send("\r\nSTM32F407VxT6 time5\r\n");
			}
			if (times % 200 == 0)
				USART1_Send("STM32F407VxT6 time2\r\n");
			if (times % 30 == 0)
				LED0 = !LED0; //闪烁LED,提示系统正在运行.
			delay_ms(10);
		}
		if (KEY_Scan(0) == KEY0_VALUE) //按下KEY0按键
		{
			delay_ms(200); //放在连续输出
			USART1_Send("\r\nKEY0da duoc an\r\n");
		}
	}
}
