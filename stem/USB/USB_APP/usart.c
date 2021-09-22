#include "sys.h"
#include "usart.h"

#if !USE_MICROLIB
#pragma import(__use_no_semihosting)
struct __FILE
{
	int handle;
};

FILE __stdout;

void _sys_exit(int x)
{
	x = x;
}

#endif

int fputc(int ch, FILE *f)
{
	while (USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET)
		;
	USART_SendData(UART4, (u8)ch);
	return ch;
}

u8 USART_RX_BUF[USART_REC_LEN];

u16 USART_RX_STA = 0;

/*****************************************************************************
 * @name       :void UART4_Init(u32 bound)
 * @date       :2020-05-08 
 * @function   :Initialize USART1
 * @parameters :bound:baudrate
 * @retvalue   :None
******************************************************************************/
void UART4_Init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_UART4);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_UART4);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(UART4, &USART_InitStructure);

	USART_Cmd(UART4, ENABLE);
	USART_ClearFlag(UART4, USART_FLAG_TC);
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/*****************************************************************************
 * @name       :void UART4_Init(u32 bound)
 * @date       :2020-05-08 
 * @function   :USART1 interrupt service program
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
/*
void USART4_IRQHandler(void)
{
	u8 Res;
	if (USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
	{
		Res = USART_ReceiveData(UART4);
		if ((USART_RX_STA & 0x8000) == 0)
		{
			if (USART_RX_STA & 0x4000)
			{
				if (Res != 0x0a)
				{
					USART_RX_STA = 0;
				}
				else
				{
					USART_RX_STA |= 0x8000;
				}
			}
			else
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
						USART_RX_STA = 0;
					}
				}
			}
		}
	}
}
*/
