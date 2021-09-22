/******************************************************************************
 * @file        InitSystem.c
 * @author
 * @version     V1.0.0
 * @date        10/11/2014
 * @brief
 ******************************************************************************/

/******************************************************************************
                                   INCLUDES
 ******************************************************************************/
#include "InitSystem.h"
#include "DataDefine.h"
#include "Parameters.h"
#include "AlcoholMeter.h"
#include "Debug.h"
#include "DriverUART.h"
#include "ILI9488.h"
#include "ResistiveTouch.h"
#include "GUI.h"
#include "TPRT.h"
#include "AirPump.h"
// Define usb lib
#include "usbd_hid_core.h"
#include "usbd_usr.h"
#include "usbd_desc.h"
#include "usb_conf.h"
#include "usart.h"
#include "delay.h"
USB_OTG_CORE_HANDLE USB_OTG_dev;

/******************************************************************************
                                   GLOBAL VARIABLES
 ******************************************************************************/
extern System_t xSystem;
extern RTC_PERIPHERAL Driver_RTC;
extern INTERNAL_FLASH_t Driver_InterFlash;
extern SPI_Driver_t SPI_Driver;
extern GPS_PERIPHERAL Driver_GPS;
extern Utility_t UtilityFn;
extern Flash_External_t Driver_Flash;
__IO uint16_t ADC_RegularConvertedValueTab[7];

/******************************************************************************
                                   GLOBAL FUNCTIONS
 ******************************************************************************/

/******************************************************************************
                                   DATA TYPE DEFINE
 ******************************************************************************/
#define ADC1_DR_ADDRESS ((uint32_t)0x4001204C)
#define ADC2_DR_ADDRESS ((uint32_t)0x4001214C)

/******************************************************************************
                                   PRIVATE VARIABLES
 ******************************************************************************/

const char WelcomeStr[] = "\rCopyright by SmartElecDevice@Gmail.com\r";

/******************************************************************************
                                   LOCAL FUNCTIONS
 ******************************************************************************/
static void InitVariables(void);
static void InitRCC(void);
static void InitIO(void);
static void DrawScreen(void);
static void InitADC(void);
static void InitIDWG(void);
static void InitOS(void);
static void InitOtherFunction(void);
static void UserGUI_Init(void);
static void InitBuzzerTimer(void);
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

RCC_ClocksTypeDef SysClock;
static void Hardware_CheckClockSource(void)
{
    uint8_t NguonClock = 0;
    NguonClock = RCC_GetSYSCLKSource();
    if (NguonClock == 0x00)
        DebugPrint("\rCLOCK: Nguon clock tu thach anh noi.\r\n");
    if (NguonClock == 0x04)
        DebugPrint("\rCLOCK: Nguon clock tu thach anh ngoai.\r\n");
    if (NguonClock == 0x08)
        DebugPrint("\rCLOCK: Nguon clock tu PLL.\r\n");
    RCC_GetClocksFreq(&SysClock);
    DebugPrint(" Tan so:\r\
	- SYSCLK_Fre: %uHz.\r\
	- HCLK_Fre: %uHz.\r\
	- PCLK1_Fre: %uHz.\r\
	- PCLK2_Fre: %uHz.\r\
	",
               SysClock.SYSCLK_Frequency, SysClock.HCLK_Frequency, SysClock.PCLK1_Frequency, SysClock.PCLK2_Frequency);
}

static void Hardware_CheckResetReason(void)
{
    if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
    {
        DebugPrint("\rRR PIN.\r\n");
    }

    if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
    {
        DebugPrint("\rRR POR.\r\n");
    }

    if (RCC_GetFlagStatus(RCC_FLAG_SFTRST) != RESET)
    {
        DebugPrint("\rRR SFT.\r\n");
    }

    if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET)
    {
        DebugPrint("\rRR IWDG.\r\n");
    }

    if (RCC_GetFlagStatus(RCC_FLAG_WWDGRST) != RESET)
    {
        DebugPrint("\rRR WWDG.\r\n");
    }

    if (RCC_GetFlagStatus(RCC_FLAG_LPWRRST) != RESET)
    {
        DebugPrint("\rRR LPWR.\r\n");
    }

    /* Clear co RCC */
    RCC_ClearFlag();
}

void FT6236_Init(void);

void InitSystem(void)
{
    SystemCoreClockUpdate();

    InitVariables();

    InitRCC();

    InitDebug();

    InitIO();

    DebugPrint("\rSystem Core Clock: %d \r\n", SystemCoreClock);
    Hardware_CheckResetReason();
    Hardware_CheckClockSource();

    InitIDWG();

    InitADC();

    InitOS();
    xSystem.Rtc->Init();

    InitBuzzerTimer();

    // Init timer gps update
    InitTimerGPSUpdate();
    DebugPrint("\rInit Timer GPS Update\r\n");

    InitTimerTransferData();
    DebugPrint("\rInit Timer Transfer Data USB\r\n");

    DebugPrint("\rWdt Init...\r\n");
    ResetWatchdog();

    ReadConfig(0);

    DebugPrint("\rILI9488_Init...\r\n");
    ILI9488_Init();

    DebugPrint("\rUserGUI_Init...\r\n");
    UserGUI_Init();
    //RTP_Init();
    DebugPrint("\rFT6236_Init...\r\n");
    FT6236_Init();
    DebugPrint("\rAlM_Init...\r\n");
    AlcoholMetter_Init();

    Beeps(1);
    xSystem.GPS->Init();
    TPRT_Initialize();

    xSystem.Debug.ExFlash = 1;
    xSystem.ExFlash->Init();
    AP_Init();
    ReadIndexRecord();
    //InitUSB();
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
static void InitOS(void)
{
    if (SysTick_Config(SystemCoreClock / 1000))
        while (1)
            ;

    /* Configure the SysTick handler priority */
    NVIC_SetPriority(SysTick_IRQn, 0x0);
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
static void InitVariables(void)
{
    uint8_t i;
    xSystem.Rtc = &Driver_RTC;
    xSystem.InternalFlash = &Driver_InterFlash;
    xSystem.GPS = &Driver_GPS;
    xSystem.SPI = &SPI_Driver;
    xSystem.Utility = &UtilityFn;
    xSystem.ExFlash = &Driver_Flash;

    xSystem.GLStatus.DebugMode = 1;

    xSystem.GLStatus.DebugTimeOut = 60;

    xSystem.TickCount1s = 0;
    xSystem.TickCount1Ms = 0;
    xSystem.GLStatus.InitSystemDone = 0;
    xSystem.GLStatus.SyncTimeGps = 0;
    xSystem.GLStatus.TmpTime = 0;
    xSystem.GLStatus.TmpParam = 0;
    xSystem.GLStatus.RequestCheckBattery = 100;
    xSystem.TimeOut.Clk1s_JumpToDFU = 0;
    xSystem.TimeOut.Clk1s_CountToTurnOff = 0;
}

static void InitBuzzerTimer(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    //  RCC_ClocksTypeDef tstClock;

    /* Enable the TIM3 global Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* TIM3 clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    /* Timer Setting Calculation
    Pulse-buzzer works well at frequencys 2.7kHz
    TimerInterrupt  = f(TIM2)/((Period + 1)*(Prescaler + 1)*(Repetition + 1))
                    = 84.000.000/(1000 * 15 * 1)
                    ~ 5400
    --> f(Buzzer) = 1/2 f(TimInterrupt) = 2700 Hz */

    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = 1000 - 1; //100

    TIM_TimeBaseStructure.TIM_Prescaler = 15;

    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    /* TIM IT enable */
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

    /* TIM3 disable counter */
    TIM_Cmd(TIM3, ENABLE);
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
static void InitRCC(void)
{

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC |
                               RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOG |
                               RCC_AHB1Periph_GPIOH | RCC_AHB1Periph_GPIOI,
                           ENABLE);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 | RCC_APB2Periph_SYSCFG, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 | RCC_APB1Periph_USART3 |
                               RCC_APB1Periph_UART4 | RCC_APB1Periph_UART5 | RCC_APB1Periph_SPI2 | RCC_APB1Periph_SPI3,
                           ENABLE);

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
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
static void InitIO(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = LED_STATUS_Pin;

    GPIO_Init(LED_STATUS_Port, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Pin = BUZZER_PIN;
    GPIO_Init(BUZZER_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = VSYS_PWR_CTRL_Pin;
    GPIO_Init(VSYS_PWR_CTRL_Port, &GPIO_InitStructure);
    GPIO_SetBits(VSYS_PWR_CTRL_Port, VSYS_PWR_CTRL_Pin);

    GPIO_InitStructure.GPIO_Pin = ADC_CS_PIN;
    GPIO_Init(ADC_CS_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = BUTTON1_Pin;
    GPIO_Init(BUTTON1_Port, &GPIO_InitStructure);

    /* GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; */
    GPIO_InitStructure.GPIO_Pin = BUTTONPWR_Pin;
    GPIO_Init(BUTTONPWR_Port, &GPIO_InitStructure);
}

static void InitADC(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    ADC_InitTypeDef ADC_InitStructure;
    ADC_CommonInitTypeDef ADC_CommonInitStructure;
    DMA_InitTypeDef DMA_InitStructure;

    /* Enable ADC1, DMA2 and GPIO clocks ****************************************/
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2 | RCC_AHB1Periph_GPIOC, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

    /* DMA2 Stream0 channel2 configuration **************************************/
    DMA_DeInit(DMA2_Stream0);
    DMA_InitStructure.DMA_Channel = DMA_Channel_0;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC1_DR_ADDRESS;
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&ADC_RegularConvertedValueTab;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;

    DMA_InitStructure.DMA_BufferSize = 7;

    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    DMA_Init(DMA2_Stream0, &DMA_InitStructure);
    DMA_Cmd(DMA2_Stream0, ENABLE);

    /* Configure ADC1 Channel 12 pin as analog input ******************************/
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

    GPIO_InitStructure.GPIO_Pin = VIN_PIN;
    GPIO_Init(VIN_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = AIR_PUMP_VCC_MEASURE_PIN;
    GPIO_Init(AIR_PUMP_VCC_MEASURE_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = V8_2_PIN;
    GPIO_Init(V8_2_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = BATTERY_PIN;
    GPIO_Init(BATTERY_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = PRESSURE_SENSOR_PIN;
    GPIO_Init(PRESSURE_SENSOR_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = TEMPERATURE_SENSOR_PIN;
    GPIO_Init(TEMPERATURE_SENSOR_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = TPRT_TEMP_PIN;
    GPIO_Init(TPRT_TEMP_PORT, &GPIO_InitStructure);

    /* ADC Common Init **********************************************************/
    ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
    ADC_CommonInit(&ADC_CommonInitStructure);

    /* ADC1 Init ****************************************************************/
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;

    ADC_InitStructure.ADC_NbrOfConversion = 7;

    ADC_Init(ADC1, &ADC_InitStructure);

    /* ADC1 regular channel configuration *************************************/
    ADC_RegularChannelConfig(ADC1, BATTERY_CHANNEL, 1, ADC_SampleTime_84Cycles);
    ADC_RegularChannelConfig(ADC1, PRESSURE_SENSOR_CHANNEL, 2, ADC_SampleTime_84Cycles);
    ADC_RegularChannelConfig(ADC1, TEMPERATURE_SENSOR_CHANNEL, 3, ADC_SampleTime_84Cycles);
    ADC_RegularChannelConfig(ADC1, VIN_CHANNEL, 4, ADC_SampleTime_84Cycles);
    ADC_RegularChannelConfig(ADC1, AIR_PUMP_VCC_MEASURE_CHANNEL, 5, ADC_SampleTime_84Cycles);
    ADC_RegularChannelConfig(ADC1, V8_2_CHANNEL, 6, ADC_SampleTime_84Cycles);
    ADC_RegularChannelConfig(ADC1, TPRT_TEMP_CHANNEL, 7, ADC_SampleTime_84Cycles);

    /* Enable DMA request after last transfer (Single-ADC mode) */
    ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);

    /* Enable ADC1 DMA */
    ADC_DMACmd(ADC1, ENABLE);

    /* Enable ADC1 */
    ADC_Cmd(ADC1, ENABLE);

    ADC_SoftwareStartConv(ADC1);
}

/*****************************************************************************/
/**
 * @brief   :   Init IWDG
 * @param   :   None
 * @retval  :   None
 * @author  :
 * @created :   15/01/2014
 * @version :
 * @reviewer:
 */
static void InitIDWG(void)
{
    /* Enable write access to IWDG_PR and IWDG_RLR registers */
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

    /* IWDG counter clock: 40KHz(LSI) / 32 = 1.25 KHz */
    IWDG_SetPrescaler(IWDG_Prescaler_128);

    /* Set counter reload value to 349 */
    IWDG_SetReload(0xFFF);

    /* Reload IWDG counter */
    IWDG_ReloadCounter();

    /* Enable IWDG (the LSI oscillator will be enabled by hardware) */
    IWDG_Enable();
}

uint8_t TestOutSideWatchDog = 0;
void ResetWatchdog(void)
{
    static uint8_t WDI_Count = 0;

    if (TestOutSideWatchDog > 0)
    {
        IWDG_ReloadCounter();
        return;
    }

    IWDG_ReloadCounter();
}

/*****************************************************************************/
/**
 * @brief   :   DrawScreen
 * @param   :   None
 * @retval  :   None
 * @author  :
 * @created :   15/01/2014
 * @version :
 * @reviewer:
 */
void DrawScreen(void)
{
    DebugPrint("\r\r\r\r");
    DebugPrint("\r********************************************************************************\r\n");
    DebugPrint(WelcomeStr);
    DebugPrint("\r********************************************************************************\r\n");

    DebugPrint("%s", "\r********************************************************************************\r\n");
}

/*****************************************************************************/
/**
 * @brief   :   Init other functions
 * @param   :   None
 * @retval  :   None
 * @author  :
 * @created :   15/01/2014
 * @version :
 * @reviewer:
 */
static void InitOtherFunction(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
}

static void UserGUI_Init(void)
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC, ENABLE);

    GUI_Init();
}

void InitUSB(void)
{
    // delay_init(168);
    UART4_Init(921600);
    USBD_Init(&USB_OTG_dev, USB_OTG_FS_CORE_ID, &USR_desc, &USBD_HID_cb, &USR_cb);
}

void DeInitUSB(void)
{
    USB_OTG_BSP_DisableInterrupt();
    DCD_DevDisconnect(&USB_OTG_dev);
    USBD_DeInit(&USB_OTG_dev);
    USB_OTG_StopDevice(&USB_OTG_dev);
}

void InitTimerGPSUpdate(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    //  RCC_ClocksTypeDef tstClock;

    /* Enable the TIM5 global Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* TIM5 clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);

    /* Timer Setting Calculation tick 2 ms => 500 Hz
    Pulse-buzzer works well at frequencys 500 Hz
    TimerInterrupt  = f(TIM2)/((Period + 1)*(Prescaler + 1)*(Repetition + 1))
                    = 168.000.000/(168 * 2000 * 1)
                    ~ 500
    */
    // Period 0xFFFF Prescaler 0xFFFF -> 50s
    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = 0xFFFF - 1; //100

    TIM_TimeBaseStructure.TIM_Prescaler = 0xFFFF - 1;

    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);

    /* TIM IT enable */
    TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);

    /* TIM5 enable counter */
    TIM_Cmd(TIM5, ENABLE);
}

void TIM5_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
    {
        if (xSystem.GLStatus.SyncTimeGps == 1)
        {
            TIM_Cmd(TIM5, DISABLE);
            Set_TimeAuto();
            xSystem.GLStatus.SyncTimeGps = 0;
            TIM_Cmd(TIM5, ENABLE);
        }
        TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
    }
}

void InitTimerTransferData(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    //  RCC_ClocksTypeDef tstClock;

    /* Enable the TIM7 global Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 7;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 7;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* TIM7 clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);

    /* Timer Setting Calculation tick 1 ms => 100000 Hz
    Pulse-buzzer works well at frequencys 100000 Hz
    TimerInterrupt  = f(TIM7)/((Period + 1)*(Prescaler + 1)*(Repetition + 1))
                    = 168.000.000/(168 * 10 * 1)
                    ~ 100000
    */
    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = 168 - 1; //100

    TIM_TimeBaseStructure.TIM_Prescaler = 10 - 1;

    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);

    /* TIM IT enable */
    TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);

    /* TIM7 disable counter */
    TIM_Cmd(TIM7, ENABLE);
}

void TIM7_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)
    {
        if (checkCommand(&USB_OTG_dev) == 1)
        {
            sendFlashData(&USB_OTG_dev);
            while (!xSystem.GLStatus.DoneSend)
            {
            }
            DebugPrint("\rDa gui xong du lieu\r\n");
        }
        TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
    }
}

/********************************* END OF FILE *******************************/
