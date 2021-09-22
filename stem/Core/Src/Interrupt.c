/******************************************************************************
 * @file        SourceFileTemplate.c
 * @author      Khanhpd
 * @version     V1.0.0
 * @date        10/11/2014
 * @brief
 ******************************************************************************/

/******************************************************************************
                                   INCLUDES
 ******************************************************************************/
#include "Interrupt.h"
#include "DataDefine.h"
#include "Debug.h"
#include "GPS.h"
#include "Measurement.h"
#include "Camera.h"
#include "NhienLieu.h"
#include "Sound.h"
#include "ManHinh.h"
#include "RTC.h"
#if (__TAXI_FUNC__ == 1)
#include "TaxiMeter.h"
#include "Tablet.h"
#include "POS.h"
#endif

/******************************************************************************
                                   GLOBAL VARIABLES
 ******************************************************************************/
extern System_t xSystem;

/******************************************************************************
                                   GLOBAL FUNCTIONS
 ******************************************************************************/
extern void SystemTick(void);
extern void disk_timerproc(void);

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
 * @author  :   Khanhpd
 * @created :   10/11/2014
 * @version :
 * @reviewer:
 */
void SysTick_Handler(void)
{
    SystemTick();
}
/*****************************************************************************/
/**
 * @brief   :
 * @param   :
 * @retval  :
 * @author  :   Khanhpd
 * @created :   10/11/2014
 * @version :
 * @reviewer:
 */
void SVC_Handler(void)
{
    DebugPrint("\rSVC Handler");
}
/*****************************************************************************/
/**
 * @brief   :
 * @param   :
 * @retval  :
 * @author  :   Khanhpd
 * @created :   10/11/2014
 * @version :
 * @reviewer:
 */
void DebugMon_Handler(void)
{
    DebugPrint("\rDebugMon Handler");
}
/*****************************************************************************/
/**
 * @brief   :
 * @param   :
 * @retval  :
 * @author  :   Khanhpd
 * @created :   10/11/2014
 * @version :
 * @reviewer:
 */
void PendSV_Handler(void)
{
}
/*****************************************************************************/
/**
 * @brief   :
 * @param   :
 * @retval  :
 * @author  :   Khanhpd
 * @created :   10/11/2014
 * @version :
 * @reviewer:
 */
void HardFault_Handler(unsigned int *hardfault_args)
{
    DebugPrint("HFE.");
    RTC_WriteBackupRegister(HARDFAULT_RST, 100);
    RTC_WriteBackupRegister(HARDFAULT_REASON, SCB->CFSR);
    ProcessSendBanTinDebug(DEBUG_INFO, "\rHard Fault exception.");

    /* Go to infinite loop when Hard Fault exception occurs */

    while (1)
        ;
}
/*****************************************************************************/
/**
 * @brief   :
 * @param   :
 * @retval  :
 * @author  :   Khanhpd
 * @created :   10/11/2014
 * @version :
 * @reviewer:
 */
void BusFault_Handler(void)
{
    DebugPrint("\rBus Fault exception.");

    /* Go to infinite loop when Hard Fault exception occurs */
    SystemReset(101);
    while (1)
        ;
}
/*****************************************************************************/
/**
 * @brief   :
 * @param   :
 * @retval  :
 * @author  :   Khanhpd
 * @created :   10/11/2014
 * @version :
 * @reviewer:
 */
void UsageFault_Handler(void)
{
    DebugPrint("\rUsage Fault exception.");

    /* Go to infinite loop when Hard Fault exception occurs */
    while (1)
        ;
}

/*******************************************************************************
 * Function Name    : USART3_IRQHandler
 * Return           : None
 * Parameters       : None
 * Created by       : Khanhpd
 * Date created     : 10/11/2014
 * Description      : This function handles USART3 global interrupt request.
 * Notes            :
 *******************************************************************************/
void DEBUG_IRQHandler(void)
{
    volatile unsigned int IIR;

    if (USART_GetITStatus(DEBUG_UART, USART_IT_RXNE) != RESET)
    {
        AddDebugData(USART_ReceiveData(DEBUG_UART));
        USART_ClearITPendingBit(DEBUG_UART, USART_IT_RXNE);
    }
}

/*******************************************************************************
 * Function Name    : USART1_IRQHandler
 * Return           : None
 * Parameters       : None
 * Created by       : Khanhpd
 * Date created     : 10/11/2014
 * Description      : This function handles UART4 global interrupt request.
 * Notes            :
 *******************************************************************************/
void GPS_IRQHandler(void)
{
    uint8_t Value = 0;

    if (USART_GetITStatus(GPS_UART, USART_IT_RXNE) != RESET)
    {
        Value = USART_ReceiveData(GPS_UART) & 0xFF;

        GetDataFromGPSModule(Value);

        USART_ClearITPendingBit(GPS_UART, USART_IT_RXNE);
    }
}

/*******************************************************************************
 * Function Name    : USART2_IRQHandler
 * Return           : None
 * Parameters       : None
 * Created by       : Khanhpd
 * Date created     : 10/11/2014
 * Description      : This function handles UART4 global interrupt request.
 * Notes            :
 *******************************************************************************/
uint16_t Value = 0;
void CAM_IRQHandler(void)
{
    volatile unsigned int IIR;

    if (USART_GetITStatus(CAM_UART, USART_IT_RXNE) != RESET)
    {

        //IIR = USART2->SR;
        Value = USART_ReceiveData(CAM_UART) & 0xFF;

#ifdef __BUS_HN__
        LNX_ReceiveData(Value);
#endif

#if (__GPS_FUNC__ == 1)
        if (xSystem.Parameters.CheDoChup > 0)
            CameraReceiveData(Value);
        else if (xSystem.Parameters.CheDoDoNhienLieu > 0)
        {

            xSystem.NhienLieu->AddNewData(Value);
        }
#endif

#if (__TAXI_FUNC__ == 1)
        if (xSystem.Parameters.CheDoChup > 0)
            CameraReceiveData(Value);

        TABLET_ReceiveData(Value);
        POS_ReceiveData(Value);
#endif

        USART_ClearITPendingBit(CAM_UART, USART_IT_RXNE);
    }
}

/*******************************************************************************
 * Function Name    : UART5_IRQHandler
 * Return           : None
 * Parameters       : None
 * Created by       : Khanhpd
 * Date created     : 10/11/2014
 * Description      : This function handles UART4 global interrupt request.
 * Notes            :
 *******************************************************************************/
void MULTIFUNCTION_IRQHandler(void)
{
    uint8_t Value = 0;

    if (USART_GetITStatus(MULTIFUNCTION_UART, USART_IT_RXNE) != RESET)
    {
        Value = USART_ReceiveData(MULTIFUNCTION_UART) & 0xFF;

#if (__TAXI_FUNC__ == 1)
        if (xSystem.Parameters.LoaiDongHo != 0)
            Taxi_ReceiveData(Value);
#endif

#if (__GPS_FUNC__ == 1)
        if (xSystem.Parameters.CheDoDoNhienLieu == DO_NHIEN_LIEU_BA_CAP_232)
            xSystem.NhienLieu->AddNewData(Value);

#if (__BA4_HW__ == 1 || __BA3G01_HW__ == 1)
        if (xSystem.Parameters.SoundEnable == __BUS_TRUONG_HAI_BA4__ || xSystem.Parameters.SoundEnable == __BUS_HAI_PHONG__)
            ManHinh_RecieveData(Value);
#endif
#endif
        USART_ClearITPendingBit(MULTIFUNCTION_UART, USART_IT_RXNE);
    }
}

/*******************************************************************************
 * Function Name    : EXTI1_IRQHandler
 * Return           : None
 * Parameters       : None
 * Created by       : Khanhpd
 * Date created     : 10/11/2014
 * Description      : This function handles UART4 global interrupt request.
 * Notes            :
 *******************************************************************************/
void VANTOC_IRQHandler(void)
{
    if (EXTI_GetITStatus(VANTOC_EXTI_LINE) != RESET)
    {
        VanTocInterruptHandler();

#if (__TAXI_FUNC__ == 1)
        /* Ham xu ly xung co khach/khong khach cua dong ho Patent va Adsun */
        Taxi_PulseTick();
#endif

        /* Clear the EXTI line 0 pending bit */
        EXTI_ClearITPendingBit(VANTOC_EXTI_LINE);
    }
}

/*******************************************************************************
* Function Name     : TIM2_IRQHandler
* Return            : None
* Parameters        : None
* Description       : Ham xu ly ngat Timer 2                :
*******************************************************************************/

//void TIM2_IRQHandler(void)    /* Ngat 10ms */     //Thay doi co bao ghi du lieu
//{
//  if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
//      {
//#if (__TCT03_HW__ == 1) || defined (__BA4E__) || defined (__BA4ETX__) || defined (__BA4E_ExtRFID__)
//      GPIO_WriteBit(BUZZER_PORT, BUZZER_PIN,(BitAction)(1 - GPIO_ReadOutputDataBit(BUZZER_PORT, BUZZER_PIN)));
//#endif
//
//#if (__TAXI_FUNC__ == 1)
//      Taxi_ProcessDIR();
//#endif
//      TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
//      }

//}

/*******************************************************************************
 * Function Name    : CAN1_RX0_IRQHandler
 * Return           : None
 * Parameters       : None
 * Created by       : Khanhpd
 * Date created     : 10/11/2014
 * Description      :
 * Notes            :
 *******************************************************************************/
#if (__BA4_HW__ == 1 || __BA3G01_HW__ == 1)
void CAN1_RX0_IRQHandler(void)
{
    //CanRxMsg RxMessage;

    if (CAN_GetITStatus(CAN1, CAN_IT_FMP0))
    {
        CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
    }
}
#endif

/********************************* END OF FILE *******************************/
