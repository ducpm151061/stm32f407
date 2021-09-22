#ifndef __DRIVER_UART_H__
#define __DRIVER_UART_H__

#include "Hardware.h"

void InitUART(USART_TypeDef *USARTx, uint32_t BaudRate);
void UART_Putc(USART_TypeDef *USARTx, uint8_t c);
void UART_Puts(USART_TypeDef *USARTx, const char *str);
void UART_Putb(USART_TypeDef *USARTx, uint8_t *Data, uint16_t Length);
void UART_Printf(USART_TypeDef *USARTx, const char *str, ...);
void DisableAllUART(void);
void EnableAllUART(void);
void DisableUart(USART_TypeDef *USARTx);
#endif //__DRIVER_UART_H__
