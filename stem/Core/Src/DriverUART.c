/******************************************************************************
 * @file        UART_Hander.c
 * @author
 * @version     V1.0.0
 * @date        10/11/2014
 * @brief
 ******************************************************************************/

/******************************************************************************
                                   INCLUDES
 ******************************************************************************/
#include "DataDefine.h"
#include <stdarg.h>

/******************************************************************************
                                   GLOBAL VARIABLES
 ******************************************************************************/
extern System_t xSystem;

/******************************************************************************
                                   GLOBAL FUNCTIONS
 ******************************************************************************/

/******************************************************************************
                                   DATA TYPE DEFINE
 ******************************************************************************/

/******************************************************************************
                                   PRIVATE VARIABLES
 ******************************************************************************/

/******************************************************************************
                                   LOCAL FUNCTIONS
 ******************************************************************************/

/*****************************************************************************/
/**
 * @brief   :
 * @param   :
 * @retval  :
 * @author  :
 * @created :   10/11/2014
 * @version :
 * @reviewer:
 */
void InitUART(USART_TypeDef *USARTx, uint32_t BaudRate)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

    if (DEBUG_UART == USARTx)
    {
        GPIO_InitStructure.GPIO_Pin = DEBUG_TX_PIN;
        GPIO_Init(DEBUG_UART_GPIO, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin = DEBUG_RX_PIN;
        GPIO_Init(DEBUG_UART_GPIO, &GPIO_InitStructure);

        GPIO_PinAFConfig(DEBUG_UART_GPIO, DEBUG_RX_PIN_SOURCE, DEBUG_UART_IO_AF);
        GPIO_PinAFConfig(DEBUG_UART_GPIO, DEBUG_TX_PIN_SOURCE, DEBUG_UART_IO_AF);
        USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;

        NVIC_InitStructure.NVIC_IRQChannel = DEBUG_UART_IRQ;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure);

        USART_ITConfig(DEBUG_UART, USART_IT_RXNE, ENABLE);
    }

    if (GPS_UART == USARTx)
    {
        GPIO_InitStructure.GPIO_Pin = GPS_TX_PIN;
        GPIO_Init(GPS_UART_GPIO, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin = GPS_RX_PIN;
        GPIO_Init(GPS_UART_GPIO, &GPIO_InitStructure);

        GPIO_PinAFConfig(GPS_UART_GPIO, GPS_RX_PIN_SOURCE, GPS_UART_IO_AF);
        GPIO_PinAFConfig(GPS_UART_GPIO, GPS_TX_PIN_SOURCE, GPS_UART_IO_AF);
        USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;

        NVIC_InitStructure.NVIC_IRQChannel = GPS_UART_IRQ;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure);

        USART_ITConfig(GPS_UART, USART_IT_RXNE, ENABLE);
    }

    USART_InitStructure.USART_BaudRate = BaudRate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USARTx, &USART_InitStructure);
    USART_Cmd(USARTx, ENABLE);
}
/*****************************************************************************/
/**
 * @brief   :
 * @param   :
 * @retval  :
 * @author  :
 * @created :   10/11/2014
 * @version :
 * @reviewer:
 */
void UART_Putc(USART_TypeDef *USARTx, uint8_t c)
{
    uint32_t timeout = 0x5FFFFFF;

    ResetWatchdog();
    while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET && --timeout)
    {
    };
    ResetWatchdog();
    if (timeout == 0)
    {
        return;
    }

    USART_SendData(USARTx, (uint16_t)c);

    timeout = 0x5FFFFFF;
    while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET && --timeout)
    {
    };
    if (timeout == 0)
        return;
}
/*****************************************************************************/
/**
 * @brief   :
 * @param   :
 * @retval  :
 * @author  :
 * @created :   10/11/2014
 * @version :
 * @reviewer:
 */
void UART_Puts(USART_TypeDef *USARTx, const char *str)
{
    while (*str != '\0')
    {
        UART_Putc(USARTx, (uint16_t)(*str));
        str++;
    }
}

/*****************************************************************************/
/**
 * @brief   :
 * @param   :
 * @retval  :
 * @author  :
 * @created :   10/11/2014
 * @version :
 * @reviewer:
 */
void UART_Putb(USART_TypeDef *USARTx, uint8_t *Data, uint16_t Length)
{
    uint16_t i;
    uint32_t timeout = 0x6FFFFFF;

    for (i = 0; i < Length; i++)
    {
        while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET && --timeout)
        {
            ResetWatchdog();
        }
        if (timeout == 0)
        {
            break;
        }
        USART_SendData(USARTx, Data[i]);
    }
}

/*****************************************************************************/
/**
 * @brief   :
 * @param   :
 * @retval  :
 * @author  :
 * @created :   10/11/2014
 * @version :
 * @reviewer:
 */
void UART_itoa(USART_TypeDef *USARTx, long val, int radix, int len)
{
    uint8_t c, r, sgn = 0, pad = ' ';
    uint8_t s[20], i = 0;
    uint32_t v;

    if (radix < 0)
    {
        radix = -radix;
        if (val < 0)
        {
            val = -val;
            sgn = '-';
        }
    }
    v = val;
    r = radix;
    if (len < 0)
    {
        len = -len;
        pad = '0';
    }
    if (len > 20)
        return;
    do
    {
        c = (uint8_t)(v % r);
        if (c >= 10)
            c += 7;
        c += '0';
        s[i++] = c;
        v /= r;
    }
    while (v);
    if (sgn)
        s[i++] = sgn;
    while (i < len)
        s[i++] = pad;
    do
        UART_Putc(USARTx, s[--i]);
    while (i);
}
/*****************************************************************************/
/**
 * @brief   :
 * @param   :
 * @retval  :
 * @author  :
 * @created :   10/11/2014
 * @version :
 * @reviewer:
 */
void UART_Printf(USART_TypeDef *USARTx, const char *str, ...)
{
    va_list arp;
    int d, r, w, s, l, i;

    // Check debug UART & timeout
#if (DEBUG_LEVEL > 0)
    if (USARTx == DEBUG_UART && xSystem.GLStatus.DebugTimeOut == 0 && xSystem.GLStatus.DebugMode == 0)
    {
        return;
    }
#endif

    // Check if only string
    for (i = 0;; i++)
    {
        if (str[i] == '%')
            break;
        if (str[i] == 0)
        {
            UART_Puts(USARTx, str);
            return;
        }
    }

    va_start(arp, str);

    while ((d = *str++) != 0)
    {
        if (d != '%')
        {
            UART_Putc(USARTx, d);
            continue;
        }
        d = *str++;
        w = r = s = l = 0;
        if (d == '0')
        {
            d = *str++;
            s = 1;
        }
        while ((d >= '0') && (d <= '9'))
        {
            w += w * 10 + (d - '0');
            d = *str++;
        }
        if (s)
            w = -w;
        if (d == 'l')
        {
            l = 1;
            d = *str++;
        }
        if (!d)
            break;
        if (d == 's')
        {
            UART_Puts(USARTx, va_arg(arp, char *));
            continue;
        }
        if (d == 'c')
        {
            UART_Putc(USARTx, (char)va_arg(arp, int));
            continue;
        }
        if (d == 'u')
            r = 10;
        if (d == 'd')
            r = -10;
        if (d == 'X' || d == 'x')
            r = 16; // 'x' added by mthomas in increase compatibility
        if (d == 'b')
            r = 2;
        if (!r)
            break;
        if (l)
        {
            UART_itoa(USARTx, (long)va_arg(arp, long), r, w);
        }
        else
        {
            if (r > 0)
                UART_itoa(USARTx, (unsigned long)va_arg(arp, int), r, w);
            else
                UART_itoa(USARTx, (long)va_arg(arp, int), r, w);
        }
    }

    va_end(arp);
}

/*****************************************************************************/
/**
 * @brief   :
 * @param   :
 * @retval  :
 * @author  :
 * @created :   10/11/2014
 * @version :
 * @reviewer:
 */
void DisableAllUART(void)
{
    USART_Cmd(DEBUG_UART, DISABLE);
    USART_Cmd(GPS_UART, DISABLE);
}

/*****************************************************************************/
/**
 * @brief   :
 * @param   :
 * @retval  :
 * @author  :
 * @created :   10/11/2014
 * @version :
 * @reviewer:
 */
void EnableAllUART(void)
{
    USART_Cmd(DEBUG_UART, ENABLE);
    USART_Cmd(GPS_UART, ENABLE);
}

/********************************* END OF FILE *******************************/
