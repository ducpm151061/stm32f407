//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ���������ɣ���������������ҵ��;
//����Ӳ������Ƭ��STM32F407VGT6,STM32F407VxT6��Сϵͳ������,��Ƶ168MHZ������8MHZ
//QDtech-TFTҺ������ for STM32 FSMC
//Chan@ShenZhen QDtech co.,LTD
//��˾��վ:www.qdtft.com
//wiki����������վ��http://www.lcdwiki.com
//��˾�ṩ����֧�֣��κμ������⻶ӭ��ʱ����ѧϰ
//�̻�(����) :+86 0755-21077707 
//�ֻ�: (����)18823372746 ������)15989313508
//����:(����/����) sales@qdtft.com  (�ۺ�/��������)service@qdtft.com
//QQ:(��ǰ��ѯ)3002706772 (����֧��)3002778157
//��������QQȺ:778679828
//��������:2020/06/29
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������ȫ�����Ӽ������޹�˾ 2018-2028
//All rights reserved
/************************************************************************************
//STM32F407VxT6��Сϵͳ�������ڲ�flashʾ��
//     LED0           --->            PA1
//     KEY0           --->            PE4
//     KEY_UP         --->            PA0
//֧��ILI9341/ILI9486/ILI9488/ST7793/ST7796S/R61509/NT35310/NT35510/SSD1963
//TFT LCD����ֱ�Ӳ���STM32F407VxT6��Сϵͳ������TFTLCD��ۻ���ͨ�����߽���TFTLCD����
//STM32����������ָTFTLCD��ۻ��߲��������ڲ����ӵ�STM32����
//=================================��Դ����=======================================//
//     LCDģ��                    STM32��������
//      VDD           --->           DC5V/3.3V          //��Դ
//      GND           --->             GND              //��Դ��
//=============================Һ���������߽���===================================//
//     LCDģ��                    STM32��������
//      DB0           --->            PD14        -|   
//      DB1           --->            PD15         |  
//      DB2           --->            PD0          | 
//      DB3           --->            PD1          | 
//      DB4           --->            PE7          |
//      DB5           --->            PE8          |
//      DB6           --->            PE9          |
//      DB7           --->            PE10         |===>Һ����16λ���������ź�
//      DB8           --->            PE11         |
//      DB9           --->            PE12         |
//      DB10          --->            PE13         |
//      DB11          --->            PE14         |
//      DB12          --->            PE15         |
//      DB13          --->            PD8          |
//      DB14          --->            PD9          |
//      DB15          --->            PD10        -|
//=============================Һ���������߽���===================================//
//     LCDģ�� 				            STM32�������� 
//      WR            --->            PD5             //Һ����д���ݿ����ź�
//      RD            --->            PD4             //Һ���������ݿ����ź�
//      RS            --->            PD11            //Һ��������/��������ź�
//      RST           --->          ��λ���ţ�Ĭ�ϣ�  //Һ������λ�����źţ�Ҳ��ѡ��PD13��
//      CS            --->            PD7             //Һ����Ƭѡ�����ź�
//      BL            --->            PB15            //Һ������������ź�
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


#define FLASH_SAVE_ADDR  0X0800C004 //����FLASH �����ַ(����Ϊż��������������,Ҫ���ڱ�������ռ�õ�������.
																		//����,д������ʱ��,���ܻᵼ�²�����������,�Ӷ����𲿷ֳ���ʧ.��������.
const u8 send_test[]={"STM32F407VxT6 FLASH TEST"};   //д������
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
	USART1_Send("STM32 Internal Flash test\r\n");
	Gui_StrCenter(0,20,RED,BLUE,"STM32F407VxT6��Сϵͳ������",16,1);//������ʾ
	Gui_StrCenter(0,45,RED,BLUE,"Internal Flash ���Գ���",16,1);//������ʾ	
	Gui_StrCenter(0,70,BLUE,BLUE,"KEY_UP:Write  KEY0:Read",16,1);//������ʾ	
	Gui_StrCenter(0,140,BLUE,BLUE,"2020-06-05",16,1);//������ʾ
	Gui_StrCenter(0,165,GREEN,BLUE,"http://www.lcdwiki.com",16,1);//������ʾ
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
	Show_Str(lcddev.width/2-84,95,RED,WHITE,"FLASH Write Finished!",16,0);//��ʾ�������
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
	Gui_StrCenter(lcddev.width/2-84,115,BLUE,WHITE,rece_buf,16,0);//��ʾ�������ַ���
}

int main(void)
{
	u8 i;
	u8 key_value;	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);             //��ʼ����ʱ����
	USART1_Init(115200);         //����1��ʼ��
	LED_Init();                  //��ʼ��LED
	LCD_Init();	                 //Һ������ʼ��
	KEY_Init();                  //������ʼ��	
	FLASH_test();                //FLASH����         
  //ѭ������
	while(1)
	{
		key_value=KEY_Scan(0);
		if(key_value==KEY_UP_PRES)  //KEY_UP����
		{ 
			Show_FLASH_Write();       //д��flash
		}
		else if(key_value==KEY0_PRES) //KEY0����
		{
			Show_FLASH_Read();		   //��ȡflash
		}
		if(i%20==0)
		{
			LED0=!LED0;
			i=0;
		}
		i++;
		delay_ms(10);//��ʱ100ms
	}
}

