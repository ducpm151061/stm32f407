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
//STM32F407VxT6��Сϵͳ������IICʾ��
//     LED0           --->            PA1
//     KEY0           --->            PE4
//     KEY_UP         --->            PA0
//     IIC_SCL        --->            PB8
//     IIC_SDA        --->            PB9
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
#include "iic.h"
#include "delay.h"

/*****************************************************************************
 * @name       :void IIC_Init(void)
 * @date       :2020-05-08 
 * @function   :Initialize IIC GPIO
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void IIC_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); //ʹ��GPIOBʱ��
	//GPIOB8,B9��ʼ������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	   //��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	   //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	   //����
	GPIO_Init(GPIOB, &GPIO_InitStructure);			   //��ʼ��
	IIC_SCL = 1;									   //����SCL
	IIC_SDA = 1;									   //����SDA
}


void LCD_IIC_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	   //��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	   //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	   //����
	GPIO_Init(GPIOB, &GPIO_InitStructure);			   //��ʼ��
	LCD_IIC_SCL = 1;									   //����SCL
	LCD_IIC_SDA = 1;									   //����SDA
}

/*****************************************************************************
 * @name       :void IIC_Start(void)
 * @date       :2018-08-09 
 * @function   :Generating IIC starting signal
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void IIC_Start(void)
{
	SDA_OUT(); //sda�����
	IIC_SDA = 1;
	IIC_SCL = 1;
	delay_us(4);
	IIC_SDA = 0; //START:when CLK is high,DATA change form high to low
	delay_us(4);
	IIC_SCL = 0; //ǯסI2C���ߣ�׼�����ͻ��������
}


/*****************************************************************************
 * @name       :void IIC_Start(void)
 * @date       :2018-08-09 
 * @function   :Generating IIC starting signal
 * @parameters :None
 * @retvalue   :None
******************************************************************************/


void LCD_IIC_Start(void)
{
	LCD_SDA_OUT(); //sda�����
	LCD_IIC_SDA = 1;
	LCD_IIC_SCL = 1;
	delay_us(4);
	LCD_IIC_SDA = 0; //START:when CLK is high,DATA change form high to low
	delay_us(4);
	LCD_IIC_SCL = 0; //ǯסI2C���ߣ�׼�����ͻ��������
}

/*****************************************************************************
 * @name       :void IIC_Stop(void)
 * @date       :2018-08-09 
 * @function   :Generating IIC stop signal
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void IIC_Stop(void)
{
	SDA_OUT(); //sda�����
	IIC_SCL = 0;
	IIC_SDA = 0; //STOP:when CLK is high DATA change form low to high
	delay_us(4);
	IIC_SCL = 1;
	IIC_SDA = 1; //����I2C���߽����ź�
	delay_us(4);
}



/*****************************************************************************
 * @name       :void IIC_Stop(void)
 * @date       :2018-08-09 
 * @function   :Generating IIC stop signal
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void LCD_IIC_Stop(void)
{
	LCD_SDA_OUT(); //sda�����
	LCD_IIC_SCL = 0;
	LCD_IIC_SDA = 0; //STOP:when CLK is high DATA change form low to high
	delay_us(4);
	LCD_IIC_SCL = 1;
	LCD_IIC_SDA = 1; //����I2C���߽����ź�
	delay_us(4);
}



/*****************************************************************************
 * @name       :u8 IIC_Wait_Ack(void)
 * @date       :2018-08-09 
 * @function   :Wait for the response signal
 * @parameters :None
 * @retvalue   :0-receive response signal successfully
								1-receive response signal unsuccessfully
******************************************************************************/
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime = 0;
	SDA_IN(); //SDA����Ϊ����
	IIC_SDA = 1;
	delay_us(1);
	IIC_SCL = 1;
	delay_us(1);
	while (READ_SDA)
	{
		ucErrTime++;
		if (ucErrTime > 250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL = 0; //ʱ�����0
	return 0;
}



u8 LCD_IIC_Wait_Ack(void)
{
	u8 ucErrTime = 0;
	LCD_SDA_IN(); //SDA����Ϊ����
	LCD_IIC_SDA = 1;
	delay_us(1);
	LCD_IIC_SCL = 1;
	delay_us(1);
	while (LCD_READ_SDA)
	{
		ucErrTime++;
		if (ucErrTime > 250)
		{
			LCD_IIC_Stop();
			return 1;
		}
	}
	LCD_IIC_SCL = 0; //ʱ�����0
	return 0;
}


/*****************************************************************************
 * @name       :void IIC_Ack(void)
 * @date       :2018-08-09 
 * @function   :Generate ACK response signal
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void IIC_Ack(void)
{
	IIC_SCL = 0;
	SDA_OUT();
	IIC_SDA = 0;
	delay_us(2);
	IIC_SCL = 1;
	delay_us(2);
	IIC_SCL = 0;
}


void LCD_IIC_Ack(void)
{
	LCD_IIC_SCL = 0;
	LCD_SDA_OUT();
	LCD_IIC_SDA = 0;
	delay_us(2);
	LCD_IIC_SCL = 1;
	delay_us(2);
	LCD_IIC_SCL = 0;
}

/*****************************************************************************
 * @name       :void IIC_NAck(void)
 * @date       :2018-08-09 
 * @function   :Don't generate ACK response signal
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void IIC_NAck(void)
{
	IIC_SCL = 0;
	SDA_OUT();
	IIC_SDA = 1;
	delay_us(2);
	IIC_SCL = 1;
	delay_us(2);
	IIC_SCL = 0;
}


void LCD_IIC_NAck(void)
{
	LCD_IIC_SCL = 0;
	LCD_SDA_OUT();
	LCD_IIC_SDA = 1;
	delay_us(2);
	LCD_IIC_SCL = 1;
	delay_us(2);
	LCD_IIC_SCL = 0;
}


/*****************************************************************************
 * @name       :void IIC_Send_Byte(u8 txd)
 * @date       :2018-08-09 
 * @function   :send a byte data by IIC bus
 * @parameters :txd:Data to be sent
 * @retvalue   :None
******************************************************************************/
void IIC_Send_Byte(u8 txd)
{
	u8 t;
	SDA_OUT();
	IIC_SCL = 0; //����ʱ�ӿ�ʼ���ݴ���
	for (t = 0; t < 8; t++)
	{
		IIC_SDA = (txd & 0x80) >> 7;
		txd <<= 1;
		delay_us(2); //��TEA5767��������ʱ���Ǳ����
		IIC_SCL = 1;
		delay_us(2);
		IIC_SCL = 0;
		delay_us(2);
	}
}


void LCD_IIC_Send_Byte(u8 txd)
{
	u8 t;
	LCD_SDA_OUT();
	LCD_IIC_SCL = 0; //����ʱ�ӿ�ʼ���ݴ���
	for (t = 0; t < 8; t++)
	{
		LCD_IIC_SDA = (txd & 0x80) >> 7;
		txd <<= 1;
		delay_us(2); //��TEA5767��������ʱ���Ǳ����
		LCD_IIC_SCL = 1;
		delay_us(2);
		LCD_IIC_SCL = 0;
		delay_us(2);
	}
}

/*****************************************************************************
 * @name       :u8 IIC_Read_Byte(unsigned char ack)
 * @date       :2018-08-09 
 * @function   :read a byte data by IIC bus
 * @parameters :ack:0-send nACK
									  1-send ACK
 * @retvalue   :Data to be read
******************************************************************************/
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i, receive = 0;
	SDA_IN(); //SDA����Ϊ����
	for (i = 0; i < 8; i++)
	{
		IIC_SCL = 0;
		delay_us(2);
		IIC_SCL = 1;
		receive <<= 1;
		if (READ_SDA)
		{
			receive++;
		}
		delay_us(1);
	}
	if (!ack)
	{
		IIC_NAck(); //����nACK
	}
	else
	{
		IIC_Ack(); //����ACK
	}
	return receive;
}



u8 LCD_IIC_Read_Byte(unsigned char ack)
{
	unsigned char i, receive = 0;
	LCD_SDA_IN(); //SDA����Ϊ����
	for (i = 0; i < 8; i++)
	{
		LCD_IIC_SCL = 0;
		delay_us(2);
		LCD_IIC_SCL = 1;
		receive <<= 1;
		if (LCD_READ_SDA)
		{
			receive++;
		}
		delay_us(1);
	}
	if (!ack)
	{
		LCD_IIC_NAck(); //����nACK
	}
	else
	{
		LCD_IIC_Ack(); //����ACK
	}
	return receive;
}
