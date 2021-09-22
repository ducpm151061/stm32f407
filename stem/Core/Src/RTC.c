/******************************************************************************
 * @file        SourceFileTemplate.c
 * @author
 * @version     V1.0.0
 * @date        15/01/2014
 * @brief
 ******************************************************************************/

/******************************************************************************
                                   INCLUDES
 ******************************************************************************/
#include <time.h>
#include "RTC.h"
#include "DataDefine.h"
#include "Utilities.h"

time_t g_sys_time = 0;

/******************************************************************************
                                   GLOBAL VARIABLES
 ******************************************************************************/

/******************************************************************************
                                   GLOBAL FUNCTIONS
 ******************************************************************************/

/******************************************************************************
                                   DATA TYPE DEFINE
 ******************************************************************************/
#define RTC_CLOCK_SOURCE_LSE
//#define RTC_CLOCK_SOURCE_LSI

#define FIRSTYEAR 2000 // start year
#define FIRSTDAY 6     // 0 = Sunday

#define RTC_INIT_FAIL 0
#define RTC_LSE_INIT_OK 1
#define RTC_LSI_INIT_OK 2

/******************************************************************************
                                   PRIVATE VARIABLES
 ******************************************************************************/

RTC_InitTypeDef RTC_InitStructure;
__IO uint32_t AsynchPrediv = 0, SynchPrediv = 0;

static const uint8_t DaysInMonth[] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

static RTC_TimeTypeDef tmpRTC_TimeStructure;
static RTC_DateTypeDef tmpRTC_DateStructure;
static DateTime_t LastRtcTime;
static uint32_t LastRtcCounter = 0;
static uint8_t RTC_State = RTC_INIT_FAIL;

/******************************************************************************
                                   LOCAL FUNCTIONS
 ******************************************************************************/
static void RTC_Config(void);
static void RTC_Tick(void);

/*****************************************************************************/

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
static void RTC_User_Init(void)
{
    /* Enable the PWR clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

    /* Allow access to RTC */
    PWR_BackupAccessCmd(ENABLE);

    if (RTC_ReadBackupRegister(RTC_BKP_DR0) != 0x32F2)
    {
        /* RTC configuration  */
        RTC_Config();

        /* Configure the RTC data register and RTC prescaler */
        RTC_InitStructure.RTC_AsynchPrediv = AsynchPrediv;
        RTC_InitStructure.RTC_SynchPrediv = SynchPrediv;
        RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;

        /* Check on RTC init */
        if (RTC_Init(&RTC_InitStructure) == ERROR)
        {
            DebugPrint("\rRTC Prescaler Config failed\r\n");
        }
        else
        {
            DebugPrint("\rKhoi tao RTC : [OK]\r\n");
            RTC_Tick();
        }
    }
    else
    {
        /* Enable the PWR clock */
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

        /* Allow access to RTC */
        PWR_BackupAccessCmd(ENABLE);

        /* Wait for RTC APB registers synchronisation */
        RTC_WaitForSynchro();

        /* Clear the RTC Alarm Flag */
        RTC_ClearFlag(RTC_FLAG_ALRAF);

        /* Clear the EXTI Line 17 Pending bit (Connected internally to RTC Alarm) */
        EXTI_ClearITPendingBit(EXTI_Line17);

        DebugPrint("\rKhoi tao RTC : [OK]\r\n");
        RTC_Tick();

        RTC_State = RTC_LSE_INIT_OK;
    }
}

/*****************************************************************************/
/**
 * @brief   :   Configure the RTC peripheral by selecting the clock source
 * @param   :
 * @retval  :
 * @author  :
 * @created :   15/01/2014
 * @version :
 * @reviewer:
 */
static void RTC_Config(void)
{
    uint32_t i = 0xFFFFFF;

#if defined(RTC_CLOCK_SOURCE_LSI) /* LSI used as RTC source clock*/
    /* The RTC Clock may varies due to LSI frequency dispersion. */
    /* Enable the LSI OSC */
    RCC_LSICmd(ENABLE);

    /* Wait till LSI is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET && i > 1)
    {
        i--;
    }

    /* Select the RTC Clock Source */
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);

    SynchPrediv = 0xFF;
    AsynchPrediv = 0x7F;

    if (i > 1)
        RTC_State = RTC_LSI_INIT_OK;

#elif defined(RTC_CLOCK_SOURCE_LSE) /* LSE used as RTC source clock */
    /* Enable the LSE OSC */
    RCC_LSEConfig(RCC_LSE_ON);

    /* Wait till LSE is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET && i > 1)
    {
        i--;
    }

    /* Select the RTC Clock Source */
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

    SynchPrediv = 0xFF;
    AsynchPrediv = 0x7F;

    if (i > 1)
        RTC_State = RTC_LSE_INIT_OK;
#else
#error "Please select the RTC Clock source inside the main.c file"
#endif /* RTC_CLOCK_SOURCE_LSI */

    /* Enable the RTC Clock */
    RCC_RTCCLKCmd(ENABLE);

    /* Wait for RTC APB registers synchronisation */
    RTC_WaitForSynchro();
}
/*******************************************************************************
 * Function Name  : counter_to_struct
 * Description    : populates time-struct based on counter-value
 * Input          : - counter-value (unit seconds, 0 -> 1.1.2000 00:00:00),
 *                  - Pointer to time-struct
 * Output         : time-struct gets populated, DST not taken into account here
 * Return         : none
 *  Based on code from Peter Dannegger found in the mikrocontroller.net forum.
 *******************************************************************************/
static void CounterToStruct(uint32_t sec, DateTime_t *t, uint8_t CalYear)
{
    uint16_t day;
    uint8_t year;
    uint16_t dayofyear;
    uint8_t leap400;
    uint8_t month;

    t->Second = sec % 60;
    sec /= 60;
    t->Minute = sec % 60;
    sec /= 60;
    t->Hour = sec % 24;

    if (CalYear == 0)
        return;

    day = (uint16_t)(sec / 24);

    year = FIRSTYEAR % 100;                    // 0..99
    leap400 = 4 - ((FIRSTYEAR - 1) / 100 & 3); // 4, 3, 2, 1

    for (;;)
    {
        dayofyear = 365;
        if ((year & 3) == 0)
        {
            dayofyear = 366; // leap year
            if (year == 0 || year == 100 || year == 200)
            {
                // 100 year exception
                if (--leap400)
                {
                    // 400 year exception
                    dayofyear = 365;
                }
            }
        }
        if (day < dayofyear)
        {
            break;
        }
        day -= dayofyear;
        year++; // 00..136 / 99..235
    }
    t->Year = year + FIRSTYEAR / 100 * 100 - 2000; // + century

    if (dayofyear & 1 && day > 58)
    {
        // no leap year and after 28.2.
        day++; // skip 29.2.
    }

    for (month = 1; day >= DaysInMonth[month - 1]; month++)
    {
        day -= DaysInMonth[month - 1];
    }

    t->Month = month; // 1..12
    t->Day = day + 1; // 1..31
}

/*******************************************************************************
 * Function Name  : struct_to_counter
 * Description    : calculates second-counter from populated time-struct
 * Input          : Pointer to time-struct
 * Output         : none
 * Return         : counter-value (unit seconds, 0 -> 1.1.2000 00:00:00),
 *  Based on code from "LalaDumm" found in the mikrocontroller.net forum.
 *******************************************************************************/
static uint32_t StructToCounter(DateTime_t *t)
{

    uint8_t i;
    uint32_t result = 0;
    uint16_t idx, year;

    year = t->Year + 2000;

    /* Calculate days of years before */
    result = (uint32_t)year * 365;
    if (t->Year >= 1)
    {
        result += (year + 3) / 4;
        result -= (year - 1) / 100;
        result += (year - 1) / 400;
    }

    /* Start with 2000 a.d. */
    result -= 730485UL;

    /* Make month an array index */
    idx = t->Month - 1;

    /* Loop thru each month, adding the days */
    for (i = 0; i < idx; i++)
    {
        result += DaysInMonth[i];
    }

    /* Leap year? adjust February */
    if (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0))
    {
        ;
    }
    else
    {
        if (t->Month > 2)
        {
            result--;
        }
    }

    /* Add remaining days */
    result += t->Day;

    /* Convert to seconds, add all the other stuff */
    result = (result - 1) * 86400L + (uint32_t)t->Hour * 3600 +
             (uint32_t)t->Minute * 60 + t->Second;

    return result;
}
/*****************************************************************************/
/**
  * @brief  Set date time for RTC.
  * @param  None
  * @retval None
  */
static void SetDateTime(DateTime_t Input, int32_t GMT_Adjust)
{
    uint8_t SetTimeOK = 1;
    uint32_t Counter;

    RTC_TimeTypeDef RTC_TimeStructure;
    RTC_DateTypeDef RTC_DateStructure;

    Counter = StructToCounter(&Input) + GMT_Adjust;
    CounterToStruct(Counter, &Input, 1);

    RTC_TimeStructure.RTC_H12 = RTC_H12_AM;
    RTC_TimeStructure.RTC_Hours = Input.Hour;
    RTC_TimeStructure.RTC_Minutes = Input.Minute;
    RTC_TimeStructure.RTC_Seconds = Input.Second;

    RTC_DateStructure.RTC_WeekDay = 1;
    RTC_DateStructure.RTC_Date = Input.Day;
    RTC_DateStructure.RTC_Month = Input.Month;
    RTC_DateStructure.RTC_Year = Input.Year;

    if (RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure) == ERROR)
    {
        SetTimeOK = 0;
    }

    if (RTC_SetDate(RTC_Format_BIN, &RTC_DateStructure) == ERROR)
    {
        SetTimeOK = 0;
    }

    if (SetTimeOK)
    {
        // DebugPrint("\rRTC : Cai dat thoi gian cho RTC thanh cong\r\n");
        RTC_WriteBackupRegister(RTC_BKP_DR0, 0x32F2);
    }
    else
    {
        DebugPrint("\rRTC : Loi cai dat thoi gian cho RTC\r\n");
    }
}

/*****************************************************************************/
/**
  * @brief  Set date time for RTC
  * @param  None
  * @retval None
  */
static void UpdateTimeFromServer(uint8_t *Buffer)
{
    DateTime_t DateTime;

    DateTime.Hour = Buffer[3];
    DateTime.Minute = Buffer[4];
    DateTime.Second = Buffer[5];

    DateTime.Day = Buffer[2];
    DateTime.Month = Buffer[1];
    DateTime.Year = Buffer[0];

    SetDateTime(DateTime, 0);
}

/*****************************************************************************/
/**
  * @brief  Cap nhat thoi gian vao bien tam.
  * @param  None
  * @retval None
  */
static void RTC_Tick(void)
{
    static uint32_t LastTimeReg = 0;

    if (LastTimeReg == RTC->TR)
        return;
    LastTimeReg = RTC->TR;

    RTC_GetTime(RTC_Format_BIN, &tmpRTC_TimeStructure);
    RTC_GetDate(RTC_Format_BIN, &tmpRTC_DateStructure);

    LastRtcTime.Day = tmpRTC_DateStructure.RTC_Date;
    LastRtcTime.Month = tmpRTC_DateStructure.RTC_Month;
    LastRtcTime.Year = tmpRTC_DateStructure.RTC_Year;
    LastRtcTime.Hour = tmpRTC_TimeStructure.RTC_Hours;
    LastRtcTime.Minute = tmpRTC_TimeStructure.RTC_Minutes;
    LastRtcTime.Second = tmpRTC_TimeStructure.RTC_Seconds;

    LastRtcCounter = LastRtcTime.Second + LastRtcTime.Minute * 60 +
                     LastRtcTime.Hour * 3600 + LastRtcTime.Day * 86400 +
                     LastRtcTime.Month * 2678400 + LastRtcTime.Year * 32140800;

    struct tm time;

    time.tm_year = LastRtcTime.Year;
    time.tm_year += 100;
    time.tm_mon = LastRtcTime.Month;
    time.tm_mon -= 1;

    time.tm_mday = LastRtcTime.Day;

    time.tm_hour = LastRtcTime.Hour;
    time.tm_min = LastRtcTime.Minute;
    time.tm_sec = LastRtcTime.Second;

    time_t from = mktime(&time);
    g_sys_time = from;
}

/*****************************************************************************/
/**
  * @brief  Lay bien dem ao trong RTC
  * @param  None
  * @retval None
  */
static uint32_t RTC_GetCounter(void)
{
    return LastRtcCounter;
}
/*****************************************************************************/
/**
  * @brief  Get date time from RTC.
  * @param  None
  * @retval None
  */
static DateTime_t GPS_GetDateTime(void)
{
    return LastRtcTime;
}

/*****************************************************************************/
/**
  * @brief  Kiem tra xem thoi gian truyen vao co hop le hay khong
  * @param  None
  * @retval 1 neu valid, 0 neu invalid
  */
uint8_t ValidDateTime(DateTime_t ThoiGian)
{
    if (ThoiGian.Year < 17 || ThoiGian.Year == 0)
        return 0;
    if (ThoiGian.Month > 12 || ThoiGian.Month == 0)
        return 0;
    if (ThoiGian.Day > 31 || ThoiGian.Day == 0)
        return 0;
    if (ThoiGian.Hour > 23)
        return 0;
    if (ThoiGian.Minute > 59)
        return 0;
    if (ThoiGian.Second > 59)
        return 0;

    return 1;
}

uint8_t RTC_GetState(void)
{
    return RTC_State;
}

RTC_PERIPHERAL Driver_RTC =
    {
        RTC_User_Init,
        SetDateTime,
        GPS_GetDateTime,
        UpdateTimeFromServer,
        RTC_GetCounter,
        RTC_Tick,
        ValidDateTime,
        RTC_GetState};

/********************************* END OF FILE *******************************/
