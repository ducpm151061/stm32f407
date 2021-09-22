#include <stdint.h>
#include "DataDefine.h"
#include "BlowDetect.h"
#include "UserGuiService.h"

extern __IO uint16_t ADC_RegularConvertedValueTab[6];

#define BLOW_DETECT_MAX_VALUE 3900 // Need testing \
// Actual > MIN + STEP*Setup level
#define BLOW_DETECT_MIN_VALUE 800  //1300   // Need testing

#define BLOW_DETECT_THRESHOLD_BASE 900 //250//2000 // Save as Parametter
#define BLOW_DETECT_THRESHOLD_CNT 100
#define BLOW_DETECT_THRESHOLD_STEP 100 // Need testing
#define NbOfSamplePerTime 5

typedef struct BlowDectect
{
    uint16_t BlowDetectSampleBuffer[NbOfSamplePerTime];
    uint8_t BlowDetectSampleIndex;
    uint8_t BlowDetectGetEnough100Sample;
    uint8_t BlowThresholdCnt;
    uint32_t startedTime;
    uint32_t blowFinish;
} BlowDectect_t;

static void BlowDetect_UpdateProgessBar(uint8_t BlowCnt);
static BlowDectect_t blowDetect;

uint8_t BlowDetect_GetCount(void)
{
    return blowDetect.BlowThresholdCnt;
}

uint16_t BlowDetect_GetOneSample(void)
{
    uint32_t sample_sum = 0;
    uint16_t ADC_Press = 0;

    /*Check valid ADC value*/
    ADC_Press = ADC_RegularConvertedValueTab[PRESSURE_SENSOR];
    //DebugPrint("\rBlowDetect: ADC value: %d\r\n", ADC_Press);
    if (ADC_Press < BLOW_DETECT_MAX_VALUE && ADC_Press > BLOW_DETECT_MIN_VALUE)
    {
        /*Get 100 sample in window*/
        blowDetect.BlowDetectSampleBuffer[blowDetect.BlowDetectSampleIndex++] = ADC_Press;
        if (blowDetect.BlowDetectSampleIndex == NbOfSamplePerTime)
        {
            blowDetect.BlowDetectSampleIndex = 0;
            blowDetect.BlowDetectGetEnough100Sample = 1;
        }
    }
    else
        return 0xFFFF;

    /*Calculate medium*/
    if (blowDetect.BlowDetectGetEnough100Sample)
    {
        blowDetect.BlowDetectGetEnough100Sample = 0;
        for (uint8_t i = 0; i < NbOfSamplePerTime; i++)
            sample_sum += blowDetect.BlowDetectSampleBuffer[i];
        return sample_sum / NbOfSamplePerTime;
    }
    return 0xFFFF;
}

/*Run in tick*/

uint8_t BlowDetect_BlowDetected(void)
{
    uint16_t tmpValue;

    if (blowDetect.blowFinish)
    {
        if (xSystem.TickCount1Ms - blowDetect.startedTime >= 1500)
        {
            return 1;
        }
        // Waiting for enought 2 seconds
        return 0;
    }
    tmpValue = BlowDetect_GetOneSample();
    if (tmpValue == 0xFFFF)
        return 0;

    //  DebugPrint("\rBlowDetect: Medium sample: %d\r\n", tmpValue);

    if (tmpValue > (BLOW_DETECT_THRESHOLD_BASE + BLOW_DETECT_THRESHOLD_STEP * xSystem.Parameters.BlowPressure))
    {
        DebugPrint("\rBlow: %d\r\n", tmpValue);
        Beep(4);
        blowDetect.BlowThresholdCnt += (6 - xSystem.Parameters.BlowTime);
        // Mark first time
        if (blowDetect.startedTime == 0)
        {
            blowDetect.startedTime = xSystem.TickCount1Ms;
        }
    }
    else
    {
        blowDetect.BlowThresholdCnt = 0;
        blowDetect.startedTime = 0;
    }
    BlowDetect_UpdateProgessBar(blowDetect.BlowThresholdCnt);
    if (blowDetect.BlowThresholdCnt >= BLOW_DETECT_THRESHOLD_CNT)
    {
        blowDetect.blowFinish = 1;
    }
    return 0;
}

void BlowDetect_Clear(void)
{
    memset(&blowDetect, 0, sizeof(blowDetect));
}

void BlowDetect_UpdateProgessBar(uint8_t BlowCnt)
{
    //if (xSystem.GLStatus.Al_Step == MEASURING) {
    switch (xSystem.GUI_Manager.CurrentWindowID)
    {
    case WD_DO_CHU_DONG:
        DoChuDong_UpdateProgessBar(BlowCnt);
        break;
    case WD_DOADC:
        DoADC_UpdateProgessBar(BlowCnt);
        break;
    default:
        break;
    }
    GUI_Exec();
}
//}
