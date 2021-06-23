//////////////////////////////////////////////////////////////////////////////////
//锟斤拷锟斤拷锟斤拷只锟斤拷学习使锟矫ｏ拷未锟斤拷锟斤拷锟斤拷锟斤拷锟缴ｏ拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷业锟斤拷途
//锟斤拷锟斤拷硬锟斤拷锟斤拷锟斤拷片锟斤拷STM32F407VGT6,STM32F407VxT6锟斤拷小系统锟斤拷锟斤拷锟斤拷,锟斤拷频168MHZ锟斤拷锟斤拷锟斤拷8MHZ
//QDtech-TFT液锟斤拷锟斤拷锟斤拷 for STM32 FSMC
//Chan@ShenZhen QDtech co.,LTD
//锟斤拷司锟斤拷站:www.qdtft.com
//wiki锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷站锟斤拷http://www.lcdwiki.com
//锟斤拷司锟结供锟斤拷锟斤拷支锟街ｏ拷锟轿何硷拷锟斤拷锟斤拷锟解欢迎锟斤拷时锟斤拷锟斤拷学习
//锟教伙拷(锟斤拷锟斤拷) :+86 0755-21077707
//锟街伙拷: (锟斤拷锟斤拷)18823372746 锟斤拷锟斤拷锟斤拷)15989313508
//锟斤拷锟斤拷:(锟斤拷锟斤拷/锟斤拷锟斤拷) sales@qdtft.com  (锟桔猴拷/锟斤拷锟斤拷锟斤拷锟斤拷)service@qdtft.com
//QQ:(锟斤拷前锟斤拷询)3002706772 (锟斤拷锟斤拷支锟斤拷)3002778157
//锟斤拷锟斤拷锟斤拷锟斤拷QQ群:778679828
//锟斤拷锟斤拷锟斤拷锟斤拷:2020/06/29
//锟芥本锟斤拷V1.0
//锟斤拷权锟斤拷锟叫ｏ拷锟斤拷锟斤拷鼐锟斤拷锟�
//Copyright(C) 锟斤拷锟斤拷锟斤拷全锟斤拷锟斤拷锟接硷拷锟斤拷锟斤拷锟睫癸拷司 2018-2028
//All rights reserved
/************************************************************************************
//STM32F407VxT6锟斤拷小系统锟斤拷锟斤拷锟藉触锟斤拷锟斤拷示锟斤拷
//     LED0                 --->            PA1
//     KEY0                 --->            PE4
//     24C02_IIC_SCL        --->            PB8
//     24C02_IIC_SDA        --->            PB9
//支锟斤拷ILI9341/ILI9486/ILI9488/ST7793/ST7796S/R61509/NT35310/NT35510/SSD1963
//TFT LCD锟斤拷锟斤拷直锟接诧拷锟斤拷STM32F407VxT6锟斤拷小系统锟斤拷锟斤拷锟斤拷TFTLCD锟斤拷刍锟斤拷锟酵�锟斤拷锟斤拷锟竭斤拷锟斤拷TFTLCD锟斤拷锟斤拷
//STM32锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷指TFTLCD锟斤拷刍锟斤拷卟锟斤拷锟斤拷锟斤拷锟斤拷诓锟斤拷锟斤拷拥锟絊TM32锟斤拷锟斤拷
//=================================锟斤拷源锟斤拷锟斤拷=======================================//
//     LCD模锟斤拷                    STM32锟斤拷锟斤拷锟斤拷锟斤拷
//      VDD           --->           DC5V/3.3V          //锟斤拷源
//      GND           --->             GND              //锟斤拷源锟斤拷
//=============================液锟斤拷锟斤拷锟斤拷锟斤拷锟竭斤拷锟斤拷===================================//
//     LCD模锟斤拷                    STM32锟斤拷锟斤拷锟斤拷锟斤拷
//      DB0           --->            PD14        -|   
//      DB1           --->            PD15         |  
//      DB2           --->            PD0          | 
//      DB3           --->            PD1          | 
//      DB4           --->            PE7          |
//      DB5           --->            PE8          |
//      DB6           --->            PE9          |
//      DB7           --->            PE10         |===>液锟斤拷锟斤拷16位锟斤拷锟斤拷锟斤拷锟斤拷锟脚猴拷
//      DB8           --->            PE11         |
//      DB9           --->            PE12         |
//      DB10          --->            PE13         |
//      DB11          --->            PE14         |
//      DB12          --->            PE15         |
//      DB13          --->            PD8          |
//      DB14          --->            PD9          |
//      DB15          --->            PD10        -|
//=============================液锟斤拷锟斤拷锟斤拷锟斤拷锟竭斤拷锟斤拷===================================//
//     LCD模锟斤拷 				            STM32锟斤拷锟斤拷锟斤拷锟斤拷 
//      WR            --->            PD5             //液锟斤拷锟斤拷写锟斤拷锟捷匡拷锟斤拷锟脚猴拷
//      RD            --->            PD4             //液锟斤拷锟斤拷锟斤拷锟斤拷锟捷匡拷锟斤拷锟脚猴拷
//      RS            --->            PD11            //液锟斤拷锟斤拷锟斤拷锟斤拷/锟斤拷锟斤拷锟斤拷锟斤拷藕锟�
//      RST           --->          锟斤拷位锟斤拷锟脚ｏ拷默锟较ｏ拷  //液锟斤拷锟斤拷锟斤拷位锟斤拷锟斤拷锟脚号ｏ拷也锟斤拷选锟斤拷PD13锟斤拷
//      CS            --->            PD7             //液锟斤拷锟斤拷片选锟斤拷锟斤拷锟脚猴拷
//      BL            --->            PB15            //液锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷藕锟�
//===============================锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷=====================================//
//	   LCD模锟斤拷                    STM32锟斤拷锟斤拷锟斤拷锟斤拷 
//      PEN           --->            PB1             //锟斤拷锟捷伙拷锟斤拷璐ワ拷锟斤拷锟斤拷锟斤拷锟斤拷卸锟斤拷藕锟�
//      MI            --->            PB2             //锟斤拷锟借触锟斤拷锟斤拷SPI锟斤拷锟竭讹拷锟脚猴拷
//      MO            --->            PC4             //锟斤拷锟借触锟斤拷锟斤拷SPI锟斤拷锟斤拷写锟脚号伙拷锟斤拷荽锟斤拷锟斤拷锟絀IC锟斤拷锟斤拷锟斤拷锟斤拷锟脚猴拷
//      TCS           --->            PC13            //锟斤拷锟借触锟斤拷锟斤拷片选锟斤拷锟斤拷锟脚号伙拷锟斤拷荽锟斤拷锟斤拷锟斤拷锟轿伙拷藕锟�
//      TCK           --->            PB0             //锟斤拷锟借触锟斤拷锟斤拷SPI锟斤拷锟竭伙拷锟斤拷荽锟斤拷锟斤拷锟絀IC锟斤拷锟斤拷时锟斤拷锟脚猴拷
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
#include "sys.h"
#include "usart.h"

//////////////////////////////////////////////////////////////////
//锟斤拷锟斤拷锟斤拷锟铰达拷锟斤拷,支锟斤拷printf锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟�,锟斤拷锟斤拷锟斤拷要选锟斤拷use MicroLIB
#if !USE_MICROLIB
#pragma import(__use_no_semihosting) //锟斤拷锟斤拷__use_no_semihosting锟斤拷锟脚ｏ拷确锟斤拷没锟叫达拷C锟斤拷使锟矫帮拷锟斤拷锟斤拷锟侥猴拷锟斤拷
//锟斤拷准锟斤拷锟斤拷要锟斤拷支锟街猴拷锟斤拷
struct __FILE
{
	int handle;
};

FILE __stdout;
//锟斤拷锟斤拷_sys_exit()锟皆憋拷锟斤拷使锟矫帮拷锟斤拷锟斤拷模式
void _sys_exit(int x)
{
	x = x;
}

#endif

//锟截讹拷锟斤拷fputc锟斤拷锟斤拷
int fputc(int ch, FILE *f)
{
	while (USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET);//循锟斤拷锟斤拷锟斤拷,直锟斤拷锟斤拷锟斤拷锟斤拷锟�
	USART_SendData(USART2, ch);
	return ch;
}

u8 USART_RX_BUF[USART_REC_LEN]; //锟斤拷锟秸伙拷锟斤拷,锟斤拷锟経SART_REC_LEN锟斤拷锟街斤拷.

//锟斤拷锟斤拷状态
//bit15锟斤拷	锟斤拷锟斤拷锟斤拷杀锟街�
//bit14锟斤拷	锟斤拷锟秸碉拷0x0d
//bit13~0锟斤拷	锟斤拷锟秸碉拷锟斤拷锟斤拷效锟街斤拷锟斤拷目
u16 USART_RX_STA = 0; //锟斤拷锟斤拷状态锟斤拷锟�

/*****************************************************************************
 * @name       :void USART1_Init(u32 bound)
 * @date       :2020-05-08 
 * @function   :Initialize USART1
 * @parameters :bound:baudrate
 * @retvalue   :None
******************************************************************************/
void USART1_Init(u32 bound)
{
	//GPIO锟剿匡拷锟斤拷锟斤拷
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);  //使锟斤拷GPIOA时锟斤拷
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); //使锟斤拷USART1时锟斤拷

	//锟斤拷锟斤拷1锟斤拷应锟斤拷锟脚革拷锟斤拷映锟斤拷
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);  //GPIOA9锟斤拷锟斤拷为USART1
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1); //GPIOA10锟斤拷锟斤拷为USART1

	//USART1锟剿匡拷锟斤拷锟斤拷
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9锟斤拷GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;			//锟斤拷锟矫癸拷锟斤拷
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//锟劫讹拷50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;			//锟斤拷锟届复锟斤拷锟斤拷锟�
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;			//锟斤拷锟斤拷
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//锟斤拷始锟斤拷PA9锟斤拷PA10

	//USART1 锟斤拷始锟斤拷锟斤拷锟斤拷
	USART_InitStructure.USART_BaudRate = bound;										//锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//锟街筹拷为8位锟斤拷锟捷革拷式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							//一锟斤拷停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;								//锟斤拷锟斤拷偶校锟斤拷位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //锟斤拷硬锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//锟秸凤拷模式
	USART_Init(USART1, &USART_InitStructure);										//锟斤拷始锟斤拷锟斤拷锟斤拷1

	USART_Cmd(USART1, ENABLE);					   //使锟杰达拷锟斤拷1
	USART_ClearFlag(USART1, USART_FLAG_TC);		   //锟斤拷锟斤拷锟街疚�
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); //锟斤拷锟斤拷锟斤拷锟斤拷卸锟�

	//Usart1 NVIC 锟斤拷锟斤拷
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;		  //锟斤拷锟斤拷1锟叫讹拷通锟斤拷
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //锟斤拷占锟斤拷锟饺硷拷3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		  //锟斤拷锟斤拷锟饺硷拷3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  //IRQ通锟斤拷使锟斤拷
	NVIC_Init(&NVIC_InitStructure);							  //锟斤拷锟斤拷指锟斤拷锟侥诧拷锟斤拷锟斤拷始锟斤拷VIC锟侥达拷锟斤拷锟斤拷
}


void USART2_Init(u32 bound)
{
	//GPIO锟剿匡拷锟斤拷锟斤拷
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);  //使锟斤拷GPIOA时锟斤拷
	RCC_APB2PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); //使锟斤拷USART1时锟斤拷

	//锟斤拷锟斤拷1锟斤拷应锟斤拷锟脚革拷锟斤拷映锟斤拷
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);  //GPIOA9锟斤拷锟斤拷为USART1
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2); //GPIOA10锟斤拷锟斤拷为USART1

	//USART1锟剿匡拷锟斤拷锟斤拷
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; //GPIOA9锟斤拷GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;			//锟斤拷锟矫癸拷锟斤拷
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//锟劫讹拷50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;			//锟斤拷锟届复锟斤拷锟斤拷锟�
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;			//锟斤拷锟斤拷
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//锟斤拷始锟斤拷PA9锟斤拷PA10

	//USART1 锟斤拷始锟斤拷锟斤拷锟斤拷
	USART_InitStructure.USART_BaudRate = bound;										//锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//锟街筹拷为8位锟斤拷锟捷革拷式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							//一锟斤拷停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;								//锟斤拷锟斤拷偶校锟斤拷位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //锟斤拷硬锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//锟秸凤拷模式
	USART_Init(USART2, &USART_InitStructure);										//锟斤拷始锟斤拷锟斤拷锟斤拷1

	USART_Cmd(USART2, ENABLE);					   //使锟杰达拷锟斤拷1
	USART_ClearFlag(USART2, USART_FLAG_TC);		   //锟斤拷锟斤拷锟街疚�
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); //锟斤拷锟斤拷锟斤拷锟斤拷卸锟�

	//Usart1 NVIC 锟斤拷锟斤拷
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;		  //锟斤拷锟斤拷1锟叫讹拷通锟斤拷
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //锟斤拷占锟斤拷锟饺硷拷3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		  //锟斤拷锟斤拷锟饺硷拷3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  //IRQ通锟斤拷使锟斤拷
	NVIC_Init(&NVIC_InitStructure);							  //锟斤拷锟斤拷指锟斤拷锟侥诧拷锟斤拷锟斤拷始锟斤拷VIC锟侥达拷锟斤拷锟斤拷
}



/*****************************************************************************
 * @name       :void USART1_Init(u32 bound)
 * @date       :2020-05-08 
 * @function   :USART1 interrupt service program
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void USART1_IRQHandler(void)
{
	u8 Res;
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) //锟斤拷锟斤拷锟叫讹拷(锟斤拷锟秸碉拷锟斤拷锟斤拷锟捷憋拷锟斤拷锟斤拷0x0d 0x0a锟斤拷尾)
	{
		Res = USART_ReceiveData(USART1);  //锟斤拷取锟斤拷锟秸碉拷锟斤拷锟斤拷锟斤拷
		if ((USART_RX_STA & 0x8000) == 0) //锟斤拷锟斤拷未锟斤拷锟�
		{
			if (USART_RX_STA & 0x4000) //锟斤拷锟秸碉拷锟斤拷0x0d
			{
				if (Res != 0x0a)
				{
					USART_RX_STA = 0; //锟斤拷锟秸达拷锟斤拷,锟斤拷锟铰匡拷始
				}
				else
				{
					USART_RX_STA |= 0x8000; //锟斤拷锟斤拷锟斤拷锟斤拷锟�
				}
			}
			else //锟斤拷没锟秸碉拷0X0D
			{
				if (Res == 0x0d)
				{
					USART_RX_STA |= 0x4000;
				}
				else
				{
					USART_RX_BUF[USART_RX_STA & 0X3FFF] = Res;
					USART_RX_STA++;
					if (USART_RX_STA > (USART_REC_LEN - 1))
					{
						USART_RX_STA = 0; //锟斤拷锟斤拷锟斤拷锟捷达拷锟斤拷,锟斤拷锟铰匡拷始锟斤拷锟斤拷
					}
				}
			}
		}
	}
}
