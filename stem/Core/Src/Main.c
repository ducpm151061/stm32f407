/******************************************************************************
 * @file        Main.c
 * @author
 * @version     V1.0.0
 * @date        10/11/2014
 * @brief
 ******************************************************************************/

/******************************************************************************
                                   INCLUDES
 ******************************************************************************/
#include <stdint.h>
#include "InitSystem.h"
#include "DataDefine.h"
#include "AlcoholMeter.h"
#include "Debug.h"
#include "TPRT.h"
#include "AirPump.h"
#include "Parameters.h"

#include "GUI.h"
#include "ResistiveTouch.h"
#include "ILI9488.h"
#include "UserGUIService.h"

// USB define
#include "usbd_hid_core.h"
#include "usbd_usr.h"
#include "usbd_desc.h"
#include "usb_conf.h"
extern USB_OTG_CORE_HANDLE USB_OTG_dev;
extern void Battery_Tick(void);

// config test define
#define FLASH_SAVE_ADDR 0x080E0000
const uint8_t send_test[] = {"STM32F405VxT6 FLASH TEST1234567890 STM32F405VxT6 FLASH TEST1234567890 STM32F405VxT6 FLASH TEST1234567890 STM32F405VxT6 FLASH TEST1234567890 STM32F405VxT6 FLASH TEST1234567890 STM32F405VxT6 FLASH TEST1234567890"};
#define SEND_SIZE sizeof(send_test)
#define DATA_SIZE SEND_SIZE / 4 + ((SEND_SIZE % 4) ? 1 : 0)
uint8_t rece_buf[DATA_SIZE];

System_t xSystem;
extern __IO int32_t OS_TimeMS;
/******************************************************************************
                                   GLOBAL FUNCTIONS
 ******************************************************************************/

/******************************************************************************
                                   DATA TYPE DEFINE
 ******************************************************************************/

/******************************************************************************
                                   PRIVATE VARIABLES
 ******************************************************************************/
static uint32_t Timeout1ms = 0;
static uint16_t Timeout10ms = 0;
static uint16_t Timeout100ms = 0;
static uint16_t Timeout1000ms = 0;
static uint16_t Timeout5000ms = 0;
static uint32_t LastTimeMs = 0;
static uint32_t ClkCheckPowerdown = 0;

/******************************************************************************
                                   LOCAL FUNCTIONS
 ******************************************************************************/
static void ProcessTimeout5000ms(void);
static void ProcessTimeout1000ms(void);
static void ProcessTimeout100ms(void);
static void ProcessTimeout10ms(void);
static void ProcessTimeout1ms(void);

void CTP_GetTouchPoint_Tick(void);

extern uint16_t GetPressure(void);

extern uint16_t GetV_Bat(void);
extern uint16_t GetVBat(void);
extern uint16_t GetPercent_Bat(void);

extern uint16_t GetTempSensor(void);
extern uint16_t GetTempPrinter(void);
extern uint16_t GetVoltageIn(void);
extern uint16_t GetVIn(void);

extern uint16_t GetV_8V2(void);
extern uint16_t GetVCCAirPump(void);
extern int8_t GetTPRTTemp(void);
extern void TimeGPS_Tick(void);

/*****************************************************************************/
/**
 * @brief   :
 * @param   :
 * @retval  :
 * @author  :
 * @created :
 * @version :
 * @reviewer:
 */

extern uint16_t CountTimerPRT;
uint8_t RTP_ReqNew = 0;

void WriteString(void)
{
    DebugPrint("WRITE CONFIG AFTER INIT\r\n");
    xSystem.InternalFlash->Prepare(xSystem.InternalFlash->GetSectorIndex(FLASH_SAVE_ADDR));
    xSystem.InternalFlash->WriteBytes((uint32_t)FLASH_SAVE_ADDR, (uint8_t *)send_test, SEND_SIZE);
    DebugPrint("\rWrite successfull!!!\r\n");
}

void ReadString(void)
{
    xSystem.InternalFlash->ReadBytes((uint32_t)FLASH_SAVE_ADDR, (uint8_t *)rece_buf, SEND_SIZE);
    DebugPrint("\rData Read: %s\r\n", (const char *)rece_buf);
    DebugPrint("\rLength Data Readed: %d\r\n", strlen(rece_buf));
}

void WriteStruct(void)
{
    uint16_t i;
    FLASH_Status Status;

    int readSize = sizeof(Parameter_t);
    DebugPrint("START CONFIG AFTER INIT\r\n");
    if (xSystem.InternalFlash->Prepare(xSystem.InternalFlash->GetSectorIndex(FLASH_SAVE_ADDR)))
    {
        DebugPrint("\rFAIL\r\n");
    }
    else
        DebugPrint("\rSUCCESS\r\n");

    // Init data Write
    snprintf((char *)xSystem.Parameters.DeviceID, sizeof(xSystem.Parameters.DeviceID), "%s", "A00000001");
    snprintf((char *)xSystem.Parameters.SetupKey, sizeof(xSystem.Parameters.SetupKey), "%s", "12345678");
    snprintf((char *)xSystem.Parameters.NgayXuatXuong, sizeof(xSystem.Parameters.NgayXuatXuong), "%s", "12/12/2012");
    snprintf((char *)xSystem.Parameters.NgayHieuChinh, sizeof(xSystem.Parameters.NgayHieuChinh), "%s", "00/00/2013");
    snprintf((char *)xSystem.Parameters.NgayDuPhong, sizeof(xSystem.Parameters.NgayDuPhong), "%s", "10/12/2015");
    snprintf((char *)xSystem.Parameters.DonVi, sizeof(xSystem.Parameters.DonVi), "%s", "CAHN");
    snprintf((char *)xSystem.Parameters.NguoiThucHien, sizeof(xSystem.Parameters.NguoiThucHien), "%s", "Nguyen Van B");
    snprintf((char *)xSystem.Parameters.Pad, sizeof(xSystem.Parameters.Pad), "%s", "01234");

    xSystem.Parameters.BootloaderVersion = 0x01;
    xSystem.Parameters.BlowTime = 1;
    xSystem.Parameters.BlowPressure = 1;
    xSystem.Parameters.TimeAutoTurnOff = 5;
    xSystem.Parameters.TimeoutKeypad = 2;

    xSystem.Parameters.AlMCalibData.s.Al_MaxValue = 11;

    xSystem.Parameters.AlMCalibData.s.Data[0].ValueADC = 0;
    xSystem.Parameters.AlMCalibData.s.Data[0].ValueMgL = 0;

    xSystem.Parameters.AlMCalibData.s.Data[1].ValueADC = 187;
    xSystem.Parameters.AlMCalibData.s.Data[1].ValueMgL = 50;

    xSystem.Parameters.AlMCalibData.s.Data[2].ValueADC = 383;
    xSystem.Parameters.AlMCalibData.s.Data[2].ValueMgL = 100;

    xSystem.Parameters.AlMCalibData.s.Data[3].ValueADC = 435;
    xSystem.Parameters.AlMCalibData.s.Data[3].ValueMgL = 150;

    xSystem.Parameters.AlMCalibData.s.Data[4].ValueADC = 686;
    xSystem.Parameters.AlMCalibData.s.Data[4].ValueMgL = 200;

    xSystem.Parameters.AlMCalibData.s.Data[5].ValueADC = 714;
    xSystem.Parameters.AlMCalibData.s.Data[5].ValueMgL = 250;

    xSystem.Parameters.AlMCalibData.s.Data[6].ValueADC = 831;
    xSystem.Parameters.AlMCalibData.s.Data[6].ValueMgL = 300;

    xSystem.Parameters.AlMCalibData.s.Data[7].ValueADC = 999;
    xSystem.Parameters.AlMCalibData.s.Data[7].ValueMgL = 350;

    xSystem.Parameters.AlMCalibData.s.Data[8].ValueADC = 1145;
    xSystem.Parameters.AlMCalibData.s.Data[8].ValueMgL = 400;

    xSystem.Parameters.AlMCalibData.s.Data[9].ValueADC = 1729;
    xSystem.Parameters.AlMCalibData.s.Data[9].ValueMgL = 600;

    xSystem.Parameters.AlMCalibData.s.Data[10].ValueADC = 2894;
    xSystem.Parameters.AlMCalibData.s.Data[10].ValueMgL = 972;

    xSystem.Parameters.CRC16 = CalculateChecksumCRC16((uint8_t *)&xSystem.Parameters, readSize - 2);

    xSystem.InternalFlash->WriteBytes(FLASH_SAVE_ADDR, (uint8_t *)&xSystem.Parameters, sizeof(Parameter_t));
    DebugPrint("\rWrite successful!!!\r\n");
}

void ReadStruct(void)
{
    int readSize = sizeof(Parameter_t);
    char *buffTmp = malloc(readSize + 2);
    if (buffTmp == NULL)
        return;
    Parameter_t *ParametersTmp = (Parameter_t *)(buffTmp);
    memset(buffTmp, 0, readSize);
    xSystem.InternalFlash->ReadBytes(FLASH_SAVE_ADDR, (uint8_t *)buffTmp, readSize);
    DebugPrint("\r\r%s\n", buffTmp);
    DebugPrint("\rRead struct config: %d:%d:%d:%d:%d:%s:%s:%s:%s:%s:%s:%s\r\n", ParametersTmp->BootloaderVersion, ParametersTmp->BlowTime, ParametersTmp->BlowPressure, ParametersTmp->TimeAutoTurnOff, ParametersTmp->TimeoutKeypad, ParametersTmp->DeviceID, ParametersTmp->NgayXuatXuong, ParametersTmp->NgayHieuChinh, ParametersTmp->NgayDuPhong, ParametersTmp->DonVi, ParametersTmp->NguoiThucHien, ParametersTmp->Pad);
    DebugPrint("\rRead struct calib: %d/%d, %d/%d, %d/%d \r\n", ParametersTmp->AlMCalibData.s.Data[0].ValueADC, ParametersTmp->AlMCalibData.s.Data[0].ValueMgL, ParametersTmp->AlMCalibData.s.Data[1].ValueADC, ParametersTmp->AlMCalibData.s.Data[1].ValueMgL, ParametersTmp->AlMCalibData.s.Data[2].ValueADC, ParametersTmp->AlMCalibData.s.Data[2].ValueMgL);
    DebugPrint("\r\Read CRC16: %04Xr\n", ParametersTmp->CRC16);
    free(buffTmp);
}

int main(void)
{
    InitSystem();

    UserGUI_WMInit();
    xSystem.CTP.CTP_Sens_DF = 20;

    while (1)
    {
        //RTP_GetTouchPoint_Tick();
        CTP_GetTouchPoint_Tick();
        if (Timeout1ms != xSystem.TickCount1Ms)
        {
            // ProcessTimeout1ms();
            AlcoholMetter_Tick();
            Timeout1ms = xSystem.TickCount1Ms;
        }
        if (Timeout10ms >= 10)
        {
            if (xSystem.CTP.CTP_Sens)
                xSystem.CTP.CTP_Sens--;
            Timeout10ms = 0;
            ProcessTimeout10ms();

            // TimeGPS_Tick();
        }
        if (Timeout100ms >= 100)
        {
            RTP_ReqNew = 1;
            Timeout100ms = 0;
            ProcessTimeout100ms();
        }
        ResetWatchdog();
        if (Timeout1000ms >= 1000)
        {
            Timeout1000ms = 0;
            ProcessTimeout1000ms();
            CountTimerPRT = 0;
        }
        if (Timeout5000ms >= 5000)
        {
            Timeout5000ms = 0;
            ProcessTimeout5000ms();
        }
        ResetWatchdog();
    }
}

__IO uint32_t TimingDelay = 0;
void Delay_Decrement(void)
{
    if (TimingDelay > 0)
        TimingDelay--;
}

//Ham Delayms
void DelayMS(uint16_t ms)
{
    TimingDelay = xSystem.TickCount1Ms;
    while (TimingDelay + ms > xSystem.TickCount1Ms)
        ResetWatchdog();
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
void SysTick_Handler(void)
{
    Timeout10ms++;
    Timeout100ms++;
    Timeout1000ms++;
    Timeout5000ms++;
    OS_TimeMS++;
    xSystem.TickCount1Ms++;
    //  Delay_Decrement();
}

/*****************************************************************************/
/**
 * @brief   :
 * @param   :
 * @retval  :
 * @author  :
 * @created :
 * @version :
 * @reviewer:
 */
static void ProcessTimeout10ms(void)
{
    Debug_Tick();
    if (ClkCheckPowerdown > 0)
    {
        if (GPIO_ReadInputDataBit(BUTTONPWR_Port, BUTTONPWR_Pin) == 0)
        {
            ClkCheckPowerdown++;
            DebugPrint("\rClkCheckPowerdown trong timer 10ms: %d\r\n", ClkCheckPowerdown);
        }
        else
        {
            ClkCheckPowerdown = 0;
        }
    }
}

static void ProcessTimeout1ms(void)
{
    // checkCommand(&USB_OTG_dev);
}

/*****************************************************************************/
/**
 * @brief   :
 * @param   :
 * @retval  :
 * @author  :
 * @created :
 * @version :
 * @reviewer:
 */

//static uint16_t lastPressure = 0;

static void ProcessTimeout100ms(void)
{
    static uint8_t AP_Check = 0;
    // update battery every 10s
    if (xSystem.GLStatus.RequestCheckBattery == 100)
    {
        Battery_Tick();
        xSystem.GLStatus.RequestCheckBattery = 0;
    }
    else
    {
        xSystem.GLStatus.RequestCheckBattery++;
    }
    if (GPIO_ReadInputDataBit(BUTTON1_Port, BUTTON1_Pin) == 0)
    {
        // ReadStruct();

        /* if (AP_Check == 0)
        {
            AP_Ctrl_Pull(20);
            AP_Check = 1;
        }
        else
        {
            AP_Ctrl_Push(20);
            AP_Check = 0;
        } */
    }

    AP_Tick();

    xSystem.GPS->Tick();
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
void TurnOffPower(void)
{
    GPIO_ResetBits(VSYS_PWR_CTRL_Port, VSYS_PWR_CTRL_Pin);
    Beep(5);
    xSystem.TimeOut.Clk1s_CountToTurnOff = 0;
}

typedef void (*pFunction)(void);
pFunction Jump_To_Application;
uint32_t JumpAddress;
extern void WD_Header_Tick(void);
static void ProcessTimeout1000ms(void)
{
    xSystem.TickCount1s++;
    xSystem.Rtc->Tick();

    LED_STATUS_Port->ODR ^= 1;
    Parameter_Tick();

    if (xSystem.TimeOut.Clk1s_JumpToDFU > 0)
    {
        xSystem.TimeOut.Clk1s_JumpToDFU--;
    }
    /* Check Power Down */
    if (GPIO_ReadInputDataBit(BUTTONPWR_Port, BUTTONPWR_Pin) == 0)
    {
        ClkCheckPowerdown = (ClkCheckPowerdown <= 0) ? 1 : ClkCheckPowerdown;
        if (xSystem.TimeOut.Clk1s_JumpToDFU > 0)
        {
            /* Jump to user application */
            RCC_DeInit();
            /**
            * Step: Disable systick timer and reset it to default values
            */
            SysTick->CTRL = 0;
            SysTick->LOAD = 0;
            SysTick->VAL = 0;
            SYSCFG->MEMRMP = 0x01;

            JumpAddress = *(__IO uint32_t *)(0x1FFF0000 + 4);
            Jump_To_Application = (pFunction)JumpAddress;
            /* Initialize user application's Stack Pointer */
            __set_MSP(*(__IO uint32_t *)0x1FFF0000);
            Jump_To_Application();
        }
    }
    if (ClkCheckPowerdown / 100 > 2)
    {
        DebugPrint("\rClkCheckPowerdown trong timer 1s: %d\r\n", ClkCheckPowerdown);
        TurnOffPower();
    }
    WD_Header_Tick();
    if (xSystem.Parameters.TimeAutoTurnOff > 1)
    {
        if (xSystem.TimeOut.Clk1s_CountToTurnOff++ >= xSystem.Parameters.TimeAutoTurnOff * 30)
        {
            TurnOffPower();
        }
    }
    // GPIO_SetBits(VSYS_PWR_CTRL_Port, VSYS_PWR_CTRL_Pin);
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

static void ProcessTimeout5000ms(void)
{
}

//Ham Delayms
void Delayms(uint16_t ms)
{
    TimingDelay = ms;
    while (TimingDelay)
    {
        if (TimingDelay % 10 == 0)
            ResetWatchdog();
    }
}

extern void ResetWatchdog(void);

Utility_t UtilityFn =
    {
        Delayms,
        ResetWatchdog};

/********************************* END OF FILE *******************************/
