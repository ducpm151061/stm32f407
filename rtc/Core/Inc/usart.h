#ifndef __USART_H
#define __USART_H
#include "stdio.h"
#include "stm32f4xx_conf.h"
#include "sys.h"
//�Ƿ�ʹ��΢��
#define USE_MICROLIB 0

#define USART_REC_LEN 256 

extern u8 USART_RX_BUF[USART_REC_LEN];
extern u16 USART_RX_STA;              

void USART1_Init(u32 bound);
void USART1_Send(const char *str);
void USART1_printf(USART_TypeDef *USARTx, char *Data, ...);
#endif
