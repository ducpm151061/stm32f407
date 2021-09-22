/******************************************************************************
 * @file        Peripheral.c
 * @author
 * @version     V1.0.0
 * @date
 * @brief
 ******************************************************************************/

/******************************************************************************
                                   INCLUDES
 ******************************************************************************/
#include "DataDefine.h"
#include "SimpleKalmanFilter.h"

/******************************************************************************
                                   GLOBAL VARIABLES
 ******************************************************************************/

/******************************************************************************
                                   DATA TYPE DEFINE
 ******************************************************************************/
#define MAX_ADC 4095
#define VREF 3280
#define K 11

/******************************************************************************
                                   PRIVATE VARIABLES
 ******************************************************************************/

extern __IO uint16_t ADC_RegularConvertedValueTab[7];

kalman k;
static uint16_t Get_ADC_Average_Value(uint16_t times);
/******************************************************************************
                                   LOCAL FUNCTIONS
 ******************************************************************************/

const uint8_t Vaccurate = 10; //mV
// const uint16_t VolToPercent[]={
//  7000,0,
//  7200,10,
//  7300,20,
//  7400,30,
//  7600,50,
//  7800,60,
//  8000,70,
//  8100,80,
//  8200,90,
//  8300,100,
// };

/* Tang gia tri len 20 khoang */
const uint16_t VolToPercent[] =
    {
        6800,
        0,
        6880,
        5,
        6960,
        10,
        7040,
        15,
        7120,
        20,
        7200,
        25,
        7280,
        30,
        7360,
        35,
        7440,
        40,
        7520,
        45,
        7600,
        50,
        7680,
        55,
        7760,
        60,
        7840,
        65,
        7920,
        70,
        8000,
        75,
        8080,
        80,
        8160,
        85,
        8240,
        90,
        8320,
        95,
        8400,
        100,
};
/*
0 -0
300 -10
500 - 20
800 - 30
1000 - 50
1200 - 60
1300 - 70
1400 - 80
1500 - 90
1600 - 100

*/

/*****************************************************************************/
/**
 * @brief   :
 * @param   :
 * @retval  :
 * @author  :
 * @created :
 * @comment :
 */

uint16_t GetV_Bat(void)
{
    uint32_t Temp;
    Temp = ADC_RegularConvertedValueTab[BATTERY] * VREF * K / MAX_ADC;
    return Temp & 0xFFFF;
}

uint16_t GetVBat(void)
{
    uint32_t Temp;
    Temp = ADC_RegularConvertedValueTab[BATTERY];
    return Temp & 0xFFFF;
}

uint16_t GetVIn(void)
{
    uint32_t Temp;
    Temp = ADC_RegularConvertedValueTab[VIN];
    return Temp & 0xFFFF;
}

uint16_t GetTempPrinter(void)
{
    uint32_t Temp;
    Temp = ADC_RegularConvertedValueTab[6];
    return Temp & 0xFFFF;
}

uint16_t GetTempSensor(void)
{
    uint32_t Temp;
    Temp = ADC_RegularConvertedValueTab[PRESSURE_SENSOR];
    return Temp & 0xFFFF;
}

uint16_t GetPressure(void)
{
    uint32_t Temp;
    Temp = ADC_RegularConvertedValueTab[PRESSURE_SENSOR];
    return Temp & 0xFFFF;
}

uint16_t GetVoltageIn(void)
{
    uint32_t Temp;
    Temp = ADC_RegularConvertedValueTab[VIN] * VREF * K / MAX_ADC;
    return Temp & 0xFFFF;
}

uint16_t GetPercent_Bat(void)
{
    uint8_t i;
    uint8_t buff[50];
    uint32_t ratio;
    uint16_t Value;
    //uint32_t mean = 0;
    /* for (int j = 0; j < 10; j++)
    {
        // DelayMS(10);
        mean += GetV_Bat();
        sprintf(buff,"value: %i %u\r\n",j,mean);
        DebugPrint(buff);
    }
    Value=mean/10;
    mean=0; */
    Value = Get_ADC_Average_Value(1000);
    sprintf((char *)buff, "value: %u\r\n", Value);
    //DebugPrint(buff);

    if (Value >= VolToPercent[20 * 2])
        return 100;
    else if (Value < VolToPercent[0 * 2])
        return 0;
    else
    {
        for (i = 0; i < 21; i++)
        {
            if (Value == VolToPercent[i * 2])
                return VolToPercent[i * 2 + 1];
            else if (Value > VolToPercent[i * 2] && Value < VolToPercent[i * 2 + 2])
            {
                ratio = (Value - VolToPercent[i * 2]) * 100 / (VolToPercent[i * 2 + 2] - VolToPercent[i * 2]); // =>% in range
                ratio = ratio * (VolToPercent[i * 2 + 2 + 1] - VolToPercent[i * 2 + 1]) / 100;                 // => % all range
                Value = VolToPercent[i * 2 + 1] + ((uint8_t)ratio % 100);
                return Value;
            }
        }
    }
    return 0;
}

uint16_t GetV_8V2(void)
{
    uint32_t Temp;
    Temp = ADC_RegularConvertedValueTab[V8_2] * VREF * K / MAX_ADC;
    return Temp & 0xFFFF;
}

const uint8_t Temp0C[] =
    {
        40,
        39,
        38,
        37,
        36,
        35,
        34,
        33,
        32,
        31,
        30,
        29,
        28,
        27,
        26,
        25,
        24,
        23,
        22,
        21,
        20,
        19,
        18,
        17,
        16,
        15,
        14,
        13,
        12,
        11,
        10,
        9,
        8,
        7,
        6,
        5,
        4,
        3,
        2,
        1,
        0,
        1,
        2,
        3,
        4,
        5,
        6,
        7,
        8,
        9,
        10,
        11,
        12,
        13,
        14,
        15,
        16,
        17,
        18,
        19,
        20,
        21,
        22,
        23,
        24,
        25,
        26,
        27,
        28,
        29,
        30,
        31,
        32,
        33,
        34,
        35,
        36,
        37,
        38,
        39,
        40,
        41,
        42,
        43,
        44,
        45,
        46,
        47,
        48,
        49,
        50,
        51,
        52,
        53,
        54,
        55,
        56,
        57,
        58,
        59,
        60,
        61,
        62,
        63,
        64,
        65,
        66,
        67,
        68,
        69,
        70,
        71,
        72,
        73,
        74,
        75,
        76,
        77,
        78,
        79,
        80,
        81,
        82,
        83,
        84,
        85,
        86,
        87,
        88,
        89,
        90,
        91,
        92,
        93,
        94,
        95,
        96,
        97,
        98,
        99,
        100,
        101,
        102,
        103,
        104,
        105,
};
const uint16_t TempRes[] =
    {
        /*-4x*/ 34092,
        31887,
        29839,
        27936,
        26167,
        24522,
        22990,
        21564,
        20236,
        18998,
        /*-3x*/ 17844,
        16767,
        15762,
        14824,
        13948,
        13128,
        12362,
        11646,
        10976,
        10348,
        /*-2x*/ 9760,
        9209,
        8693,
        8208,
        7754,
        7327,
        6927,
        6551,
        6198,
        5865,
        /*-1x*/ 5553,
        5259,
        4982,
        4722,
        4477,
        4246,
        4028,
        3823,
        3629,
        3446,
        /*0x*/ 3274,
        3111,
        2957,
        2812,
        2674,
        2545,
        2422,
        2306,
        2196,
        2092,
        /*1x*/ 1993,
        1900,
        1811,
        1728,
        1648,
        1573,
        1501,
        1433,
        1369,
        1308,
        /*2x*/ 1250,
        1194,
        1142,
        1092,
        1045,
        1000,
        957,
        916,
        877,
        840,
        /*3x*/ 805,
        771,
        739,
        709,
        679,
        652,
        625,
        600,
        576,
        553,
        /*4x*/ 531,
        510,
        490,
        471,
        453,
        435,
        418,
        402,
        387,
        372,
        /*5x*/ 358,
        345,
        332,
        320,
        308,
        297,
        286,
        276,
        266,
        256,
        /*6x*/ 247,
        238,
        230,
        222,
        214,
        207,
        199,
        193,
        186,
        180,
        /*7x*/ 174,
        168,
        162,
        157,
        152,
        147,
        142,
        137,
        133,
        128,
        /*8x*/ 124,
        120,
        116,
        113,
        109,
        106,
        102,
        99,
        96,
        93,
        /*9x*/ 90,
        88,
        85,
        82,
        80,
        78,
        75,
        73,
        71,
        69,
        /*10x*/ 67,
        65,
        63,
        61,
        59,
        58,
};

#define ADC_MaxValue 4095 // 1024/10 ~ 10 bit
#define R_Pullup_x10 1000 //10kOhm * 100 (13k3)

int8_t AdcToTemp(uint16_t AdcValue)
{
    float NTC = 0.0;
    int8_t Temperature;
    uint16_t i;
    if (AdcValue > 10)
        NTC = ((float)AdcValue * (float)R_Pullup_x10) / (ADC_MaxValue - AdcValue);
    //else NTC=280;
    else
        NTC = 3274;

    for (i = 0; i < sizeof(TempRes) - 1; i++)
        if (((uint16_t)NTC <= TempRes[i]) && ((uint16_t)NTC > TempRes[i + 1]))
            break;

    Temperature = Temp0C[i];
    if (i < 40)
        Temperature *= -1;
    if (Temperature < 0)
        Temperature = 0;
    if (Temperature > 100)
        Temperature = 100;

    //  DebugPrint("\rADC: %d NTC:%d T:%d",AdcValue,(uint16_t)(NTC*10),Temperature);
    return Temperature;
}

int8_t GetTPRTTemp(void)
{
    return AdcToTemp(ADC_RegularConvertedValueTab[6]);
}

int16_t GetVCCAirPump(void)
{
    return AdcToTemp(ADC_RegularConvertedValueTab[VCC_AIR_PUMP]);
}

int16_t GetPumpTemp(void)
{
    kalman_init(&k);
    int16_t val = 0;
    for (int i = 0; i < 50; i++)
    {
        //DelayMS(2);
        val += kalman_update(&k, (float)(ADC_RegularConvertedValueTab[TEMPERATURE_SENSOR]) - 600);
    }
    return (int16_t)val / 500;
}

/* Lay trung binh gia tri ADC sau khi cho qua thuat toan kalman */
static uint16_t Get_ADC_Average_Value(uint16_t times)
{
    //return GetV_Bat();
    uint32_t temp_val = 0;
    uint16_t t;
    kalman_init(&k);

    for (t = 0; t < times; t++)
    {
        temp_val += kalman_update(&k, GetV_Bat());
        //DelayMS(5);
    }
    return temp_val / times;
}

/********************************* END OF FILE *******************************/
