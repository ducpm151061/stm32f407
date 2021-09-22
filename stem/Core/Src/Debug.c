/******************************************************************************
 * @file        Debug.c
 * @author
 * @version     V1.0.0
 * @date        10/11/2014
 * @brief
 ******************************************************************************/

/******************************************************************************
                                   INCLUDES
 ******************************************************************************/
#include <string.h>
#include <stdlib.h>

#include "Debug.h"
#include "Utilities.h"
#include "GPS.h"
#include "stdlib.h"
#include "DriverUART.h"
#include "AlcoholMeter.h"
#include "Parameters.h"
#include "buffer.h"

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
static LargeBuffer_t DebugBuffer;
static uint32_t DebugTimeout = 0;
static uint8_t tmpDebugBuffer[150];

/******************************************************************************
                                   LOCAL FUNCTIONS
 ******************************************************************************/
static void ProcessNewDebugData(void);
extern AlcoholManage_t AlMManage;
static GUT_BUFFER buffer;

/*****************************************************************************/
/**
 * @brief   :   Tick every 10ms
 * @param   :
 * @retval  :
 * @author  :
 * @created :   10/11/2014
 * @version :
 * @reviewer:
 */
void Debug_Tick(void)
{
    if (DebugTimeout)
    {
        DebugTimeout--;

        if (DebugTimeout == 0)
        {
            ProcessNewDebugData();
            DebugBuffer.BufferIndex = 0;
            memset(DebugBuffer.Buffer, 0, LARGE_BUFFER_SIZE);
        }
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
void InitDebug(void)
{
    InitUART(DEBUG_UART, 115200);
    DebugTimeout = 0;
    buff_alloc(&buffer, 1024);
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
void AddDebugData(uint8_t Data)
{

    DebugBuffer.Buffer[DebugBuffer.BufferIndex++] = Data;

    if (DebugBuffer.BufferIndex >= LARGE_BUFFER_SIZE)
        DebugBuffer.BufferIndex = 0;

    DebugBuffer.Buffer[DebugBuffer.BufferIndex] = 0;

    DebugTimeout = 5 * 10;
    buff_add(&buffer, Data);
}

/*****************************************************************************/
/**
 * @brief   :
 * @param   :
 * @retval  :
 * @author  :
 * @created :   15/01/2014
 * @version :
 * @reviewer:
 */
uint8_t FTP_IP[] = {210, 245, 85, 158};
uint8_t ExecTestCommand(uint8_t TestCommand)
{
    switch (TestCommand)
    {
    case TESTDEBUG:
        if (xSystem.GLStatus.DebugTimeOut == 0)
        {
            xSystem.GLStatus.DebugTimeOut = 600;
            DebugPrint("\rVao che do debug...\r\n");
        }
        xSystem.GLStatus.DebugTimeOut = 600;

        break;

    case 2:
        SystemReset(1);
        break;
    case 3:
        AlMManage.Stage = BLOW_DETECT;
        break;
    case 4:
        AlMManage.Stage = MEASURE_ZERO_POINT;
        break;
    }
    return 0;
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

int server_on_command(int fromSource, GUT_BUFFER *buffer);
void ProcessNewDebugData(void)
{

    uint8_t ret, tmpValue;
    uint8_t command = 0;
    int index = 0;
    char *pParams[10];
    char *input = (char *)buffer.data;
    //int size = strlen(input);
    //char* p = strchr(input, '$');
    //UART_Printf(DEBUG_UART, "PROCESS INPUT SIZE: %d, INDEX:%d\r\n", size, DebugBuffer.BufferIndex);
    //if (p != NULL && (p-input)<256) {
    //    UART_Printf(DEBUG_UART, "%s\r\n",input);
    //} else {
    //    return;
    //}
    //buff_alloc(&buffer, size+1);
    //buff_write(&buffer, input, size);
    DebugPrint("PROCESS INPUT SIZE:%d, CRC16:%04x, %s\r\n", buffer.size,
               xSystem.Parameters.CRC16, input);
    server_on_command(0, &buffer);
    //buff_free(&buffer);
    buff_clear(&buffer);
    DebugPrint("FINISH PROCESS INPUT, REMAIN SIZE:%d, CRC16:%04x\r\n", buffer.size, xSystem.Parameters.CRC16);
    return;

    if (strstr((char *)DebugBuffer.Buffer, "TEST,") != NULL)
    {

        tmpValue = GetNumberFromString(5, (char *)DebugBuffer.Buffer);
        ExecTestCommand(tmpValue);
    }
    if (strstr((char *)pParams[0], "SET,"))
    {
        command = GetNumberFromString(0, pParams[1]);
        tmpValue = GetNumberFromString(0, pParams[2]);
        switch (tmpValue)
        {
        case CFG_DeviceSerial: // 1: Serial cua thiet bi
            break;
        case CFG_Device_ProductDate: //     2   // Ngay San Xuat
        case CFG_Device_CalibDate:   //  3   // Ngay hieu chuan
        case CFG_Device_ResDate:     //      4   // Ngay Du Phong
        case CFG_BlowTime:           //                5   // Do Dai Thoi Gian thoi truoc khi lay mau
        case CFG_BlowPressure:       //            6   // Ap luc de tinh dang thoi hoi
        case CFG_TimeAutoTurnOff:    //         7   // Thoi gian tu dong tat may
        case CFG_TimeoutKey:         //              8   // Thoi gian tu dong tat may
        case CFG_SetupKey:           //                    9   // Key cai dat nang cao
            break;
        default:
            break;
        }
    }
}

void DEBUG_IRQHandler(void)
{

    if (USART_GetITStatus(DEBUG_UART, USART_IT_TXE) == SET)
    {
#if 0
        /* The interrupt was caused by the THR becoming empty.  Are there any
        more characters to transmit? */
        if (xQueueReceiveFromISR(xTXCom1Chars, &cChar, &xHigherPriorityTaskWoken) == pdTRUE)
        {
            /* A character was retrieved from the queue so can be sent to the
            THR now. */
            USART_SendData(COMX1, cChar);
        }
        else
#endif
        {
            //USART_ClearITPendingBit(USART1, USART_IT_TXE);
            USART_ITConfig(DEBUG_UART, USART_IT_TXE, DISABLE);
        }
    }
    if (USART_GetITStatus(DEBUG_UART, USART_IT_RXNE) == SET)
    {

        char data = USART_ReceiveData(DEBUG_UART);
        USART_SendData(DEBUG_UART, data);
        //UART_Putc(DEBUG_UART, data);
        AddDebugData(data);
        //USART_ClearITPendingBit(DEBUG_UART, USART_IT_RXNE);
    }
}

/********************************* END OF FILE *******************************/
