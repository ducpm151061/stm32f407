//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ���������������ҵ��;
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
//STM32F407VxT6��Сϵͳ�����崥����ʾ��
//     LED0                 --->            PA1
//     KEY0                 --->            PE4
//     24C02_IIC_SCL        --->            PB8
//     24C02_IIC_SDA        --->            PB9
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
//      WR/CLK            --->            PD5             //Һ����д���ݿ����ź�
//      RD            --->            PD4             //Һ���������ݿ����ź�
//      RS            --->            PD11            //Һ��������/��������ź�
//      RST           --->          ��λ���ţ�Ĭ�ϣ�  //Һ������λ�����źţ�Ҳ��ѡ��PD13��
//      CS            --->            PD7             //Һ����Ƭѡ�����ź�
//      BL/LED            --->            PB15            //Һ������������ź�
//===============================������������=====================================//
//	   LCDģ��                    STM32�������� 
//      PEN           --->            PB1             //���ݻ���败���������ж��ź�
//      MI            --->            PB2             //���败����SPI���߶��ź�
//      MO            --->            PC4             //���败����SPI����д�źŻ���ݴ�����IIC���������ź�
//      TCS           --->            PC13            //���败����Ƭѡ�����źŻ���ݴ�������λ�ź�
//      TCK           --->            PB0             //���败����SPI���߻���ݴ�����IIC����ʱ���ź�
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
    
//��ʼ������������������
_m_tp_dev tp_dev=
{
	GT911_Scan, //Ĭ��ֵΪGT911_Scan
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
	tp_dev.touchtype|=0x80;			//������ 
#else	
	ret=RTP_Init();
	tp_dev.touchtype|=0x00;			//������
#endif
	return ret;
}
