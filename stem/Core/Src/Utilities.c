/******************************************************************************
 * @file        Utilities.c
 * @author
 * @version     V1.0.0
 * @date        15/01/2014
 * @brief
 ******************************************************************************/

/******************************************************************************
                                   INCLUDES
 ******************************************************************************/
#include "Utilities.h"

/******************************************************************************
                                   GLOBAL VARIABLES
 ******************************************************************************/
extern System_t xSystem;
extern const char Version[];
uint8_t PublicBuffer[200];

/******************************************************************************
                                   GLOBAL FUNCTIONS
 ******************************************************************************/

/******************************************************************************
                                   DATA TYPE DEFINE
 ******************************************************************************/
#define ON 0
#define OFF 1

#define __NEW_BUZZER_MODE__ 1

/******************************************************************************
                                   PRIVATE VARIABLES
 ******************************************************************************/
static int8_t BeepCount = 0;
static uint32_t tmpBeepCount = 0;
static uint32_t BeepLength = 0;
#if __NEW_BUZZER_MODE__
uint32_t BeepCountLength = 540;
#else
uint32_t BeepCountLength = 10;
#endif
static uint8_t BuzzerOn = 0;
/******************************************************************************
                                   LOCAL FUNCTIONS
 ******************************************************************************/
static void ctrlUserBuzzer(uint8_t MODE);

/*****************************************************************************/
/**
 * @brief   :   TickBeep, tick every 10ms
 * @param   :
 * @retval  :
 * @author  :
 * @created :   15/01/2014
 * @version :
 * @reviewer:
 */

void TickBeep(void)
{
    if (BeepCount > 0)
    {
        if (tmpBeepCount++ >= BeepCountLength)
        {
            tmpBeepCount = 0;
            ctrlUserBuzzer(BeepCount % 2);

            BeepCount--;
            if (BeepCount == 0)
                ctrlUserBuzzer(OFF);
        }
    }
    else
    {
        if (BeepLength > 0)
        {
            BeepLength--;
            if (BeepLength == 0)
                ctrlUserBuzzer(OFF);
        }
    }
}

void Beep(uint16_t Length)
{
#if __NEW_BUZZER_MODE__
    BuzzerOn = 1;
    BeepCount = 1;
    BeepCountLength = 5400 * Length / 1000;
#else
    ctrlUserBuzzer(OFF);
    ctrlUserBuzzer(ON);
    BeepLength = Length;
#endif
}

/***********************************************************************************************************************************/

/* Keu nhieu tieng */
void Beeps(uint8_t Count)
{
#if __NEW_BUZZER_MODE__
    BuzzerOn = 1;
    BeepCountLength = 540;
    BeepCount = Count + (Count - 1);
#else
    ctrlUserBuzzer(ON);
    BeepCount = Count + (Count - 1);
#endif
}

void BeepKeypad(void)
{
    xSystem.CTP.CTP_Sens = xSystem.CTP.CTP_Sens_DF;
    Beep(10);
}

static void ctrlUserBuzzer(uint8_t MODE)
{

    if (MODE == ON)
    {
        TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
        TIM_Cmd(TIM2, ENABLE);
    }
    else
    {
        TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
        TIM_Cmd(TIM2, DISABLE);

        GPIO_WriteBit(BUZZER_PORT, BUZZER_PIN, Bit_RESET);
    }
}

void TIM3_IRQHandler(void) /* Ngat 1/5700s */ //Thay doi co bao ghi du lieu
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
    {

        if (BeepCount % 2 && BeepCount > 0 && BuzzerOn == 1)
        {

            if (tmpBeepCount++ >= BeepCountLength)
            {
                BeepCount--;
                tmpBeepCount = 0;
            }
            else
            {

                GPIO_WriteBit(BUZZER_PORT, BUZZER_PIN, (BitAction)(1 - GPIO_ReadOutputDataBit(BUZZER_PORT, BUZZER_PIN)));
            }
        }
        else
        {

            GPIO_ResetBits(BUZZER_PORT, BUZZER_PIN);
            if (tmpBeepCount++ >= BeepCountLength)
            {
                BeepCount--;
                tmpBeepCount = 0;
            }
            if (BeepCount <= 0)
            {
                BuzzerOn = 0;
                BeepCount = 0;
                tmpBeepCount = 0;
            }
        }

        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
    }
}

uint8_t CopyParameter(char *BufferSource, char *BufferDes, char FindCharBegin, char FindCharEnd)
{
    int16_t ViTriBatDau = FindIndexOfChar(FindCharBegin, BufferSource);
    int16_t ViTriKetThuc = FindIndexOfChar(FindCharEnd, BufferSource);
    int16_t tmpCount, i = 0;

    /* Kiem tra dinh dang du lieu */
    if (ViTriBatDau == -1 || ViTriKetThuc == -1)
        return 0;
    if (ViTriKetThuc <= 1 + ViTriBatDau)
        return 0;
    if (ViTriKetThuc >= 199 + ViTriBatDau)
        return 0;

    for (tmpCount = ViTriBatDau + 1; tmpCount < ViTriKetThuc; tmpCount++)
    {
        BufferDes[i++] = BufferSource[tmpCount];
    }

    BufferDes[i] = 0;

    return 1;
}

int16_t FindIndexOfChar(char CharToFind, char *BufferToFind)
{
    uint8_t tmpCount = 0;
    uint16_t DoDai = 0;

    /* Do dai du lieu */
    DoDai = strlen(BufferToFind);

    for (tmpCount = 0; tmpCount < DoDai; tmpCount++)
    {
        if (BufferToFind[tmpCount] == CharToFind)
            return tmpCount;
    }
    return -1;
}
/******************************************************************************************************************/
uint32_t GetHexNumberFromString(uint16_t BeginAddress, char *Buffer)
{
    uint32_t Value = 0;
    uint16_t tmpCount = 0;

    tmpCount = BeginAddress;
    Value = 0;
    while (Buffer[tmpCount] && tmpCount < 1024)
    {
        if ((Buffer[tmpCount] >= '0' && Buffer[tmpCount] <= '9') || (Buffer[tmpCount] >= 'A' && Buffer[tmpCount] <= 'F') || (Buffer[tmpCount] >= 'a' && Buffer[tmpCount] <= 'f'))
        {
            Value *= 16;

            if (Buffer[tmpCount] == '1')
                Value += 1;
            if (Buffer[tmpCount] == '2')
                Value += 2;
            if (Buffer[tmpCount] == '3')
                Value += 3;
            if (Buffer[tmpCount] == '4')
                Value += 4;
            if (Buffer[tmpCount] == '5')
                Value += 5;
            if (Buffer[tmpCount] == '6')
                Value += 6;
            if (Buffer[tmpCount] == '7')
                Value += 7;
            if (Buffer[tmpCount] == '8')
                Value += 8;
            if (Buffer[tmpCount] == '9')
                Value += 9;

            if (Buffer[tmpCount] == 'A' || Buffer[tmpCount] == 'a')
                Value += 10;
            if (Buffer[tmpCount] == 'B' || Buffer[tmpCount] == 'b')
                Value += 11;
            if (Buffer[tmpCount] == 'C' || Buffer[tmpCount] == 'c')
                Value += 12;
            if (Buffer[tmpCount] == 'D' || Buffer[tmpCount] == 'd')
                Value += 13;
            if (Buffer[tmpCount] == 'E' || Buffer[tmpCount] == 'e')
                Value += 14;
            if (Buffer[tmpCount] == 'F' || Buffer[tmpCount] == 'f')
                Value += 15;
        }
        else
            break;

        tmpCount++;
    }

    return Value;
}
/******************************************************************************************************************/

/*
 *  Ham doc mot so trong chuoi bat dau tu dia chi nao do.
 *  Buffer = abc124mff thi GetNumberFromString(3,Buffer) = 123
 *
 */
uint32_t GetNumberFromString(uint16_t BeginAddress, char *Buffer)
{
    uint32_t Value = 0;
    uint16_t tmpCount = 0;

    tmpCount = BeginAddress;
    Value = 0;
    while (Buffer[tmpCount] && tmpCount < 1024)
    {
        if (Buffer[tmpCount] >= '0' && Buffer[tmpCount] <= '9')
        {
            Value *= 10;

            Value += Buffer[tmpCount] - 48;
        }

        tmpCount++;
    }

    return Value;
}

void TimeToString(DateTime_t *Time, char *String)
{
    sprintf(String, "%02d:%02d:%02d", Time->Hour, Time->Minute, Time->Second);
}

void DateToString(DateTime_t *Time, char *String)
{
    sprintf(String, "%02d/%02d/%02d", Time->Day, Time->Month, Time->Year);
}

void DateTimeToString(DateTime_t *Time, char *String)
{
    sprintf(String, "%02d:%02d:%02d-%02d/%02d/%02d", Time->Hour, Time->Minute, Time->Second, Time->Day, Time->Month, Time->Year);
}

/***************************************************************************************************************************/

/*
 *  Kiem tra ma bao mat
 *
 */
static uint16_t MaskSecureArray[] = {0, 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768};

uint8_t ValidSecureCode(char *BufferData)
{
    char *BufferCode;
    uint32_t SecureCode = 0;
    uint8_t MaskID = 0;

    BufferCode = strstr(BufferData, "),");
    if (!BufferCode)
        return 0;

    SecureCode = GetNumberFromString(2, BufferCode);
    DebugPrint("\rMa bao mat: %d\r\n", SecureCode);

    if (xSystem.Rtc->GetDateTime().Year < 17)
        return 0;

    /* Ngay */
    MaskID = xSystem.Rtc->GetDateTime().Day;
    MaskID = MaskID % xSystem.Rtc->GetDateTime().Month;
    if (MaskID == 0)
        MaskID = 1;
    if ((SecureCode & MaskSecureArray[MaskID]) == 0)
        return 0;

    /* Thang */
    MaskID = xSystem.Rtc->GetDateTime().Month;
    if (MaskID == 0)
        MaskID = 1;
    if ((SecureCode & MaskSecureArray[MaskID]) == 0)
        return 0;

    /* Gio */
    MaskID = xSystem.Rtc->GetDateTime().Hour;
    MaskID = MaskID % xSystem.Rtc->GetDateTime().Month;
    if (MaskID == 0)
        MaskID = 1;
    if ((SecureCode & MaskSecureArray[MaskID]) == 0)
        return 0;

    return 1;
}
/***************************************************************************************************************************/

/*
 *  Tinh check sum trong mot mang buffer
 *
 */
uint16_t CalculateCheckSum(uint8_t *Buffer, uint16_t BeginAddress, uint16_t Length)
{
    uint32_t tempChecksum = 0;
    uint16_t i = 0;

    for (i = BeginAddress; i < BeginAddress + Length; i++)
        tempChecksum += Buffer[i];

    return (uint16_t)tempChecksum;
}

/***************************************************************************************************************************/

/*
 *  Tinh check sum CRC 16
 *
 */
#define ISO15693_PRELOADCRC16 0xFFFF
#define ISO15693_POLYCRC16 0x8408
#define ISO15693_MASKCRC16 0x0001
#define ISO15693_RESIDUECRC16 0xF0B8

uint16_t CalculateChecksumCRC16(uint8_t *DataIn, uint8_t NbByte)
{
    int16_t i, j;
    int32_t ResCrc = ISO15693_PRELOADCRC16;

    for (i = 0; i < NbByte; i++)
    {
        ResCrc = ResCrc ^ DataIn[i];
        for (j = 8; j > 0; j--)
        {
            ResCrc = (ResCrc & ISO15693_MASKCRC16) ? (ResCrc >> 1) ^ ISO15693_POLYCRC16 : (ResCrc >> 1);
        }
    }

    return ((~ResCrc) & 0xFFFF);
}

/***************************************************************************************************************************/
/*
 *  Lay thoi gian tu buffer
 *
 */
DateTime_t GetTimeFromBuffer(char *Buffer, uint8_t Begin)
{
    DateTime_t CmdDateTime;

    CmdDateTime.Hour = 0;
    CmdDateTime.Minute = 0;
    CmdDateTime.Second = 0;
    CmdDateTime.Day = 0;
    CmdDateTime.Month = 0;
    CmdDateTime.Year = 0;

    if (Buffer[Begin] < '0' || Buffer[Begin + 1] > '9')
        return CmdDateTime;
    CmdDateTime.Day = (Buffer[Begin] - 48) * 10 + (Buffer[Begin + 1] - 48);
    Begin += 2;

    if (Buffer[Begin] < '0' || Buffer[Begin + 1] > '9')
        return CmdDateTime;
    CmdDateTime.Month = (Buffer[Begin] - 48) * 10 + (Buffer[Begin + 1] - 48);
    Begin += 2;

    if (Buffer[Begin] < '0' || Buffer[Begin + 1] > '9')
        return CmdDateTime;
    CmdDateTime.Year = (Buffer[Begin] - 48) * 10 + (Buffer[Begin + 1] - 48);
    Begin += 2;

    if (Buffer[Begin] < '0' || Buffer[Begin + 1] > '9')
        return CmdDateTime;
    CmdDateTime.Hour = (Buffer[Begin] - 48) * 10 + (Buffer[Begin + 1] - 48);
    Begin += 2;

    if (Buffer[Begin] < '0' || Buffer[Begin + 1] > '9')
        return CmdDateTime;
    CmdDateTime.Minute = (Buffer[Begin] - 48) * 10 + (Buffer[Begin + 1] - 48);
    Begin += 2;

    if (Buffer[Begin] < '0' || Buffer[Begin + 1] > '9')
        return CmdDateTime;
    CmdDateTime.Second = (Buffer[Begin] - 48) * 10 + (Buffer[Begin + 1] - 48);
    Begin += 2;

    if (CmdDateTime.Year < 12 || CmdDateTime.Month > 12 || CmdDateTime.Day > 31)
        return CmdDateTime;
    if (CmdDateTime.Hour > 23 || CmdDateTime.Minute > 60 || CmdDateTime.Second > 60)
        return CmdDateTime;

    return CmdDateTime;
}

/****************************************************************************************************************************/
void PrintHexStr(uint8_t *Buffer, uint16_t Length, char *UserStr)
{
    uint16_t i;

    DebugPrint("\r%s (%u): \r\n", UserStr, Length);
    for (i = 0; i < Length; i++)
        DebugPrint("%02X ", Buffer[i]);
}

char tmpUtilitiesBuffer[501];
char *ConvBufferToStr(uint8_t *Buffer, uint8_t Length)
{
    uint16_t i, Index = 0;
    if (Length > 250)
        Length = 250;

    memset(tmpUtilitiesBuffer, 0, 501);
    for (i = 0; i < Length; i++)
    {
        sprintf(&tmpUtilitiesBuffer[Index], "%02X ", Buffer[i]);
        Index += 3;
    }

    return tmpUtilitiesBuffer;
}

uint32_t CalcABS(uint32_t Value1, uint32_t Value2)
{
    if (Value1 > Value2)
        return (Value1 - Value2);
    else
        return (Value2 - Value1);
}

void SystemReset(uint8_t NguyenNhanReset)
{

    NVIC_SystemReset();
}

/********************************* END OF FILE *******************************/
