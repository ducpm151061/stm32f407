#include "AlcoholMeter.h"
#include "UserGUIService.h"
#include "SimpleKalmanFilter.h"
#include "AirPump.h"
#include "BlowDetect.h"
#include "ADS7822.h"

extern System_t xSystem;

void DoADC_UpdatePeak(uint32_t peak);
static void AlcoholMetter_InitTimer(void);
static void Alcohol_Measuring(void);
static void AlcoholMetter_StartMeasure(void);

static void AlM_PumpOn(void);
static void AlM_PumpOff(void);
static void AlM_DoChuDong_Tick(void);
static void AlM_DO_Tick(void);
static void AlM_XA_Tick(void);
static void TestChart();

static uint8_t AP_check = 0;

static void getTimeBeforeMeasuring(void);

static uint16_t Al_ConvertAdcToMgL(uint16_t ADCValue);

volatile uint8_t RequestGetSample = 0;

AlcoholManage_t AlMManage;
PeakData_t peakData;
kalman km_zero_point;
kalman km_normal;

static int isInitPressure = 0;
static int isOutputForChart = 0;

#define MAX_PEAK_BUFFER_SIZE 10
#define USE_DYNAMIC_PEAK_BUFFER 1
#define RAW_SAMPLE_BUFFER_SIZE 3000
typedef struct AlcoholMeter
{

    uint16_t Alcohol_RawSampleBuffer[RAW_SAMPLE_BUFFER_SIZE];
    uint16_t Alcohol_FIRSampleBuffer[2900];
    uint16_t RawIndex;
    uint16_t FIRIndex;

    uint16_t ZeroPeak;    // Avg value, may be top peak
    uint16_t ZeroPeakTmp; // Avg value, may be top peak
    uint16_t ZeroTopPeak; // top of zero point
    uint16_t PeakValue;
    uint16_t PeakIndex;
    uint16_t PeakBuff[MAX_PEAK_BUFFER_SIZE];
    uint16_t tmpPeakValue;
    uint16_t oldTmpPeakValue;

    uint16_t CurrentValue, LastValue;
    uint8_t AlcoholMeasureDone;
    uint8_t isBlowDetected;

    uint8_t pumpStatus;
    uint8_t GraphTrend;
    uint16_t UptrendCnt;
    uint16_t DowntrendCnt;

    uint32_t CleanSensor_CleanTick;
    //uint32_t IntakeSensorTick;

    uint32_t InvalidDataClk;
    uint32_t rawSampleCount;
    uint32_t startTimeTick;
    uint32_t startMeasureTick;
    uint32_t pumpTimeTick;
    uint8_t mode;
} AlcoholMeter_t;

static AlcoholMeter_t alcoholMeter;

const uint8_t Time_CoilPwr = 20;
const uint16_t INVALID_VALUE = 0;

const uint16_t SENSOR_INIT_TIME1 = 30;
const uint16_t SENSOR_INIT_TIME2 = 50;
const int32_t TIME_TICK_10MS = 10;

/*
    is Called every 10ms
*/

void AlM_SetTask(ALM_Task task)
{
    AlMManage.Task = task;
    RequestGetSample = 1;
    AlM_ChangeStage(SENSOR_IDLE);
    DebugPrint("\rAlM_SetTask:%d\r\n", task);
}

void AlM_ChangeStage(uint8_t Stage)
{
    AlMManage.Stage = Stage;
    AlMManage.Step = 0;
}

void AlcoholMetter_Init(void)
{
    ADS7822_Init();
    AlcoholMetter_InitTimer();
}

void AM_Clear(void)
{
    memset(&alcoholMeter, 0, sizeof(alcoholMeter));
    memset(&peakData, 0, sizeof(peakData));
    kalman_init(&km_zero_point);
    kalman_init(&km_normal);
    isOutputForChart = 0;
}

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

// This function was called every 10ms
void AlcoholMetter_Tick(void)
{
    // TestChart();

    //if (RequestGetSample == 0)
    //  return;
    //RequestGetSample = 0;

    switch (AlMManage.Task)
    {
    case ALM_IDLE:
        break;
    case ALM_DOCHUDONG:
        AlM_DoChuDong_Tick();
        break;
    case ALM_DOTHUDONG:
        AlM_DoChuDong_Tick();
        break;
    case ALM_DOADC:
        AlM_DoChuDong_Tick();
        break;
    case ALM_DO:
        AlM_DoChuDong_Tick();
        break;
    case ALM_XA:
        //AlM_XA_Tick();
        break;
    case ALM_LISTRECORD:
        break;
    case ALM_SETUP:
        break;
    }
}

/*
Lam sach cam bien, gom cac step:
Step=1: Dua pump ve vi tri ban dau trong 30ms
Step=2: Pull pump for 30ms
Step=3: Push pump for 30ms
Step=4: Khoi tao kalman data
*/
static void SensorClean()
{
    /*Clean sensor*/
    if (AlMManage.Step == 0)
    {
        if (AlMManage.Task != ALM_DOADC)
        {
            GUI_WD_Change(WD_KHOITAO_CAMBIEN);
        }
        AlMManage.Step++;
        AP_Ctrl_Push(1);
    }
    /*Thoi khong khi ra khoi sensor 5s*/
    //diffMs = xSystem.TickCount1Ms-alcoholMeter.startTimeTick;

    //DebugPrint("uptime:%d %d \n", xSystem.TickCount1Ms, CleanSensor_CleanTick);
    if (alcoholMeter.CleanSensor_CleanTick == 2 * TIME_TICK_10MS)
    {
        DebugPrint("%d 2: push to clean sensor\r\n", xSystem.TickCount1Ms);
        //AP_Ctrl_Push(1);
    }
    else if (alcoholMeter.CleanSensor_CleanTick <= SENSOR_INIT_TIME1 * TIME_TICK_10MS)
    {
        // Keo khong khi vao sensor sau khi clean buong khi 10ms
        if (alcoholMeter.CleanSensor_CleanTick == SENSOR_INIT_TIME1 * TIME_TICK_10MS)
        {
            DebugPrint("%d pull (for seeking zero point)\r\n", xSystem.TickCount1Ms);
            // Do thu dong, khong can pull de do Zero point
            if (AlMManage.Task != ALM_DOTHUDONG)
                AP_Ctrl_Pull(1);
        }
    }
    else
    {
        // Chuyen sang buoc do thu sau khi push 30ms, do not pull here
        if (alcoholMeter.CleanSensor_CleanTick == 2 * SENSOR_INIT_TIME1 * TIME_TICK_10MS)
        {
            if (AlMManage.Task != ALM_DOTHUDONG)
                AP_Ctrl_Push(1);
            DebugPrint("%d push (for seeking zero point) after %d\r\n", xSystem.TickCount1Ms, alcoholMeter.CleanSensor_CleanTick);
        }
        if (alcoholMeter.CleanSensor_CleanTick == 3 * SENSOR_INIT_TIME1 * TIME_TICK_10MS)
        {
            DebugPrint("%d Switch state to MEASURE_ZERO_POINT, after:%d, rawCnt:%d, invalid:%d, idx:%d\r\n", xSystem.TickCount1Ms,
                       alcoholMeter.CleanSensor_CleanTick, alcoholMeter.rawSampleCount, alcoholMeter.InvalidDataClk, alcoholMeter.RawIndex);

            AlM_ChangeStage(MEASURE_ZERO_POINT);
            kalman_init(&km_zero_point);
            kalman_init(&km_normal);
        }
    }
}

/*
Step 1: Pull khong khi vao sensor trong 30ms
Step 2: Tien hanh lay mau 3s
Param:
        base: Do zero point or normal
*/
static void SensorMeasure(int base)
{
    /*Hut khong khi vao sensor*/
    uint8_t Step = AlMManage.Step;
    switch (Step)
    {
    case 0:
    {
        if (!base)
        {
            DebugPrint("%d MEASURE_SAMPLE, pull \r\n", xSystem.TickCount1Ms);
            AP_Ctrl_Pull(1);
        }
        AlMManage.Step = 1;
        xSystem.GLStatus.Al_Step = NO_MEASURE;
        alcoholMeter.startMeasureTick = xSystem.TickCount1Ms;
    }
    break;
    /*Vua hut vua do HOAC hut xong do ????*/
    /*1. Chon hut xong do*/
    case 1:
    {
        if (xSystem.GLStatus.Al_Step == NO_MEASURE)
        {
            DebugPrint("time 1: %02d:%02d:%02d \r\n",
                       xSystem.Rtc->GetDateTime().Hour,
                       xSystem.Rtc->GetDateTime().Minute,
                       xSystem.Rtc->GetDateTime().Second);
            AlcoholMetter_StartMeasure();
        }
        if (xSystem.GLStatus.Al_Step == MEASURING)
        {
            if (xSystem.TickCount1Ms % 100 == 0)
            {
            }
            if ((xSystem.TickCount1Ms - alcoholMeter.startMeasureTick) < 50)
            {
                // Waiting for 50ms after pull air to sensor
                break;
            }
            Alcohol_Measuring();
        }
        if (xSystem.GLStatus.Al_Step == MEASURE_DONE)
        {
            isOutputForChart = 0;
            DebugPrint("%d MEASURE_DONE: base:%d,  ZeroPeak:%d, %d\r\n", xSystem.TickCount1Ms, base, alcoholMeter.ZeroPeak, xSystem.GLStatus.Al_Step);
            AP_Ctrl_Push(1);
            if (base)
            {
                //alcoholMeter.ZeroPeak = alcoholMeter.PeakValue;
                // Lay gia tri kalman khi measure zero point
                alcoholMeter.ZeroPeak = alcoholMeter.ZeroPeakTmp;
                DebugPrint("%d MEASURE_ZERO_POINT DONE/Check ZeroPoint: %d, %d\r\n", xSystem.TickCount1Ms, alcoholMeter.ZeroPeak, xSystem.GLStatus.Al_Step);
                switch (AlMManage.Task)
                {
                case ALM_DOCHUDONG:
                    DebugPrint("%d Switch to BLOW_DETECT %d\r\n", xSystem.TickCount1Ms, alcoholMeter.CleanSensor_CleanTick);
                    AlM_ChangeStage(BLOW_DETECT);
                    break;
                case ALM_DOTHUDONG:
                    DebugPrint("%d Switch to MEASURE_SAMPLE for ALM_DOTHUDONG: %d\r\n", xSystem.TickCount1Ms, alcoholMeter.CleanSensor_CleanTick);
                    AlM_ChangeStage(MEASURE_SAMPLE);
                    break;
                case ALM_DOADC:
                    DebugPrint("%d Switch to MEASURE_SAMPLE for ALM_DOADC: %d\r\n", xSystem.TickCount1Ms, alcoholMeter.CleanSensor_CleanTick);
                    AlM_ChangeStage(BLOW_DETECT);
                    break;
                default:
                    AlM_ChangeStage(SENSOR_IDLE);
                    break;
                }
            }
            else
            {
                AlMManage.Step = 2;
                if (alcoholMeter.ZeroPeak > 0)
                {
                    alcoholMeter.PeakValue = alcoholMeter.PeakValue - alcoholMeter.ZeroPeak;
                }
                xSystem.Record.Name.KetQua.value = (float)Al_ConvertAdcToMgL(alcoholMeter.PeakValue) / 10000;
                DebugPrint("%d Measure done: %d\r\n", xSystem.TickCount1Ms, alcoholMeter.PeakValue);
                peakData.peak = alcoholMeter.PeakValue;
                peakData.maxPeak = alcoholMeter.tmpPeakValue;
                peakData.max90 = (float)((alcoholMeter.tmpPeakValue * 90.0) / 100.0);
                DoADC_UpdatePeak(peakData.maxPeak);
            }
        }

        if (xSystem.GLStatus.Al_Step == MEASURE_FAIL)
        {
            isOutputForChart = 0;
            AP_Ctrl_Push(1);
            if (base)
            {
                DebugPrint("%d MEASURE_ZERO_POINT/MEASURE_FAIL, sampleCount: %d\r\n", xSystem.TickCount1Ms, alcoholMeter.rawSampleCount);
                alcoholMeter.ZeroPeak = 0;
                //AP_Ctrl_Push(1);
                AlM_ChangeStage(SENSOR_IDLE);
            }
            else
            {
                AlMManage.Step = 2;
                xSystem.Record.Name.KetQua.value = 0;
                alcoholMeter.ZeroPeak = 0;
                peakData.peak = 0;
                peakData.maxPeak = 0;
                peakData.max90 = 0;
            }
            if (AlMManage.Task == ALM_DOADC)
            {
                xSystem.Keypad.WD_Req = WD_HIEUCHINH;
                strcpy((char *)xSystem.Keypad.Str, Txt_LoiSensor);
                xSystem.Keypad.Status = 0;
                strcpy((char *)xSystem.Keypad.Header, Txt_ThongBao);
                GUI_WD_Change(WD_NOTIOK);
            }
            else
            {
                xSystem.Keypad.WD_Req = WD_MAIN_MENU;
                strcpy((char *)xSystem.Keypad.Str, Txt_LoiSensor);
                xSystem.Keypad.Status = 0;
                strcpy((char *)xSystem.Keypad.Header, Txt_ThongBao);
                GUI_WD_Change(WD_NOTIOK);
            }
            AlM_SetTask(ALM_IDLE);
        }
    }
    break;
    case 2:
    {
        // Finish,
        if ((xSystem.TickCount1Ms - alcoholMeter.startMeasureTick) > 1000)
        {
            switch (AlMManage.Task)
            {
            case ALM_DOCHUDONG:
                GUI_WD_Change(WD_KQ_DO_CHU_DONG);
                break;
            case ALM_DOTHUDONG:
                GUI_WD_Change(WD_DO_THU_DONG);
                break;
            case ALM_DOADC:
                //AlM_ChangeStage(MEASURE_SAMPLE);
                GUI_WD_Change(WD_DOADC);
                break;
            default:
                AlM_ChangeStage(SENSOR_IDLE);
                break;
            }
            DebugPrint("%d MEASURE_SAMPLE FINISH, push, after %d \r\n", xSystem.TickCount1Ms, alcoholMeter.CleanSensor_CleanTick);
            AP_Ctrl_Push(1);
            AlMManage.Task = ALM_IDLE;
            AlMManage.Stage = SENSOR_IDLE;
            AlMManage.Step = 0;
            AlM_ChangeStage(SENSOR_IDLE);
        }
    }
    break;
    default:
        break;
    }
}

/* Cac stage do chu dong:
1. SENSOR_IDLE: Khoi tao cac thong tin ban dau, doi sang stage SENSOR_CHECK
2. SENSOR_CLEAN: Push
*/

static void AlM_DoChuDong_Tick(void)
{

    uint32_t diffMs = xSystem.TickCount1Ms - alcoholMeter.startTimeTick;
    alcoholMeter.CleanSensor_CleanTick++;
    uint8_t Stage = AlMManage.Stage;
    switch (Stage)
    {
    case SENSOR_IDLE:
    {
        xSystem.GLStatus.Al_Step = NO_MEASURE;
        BlowDetect_Clear();
        AM_Clear();

        DebugPrint("%d Switch state to SENSOR_CHECK, after:%d, rawCnt:%d, invalid:%d, idx:%d, lastValue:%d\r\n", xSystem.TickCount1Ms,
                   alcoholMeter.CleanSensor_CleanTick, alcoholMeter.rawSampleCount, alcoholMeter.InvalidDataClk,
                   alcoholMeter.RawIndex, alcoholMeter.LastValue);
        AlM_ChangeStage(SENSOR_CLEAN);
        alcoholMeter.startTimeTick = xSystem.TickCount1Ms;
    }
    break;
    case SENSOR_CLEAN:
    {
        SensorClean();
    }
    break;
    case MEASURE_ZERO_POINT: // Zero point without
    {
        SensorMeasure(1);
    }
    break;
    case BLOW_DETECT:
    {
        // ! timeout
        if (AlMManage.Step == 0)
        {
            if (ALM_DOADC != AlMManage.Task)
            {
                GUI_WD_Change(WD_DO_CHU_DONG);
            }
            else
            {
                GUI_WD_Change(WD_DOADC);
            }
            AlMManage.Step++;
        }
        else
        {
            // Blow_detect: 10ms
            if ((xSystem.TickCount1Ms % 10) != 0)
                break;
            if (BlowDetect_BlowDetected())
            {
                DebugPrint("%d Switch state to MEASURE_SAMPLE %d\r\n", xSystem.TickCount1Ms, xSystem.GLStatus.Al_Step);
                AlM_ChangeStage(MEASURE_SAMPLE);
            }
        }
    }
    break;
    case MEASURE_SAMPLE:
    {
        SensorMeasure(0);
    }
    break;
    default:
        break;
    }
}

#if 0

static void AlM_DoADC_Tick(void)
{
    uint8_t Stage = AlMManage.Stage;
    switch (Stage)
    {
    case SENSOR_IDLE:
        xSystem.GLStatus.Al_Step = NO_MEASURE;
        //alcoholMeter.CurrentValue = alcoholMeter.LastValue = 0;
        //alcoholMeter.UptrendCnt = alcoholMeter.DowntrendCnt = alcoholMeter.GraphTrend = 0;
        //alcoholMeter.RawIndex = alcoholMeter.ZeroPeak = alcoholMeter.PeakValue = 0;
        //alcoholMeter.tmpPeakValue = alcoholMeter.oldTmpPeakValue = 0;
        // Chuyen sang che do Hut khi check truoc khi do
        //CleanSensor_CleanTick = 0;
        BlowDetect_Clear();
        AM_Clear();

        AlM_ChangeStage(SENSOR_CHECK);
        break;
    case SENSOR_CLEAN:
        /*Clean sensor*/
        if (AlMManage.Step == 0)
        {
            GUI_WD_Change(WD_KHOITAO_CAMBIEN);
            AlMManage.Step++;
        }
        /*Thoi khong khi ra khoi sensor 5s*/
        alcoholMeter.CleanSensor_CleanTick++;

        if (alcoholMeter.CleanSensor_CleanTick == 2)
        {
            DebugPrint("%d push \r\n", xSystem.TickCount1Ms);
            AP_Ctrl_Push(1);
        }
        else if (alcoholMeter.CleanSensor_CleanTick <= SENSOR_INIT_TIME1)
        {
            if (alcoholMeter.CleanSensor_CleanTick == SENSOR_INIT_TIME1)
            {
                DebugPrint("%d pull \r\n", xSystem.TickCount1Ms);
                AP_Ctrl_Pull(1);
            }
        }
        else
        {
            // Chuyen sang buoc do thu
            if (alcoholMeter.CleanSensor_CleanTick == SENSOR_INIT_TIME2)
            {
                DebugPrint("Switch to MEASURE_ZERO_POINT %d\r\n", xSystem.GLStatus.Al_Step);
                AlM_ChangeStage(MEASURE_ZERO_POINT);
            }
        }
        break;
    case MEASURE_ZERO_POINT:
        if (xSystem.GLStatus.Al_Step == NO_MEASURE)
        {
            AlcoholMetter_StartMeasure();
        }

        if (xSystem.GLStatus.Al_Step == MEASURING)
        {
            Alcohol_Measuring();
        }
        if (xSystem.GLStatus.Al_Step == MEASURE_DONE)
        {
            alcoholMeter.ZeroPeak = alcoholMeter.PeakValue;
            DebugPrint("\rCheck ZeroPoint: %d", alcoholMeter.ZeroPeak);
            DebugPrint("%d push \r\n", xSystem.TickCount1Ms);
            AP_Ctrl_Push(1);
            AlM_ChangeStage(BLOW_DETECT);
            // AlM_ChangeStage(SENSOR_IDLE);
        }
        if (xSystem.GLStatus.Al_Step == MEASURE_FAIL)
        {
            alcoholMeter.ZeroPeak = 0;
            AP_Ctrl_Push(1);
            DebugPrint("\rMEASURE_FAIL ==>>");
            DebugPrint("%d push \r\n", xSystem.TickCount1Ms);
            //              AlM_ChangeStage(BLOW_DETECT);
            AlM_ChangeStage(SENSOR_IDLE);
        }
        break;
    case BLOW_DETECT:
        if (AlMManage.Step == 0)
        {
            GUI_WD_Change(WD_DOADC);
            AlMManage.Step++;
        }
        else if (BlowDetect_BlowDetected())
        {
            AlM_ChangeStage(MEASURE_SAMPLE);
        }
        break;
    case MEASURE_SAMPLE:
    {
        /*Hut khong khi vao sensor*/
        uint8_t Step = AlMManage.Step;
        switch (Step)
        {
        case 0:
        {
            DebugPrint("%d pull \r\n", xSystem.TickCount1Ms);
            AP_Ctrl_Pull(1);
            AlMManage.Step = 1;
            xSystem.GLStatus.Al_Step = NO_MEASURE;
        }
        break;
        /*Vua hut vua do HOAC hut xong do ????*/
        /*1. Chon hut xong do*/
        case 1:
        {
            if (xSystem.GLStatus.Al_Step == NO_MEASURE)
            {
                DebugPrint("Switch to MEASURING %d\r\n", xSystem.GLStatus.Al_Step);
                AlcoholMetter_StartMeasure();
            }

            if (xSystem.GLStatus.Al_Step == MEASURING)
            {
                Alcohol_Measuring();
            }

            if (xSystem.GLStatus.Al_Step == MEASURE_DONE)
            {
                AlMManage.Step = 2;
                alcoholMeter.ZeroPeak = alcoholMeter.PeakValue;
                DebugPrint("\rZeroPoint Measure: %d\r\n", alcoholMeter.ZeroPeak);
                xSystem.Record.Name.KetQuaADC.peak = alcoholMeter.ZeroPeak;
                xSystem.Record.Name.KetQuaADC.maxPeak = alcoholMeter.PeakBuff[2];
                xSystem.Record.Name.KetQuaADC.max90 = alcoholMeter.PeakBuff[2] * 0.9;
                //xSystem.Record.Name.KetQua.value = (float)Al_ConvertAdcToMgL(ZeroPeak)/10;
                //DoChuDong_UpdateValueView(xSystem.SysData.AlMgLx10);
                //DoChuDong_UpdateADCValueView(ZeroPeak);
            }
            if (xSystem.GLStatus.Al_Step == MEASURE_FAIL)
            {
                AlMManage.Step = 2;
                xSystem.Record.Name.KetQuaADC.peak = 0;
                xSystem.Record.Name.KetQuaADC.maxPeak = 0;
                xSystem.Record.Name.KetQuaADC.max90 = 0;
                // DoChuDong_UpdateADCValueView(ZeroPeak);
                // DoChuDong_UpdateStrView((uint8_t*)"***");
            }
        }
        break;
        case 2:
        {
            // Dong van day khi ra
            DebugPrint("%d push \r\n", xSystem.TickCount1Ms);
            AP_Ctrl_Push(1);
            AlMManage.Step = 0;
            AlMManage.Task = ALM_IDLE;
            AlM_ChangeStage(SENSOR_IDLE);
        }
        break;
        }
        break;
    }
    }
}
#endif
static void AlM_DO_Tick(void)
{
    peakData.maxPeak = 0;
    peakData.peak = 0;
    peakData.max90 = 0;
}

static void AlM_XA_Tick(void)
{
#if 1
    uint16_t i;
    AP_Ctrl_Pull(1);
    AlM_SetTask(ALM_IDLE);
#else
    uint8_t Stage = AlMManage.Stage;
    switch (Stage)
    {
    case SENSOR_IDLE:
    {
        AP_Ctrl_Pull(1);
    }
    break;
    }
#endif
}

#if 0
void AlM_PumpOn(void)
{
    /*Clear 1 chan roi moi set chan con lai*/
    GPIO_WriteBit(AIR_PUMP_A_CTRL_PORT, AIR_PUMP_A_CTRL_PIN, (BitAction)0);
    GPIO_WriteBit(AIR_PUMP_B_CTRL_PORT, AIR_PUMP_B_CTRL_PIN, (BitAction)1);
}

void AlM_PumpOff(void)
{
    GPIO_WriteBit(AIR_PUMP_A_CTRL_PORT, AIR_PUMP_A_CTRL_PIN, (BitAction)0);
    GPIO_WriteBit(AIR_PUMP_B_CTRL_PORT, AIR_PUMP_B_CTRL_PIN, (BitAction)0);
}
#endif

/*
typedef struct AlcoholMeter
{
    uint16_t ZeroPeak; // Avg zero peak value, may be top peak
      uint16_t ZeroTopPeak; // top of zero point

    uint16_t Alcohol_RawSampleBuffer[RAW_SAMPLE_BUFFER_SIZE];
    uint16_t Alcohol_FIRSampleBuffer[2900];
    uint16_t RawIndex;
    uint16_t FIRIndex;

    uint16_t PeakBuff[MAX_PEAK_BUFFER_SIZE];
    uint16_t PeakValue;
    uint16_t PeakIndex;
        uint16_t tmpPeakValue;
    uint16_t oldTmpPeakValue;

    uint16_t CurrentValue, LastValue;
    uint8_t AlcoholMeasureDone;
    uint8_t isBlowDetected;

    uint8_t pumpStatus;
    uint8_t GraphTrend;
    uint16_t UptrendCnt;
    uint16_t DowntrendCnt;

    uint32_t CleanSensor_CleanTick;

    uint32_t InvalidDataClk;
    uint32_t rawSampleCount;
    uint32_t startTimeTick;
    uint32_t startMeasureTick;
    uint32_t pumpTimeTick;
    uint8_t mode;
} AlcoholMeter_t;

*/
static void AlcoholMetter_StartMeasure(void)
{
    /*Clear Buffer*/
    memset(alcoholMeter.Alcohol_RawSampleBuffer, 0x00, sizeof(alcoholMeter.Alcohol_RawSampleBuffer));
    memset(alcoholMeter.Alcohol_FIRSampleBuffer, 0x00, sizeof(alcoholMeter.Alcohol_FIRSampleBuffer));
    memset(alcoholMeter.PeakBuff, 0, sizeof(alcoholMeter.PeakBuff));
    alcoholMeter.PeakValue = 0;
    alcoholMeter.PeakIndex = 0;
    alcoholMeter.tmpPeakValue = 0;
    alcoholMeter.oldTmpPeakValue = 0;
    alcoholMeter.CurrentValue = alcoholMeter.LastValue = 0;

    alcoholMeter.RawIndex = 0;
    alcoholMeter.rawSampleCount = 0;
    alcoholMeter.isBlowDetected = 0;
    alcoholMeter.GraphTrend = UNKNOWN_TREND;
    alcoholMeter.DowntrendCnt = alcoholMeter.UptrendCnt = 0;
    alcoholMeter.InvalidDataClk = 0;
    xSystem.GLStatus.Al_Step = MEASURING;
}

static void AlcoholMetter_StopMeasure(void)
{
    if (alcoholMeter.LastValue == 4095)
    {
        xSystem.GLStatus.Al_Step = MEASURE_FAIL;
    }
    else
        xSystem.GLStatus.Al_Step = MEASURE_DONE;
    /*Process Raw sample buffer*/
}

uint16_t Al_Data_Index = 0;
const uint16_t Al_Data[3000] =
    {
        1, 5, 7, 10, 20, 27, 34, 41, 48, 55, 62, 69, 76, 83, 90, 97, 104, 111, 118, 125, 132, 139, 146, 153, 160, 167, 174, 181, 188, 195, 202, 209, 216, 223, 230, 237, 244, 251, 258, 265, 272, 279, 286, 293, 300, 307, 314, 321, 328, 335, 342, 349, 356, 363, 370, 377, 384, 391, 398, 405, 412, 419, 426, 433, 440, 447, 454, 461, 468, 475, 482, 489, 496, 503, 510, 517, 524, 531, 538, 545, 552, 559, 566, 573, 580, 587, 594, 601, 608, 615, 622, 629, 636, 643, 650, 657, 664, 671, 678, 685, 692, 699, 706, 713, 720, 727, 734, 741, 748, 755, 762, 769, 776, 783, 790, 797, 804, 811, 818, 825, 832, 839, 846, 853, 860, 867, 874, 881, 888, 895, 902, 909, 916, 923, 930, 937, 944, 951, 958, 965, 972, 979, 986, 993, 1000, 1007, 1014, 1021, 1028, 1035, 1042, 1049, 1056, 1063, 1070, 1077, 1084, 1091, 1098, 1105, 1112, 1119, 1126, 1133, 1140, 1147, 1154, 1161, 1168, 1175, 1182, 1189, 1196, 1203, 1210, 1217, 1224, 1231, 1238, 1245, 1252, 1259, 1266, 1273, 1280, 1287, 1294, 1301, 1308, 1315, 1322, 1329, 1336, 1343, 1350, 1357, 1364, 1371, 1378, 1385, 1392, 1399, 1406, 1413, 1420, 1427, 1434, 1441, 1448, 1455, 1462, 1469, 1476, 1483, 1490, 1497, 1504, 1511, 1518, 1525, 1532, 1539, 1546, 1553, 1560, 1567, 1574, 1581, 1588, 1595, 1602, 1609, 1616, 1623, 1630, 1637, 1644, 1651, 1658, 1665, 1672, 1679, 1686, 1693, 1700, 1707, 1714, 1721, 1728, 1735, 1742, 1749, 1756, 1763, 1770, 1777, 1784, 1791, 1798, 1805, 1812, 1819, 1826, 1833, 1840, 1847, 1854, 1861, 1868, 1875, 1882, 1889, 1896, 1903, 1910, 1917, 1924, 1931, 1938, 1945, 1952, 1959, 1966, 1973, 1980, 1987, 1994, 2001, 2008, 2015, 2022, 2029, 2036, 2043, 2050, 2057, 2064, 2071, 2078, 2085, 2092, 2099, 2106, 2113, 2120, 2127, 2134, 2141, 2148, 2155, 2162, 2169, 2176, 2183, 2190, 2197, 2204, 2211, 2218, 2225, 2232, 2239, 2246, 2253, 2260, 2267, 2274, 2281, 2288, 2295, 2302, 2309, 2316, 2323, 2330, 2337, 2344, 2351, 2358, 2365, 2372, 2379, 2386, 2393, 2400, 2407, 2414, 2421, 2428, 2435, 2442, 2449, 2456, 2463, 2470, 2477, 2484, 2491, 2498, 2505, 2512, 2519, 2526, 2533, 2540, 2547, 2554, 2561, 2568, 2575, 2582, 2589, 2596, 2603, 2610, 2617, 2624, 2631, 2631, 2631, 2631, 2631, 2631, 2631, 2631, 2631, 2631, 2631, 2631, 2631, 2631, 2631, 2631, 2631, 2631, 2631, 2631, 2631, 2631, 2631, 2631, 2631, 2632, 2632, 2632, 2632, 2630, 2628, 2626, 2624, 2622, 2620, 2618, 2616, 2614, 2612, 2610, 2608, 2606, 2604, 2610, 2616, 2622, 2628, 2634, 2640, 2646, 2652, 2658, 2664, 2670, 2676, 2682, 2688, 2694, 2700, 2706, 2712, 2718, 2724, 2730, 2736, 2742, 2748, 2754, 2760, 2766, 2772, 2778, 2784, 2790, 2796, 2802, 2808, 2814, 2820, 2826, 2832, 2838, 2844, 2850, 2856, 2862, 2868, 2874, 2880, 2886, 2892, 2898, 2904, 2910, 2916, 2922, 2928, 2934, 2940, 2946, 2952, 2958, 2964, 2970, 2976, 2982, 2988, 2994, 3000, 3006, 3012, 3018, 3024, 3030, 3036, 3042, 3048, 3054, 3060, 3066, 3072, 3078, 3084, 3090, 3096, 3102, 3108, 3114, 3120, 3126, 3132, 3138, 3144, 3150, 3156, 3162, 3168, 3174, 3180, 3186, 3192, 3198, 3204, 3210, 3216, 3222, 3228, 3234, 3240, 3246, 3252, 3258, 3264, 3270, 3276, 3282, 3288, 3294, 3300, 3306, 3312, 3318, 3324, 3330, 3336, 3342, 3348, 3354, 3360, 3366, 3372, 3378, 3384, 3390, 3396, 3402, 3408, 3414, 3420, 3426, 3432, 3438, 3444, 3450, 3456, 3462, 3468, 3474, 3480, 3486, 3492, 3498, 3504, 3510, 3516, 3522, 3528, 3534, 3540, 3546, 3552, 3558, 3564, 3570, 3576, 3582, 3588, 3594, 3600, 3606, 3612, 3618, 3624, 3630, 3636, 3642, 3648, 3654, 3660, 3666, 3672, 3678, 3684, 3690, 3696, 3702, 3708, 3714, 3720, 3726, 3732, 3738, 3744, 3750, 3756, 3762, 3768, 3774, 3780, 3786, 3792, 3798, 3804, 3810, 3816, 3822, 3828, 3834, 3840, 3846, 3852, 3858, 3864, 3870, 3876, 3882, 3888, 3894, 3900, 3906, 3912, 3918, 3924, 3930, 3936, 3942, 3948, 3954, 3960, 3966, 3972, 3978, 3984, 3990, 3996, 3990, 3984, 3978, 3972, 3966, 3960, 3954, 3948, 3942, 3936, 3930, 3924, 3918, 3912, 3906, 3900, 3894, 3888, 3882, 3876, 3870, 3864, 3858, 3852, 3846, 3840, 3834, 3828, 3822, 3816, 3810, 3804, 3798, 3792, 3786, 3780, 3774, 3768, 3762, 3756, 3750, 3744, 3738, 3732, 3726, 3720, 3714, 3708, 3702, 3696, 3690, 3684, 3678, 3672, 3666, 3660, 3654, 3648, 3642, 3636, 3630, 3624, 3618, 3612, 3606, 3600, 3594, 3588, 3582, 3576, 3570, 3564, 3558, 3552, 3546, 3540, 3534, 3528, 3522, 3516, 3510, 3504, 3498, 3492, 3486, 3480, 3474, 3468, 3462, 3456, 3450, 3444, 3438, 3432, 3426, 3420, 3414, 3408, 3402, 3396, 3390, 3384, 3378, 3372, 3366, 3360, 3354, 3348, 3342, 3336, 3330, 3324, 3318, 3312, 3306, 3300, 3294, 3288, 3282, 3276, 3270, 3264, 3258, 3252, 3246, 3240, 3234, 3228, 3222, 3216, 3210, 3204, 3198, 3192, 3186, 3180, 3174, 3168, 3162, 3156, 3150, 3144, 3138, 3132, 3126, 3120, 3114, 3108, 3102, 3096, 3090, 3084, 3078, 3072, 3066, 3060, 3054, 3048, 3042, 3036, 3030, 3024, 3018, 3012, 3006, 3000, 2994, 2988, 2982, 2976, 2970, 2964, 2958, 2952, 2946, 2940, 2934, 2928, 2922, 2916, 2910, 2904, 2898, 2892, 2886, 2880, 2874, 2868, 2862, 2856, 2850, 2844, 2838, 2832, 2826, 2820, 2814, 2808, 2802, 2796, 2790, 2784, 2778, 2772, 2766, 2760, 2754, 2748, 2742, 2736, 2730, 2724, 2718, 2712, 2706, 2700, 2694, 2688, 2682, 2676, 2670, 2664, 2658, 2652, 2646, 2640, 2634, 2628, 2622, 2616, 2610, 2604, 2598, 2592, 2586, 2580, 2574, 2568, 2562, 2556, 2550, 2544, 2538, 2532, 2526, 2520, 2514, 2508, 2502, 2496, 2490, 2484, 2478, 2472, 2466, 2460, 2454, 2448, 2442, 2436, 2430, 2424, 2418, 2412, 2406, 2400, 2394, 2388, 2382, 2376, 2370, 2364, 2358, 2352, 2346, 2340, 2334, 2328, 2322, 2316, 2310, 2304, 2298, 2292, 2286, 2280, 2274, 2268, 2262, 2256, 2250, 2244, 2238, 2232, 2226, 2220, 2214, 2208, 2202, 2196, 2190, 2184, 2178, 2172, 2166, 2160, 2154, 2148, 2142, 2136, 2130, 2124, 2118, 2112, 2106, 2100, 2094, 2088, 2082, 2076, 2070, 2064, 2058, 2052, 2046, 2040, 2034, 2028, 2022, 2016, 2010, 2004, 1998, 1992, 1986, 1980, 1974, 1968, 1962, 1956, 1950, 1944, 1938, 1932, 1926, 1920, 1914, 1908, 1902, 1896, 1890, 1884, 1878, 1872, 1866, 1860, 1854, 1848, 1842, 1836, 1830, 1824, 1818, 1812, 1806, 1800, 1794, 1788, 1782, 1776, 1770, 1764, 1758, 1752, 1746, 1740, 1734, 1728, 1722, 1716, 1710, 1704, 1698, 1692, 1686, 1680, 1674, 1668, 1662, 1656, 1650, 1644, 1638, 1632, 1626, 1620, 1614, 1608, 1602, 1596, 1590, 1584, 1578, 1572, 1566, 1560, 1554, 1548, 1542, 1536, 1530, 1524, 1518, 1512, 1506};

uint16_t TestValue = 0;

static void Alcohol_Measuring(void)
{
    uint8_t Stage = AlMManage.Stage;
    uint32_t avgPeak = 0;
    xSystem.GLStatus.Al_Step = MEASURING;
    uint16_t i = 0;
    int count = 0;
    float Percent = 0;

    alcoholMeter.CurrentValue = ADS7822_GetOneSample();
    if (alcoholMeter.rawSampleCount < RAW_SAMPLE_BUFFER_SIZE)
    {
        alcoholMeter.rawSampleCount++;
    }
    if (Stage == MEASURE_ZERO_POINT)
    {
        alcoholMeter.ZeroPeakTmp = (int32_t)kalman_update(&km_zero_point, alcoholMeter.CurrentValue);
    }
    int32_t normalValue = (int32_t)kalman_update(&km_normal, alcoholMeter.CurrentValue);
    // Set isOutputForChart = 1 to print all measure data
    static uint32_t countPacket = 0;
    if (isOutputForChart == 0)
    {
        isOutputForChart = 1;
        countPacket = 0;
    }
    countPacket++;
    isOutputForChart = 0;
    if (isOutputForChart)
    {
        UART_Printf(DEBUG_UART, "%d,%d,%d\r\n", alcoholMeter.ZeroPeakTmp, normalValue, alcoholMeter.CurrentValue);
    }
    //else
    //{
    //    if (Stage == MEASURE_SAMPLE)
    //    {
    //    }
    //}
    //DebugPrint("%d Index [%d:%d - %d] %d\r\n", xSystem.TickCount1Ms, alcoholMeter.rawSampleCount, alcoholMeter.CurrentValue, alcoholMeter.tmpPeakValue, alcoholMeter.LastValue);

    if (alcoholMeter.RawIndex == 0)
    {
        count = 0;
        alcoholMeter.LastValue = alcoholMeter.CurrentValue;
        DebugPrint("%d Index [%d:%d:%d - %d] %d\r\n", xSystem.TickCount1Ms, alcoholMeter.rawSampleCount, alcoholMeter.RawIndex, alcoholMeter.CurrentValue, alcoholMeter.tmpPeakValue, alcoholMeter.LastValue);
        alcoholMeter.startMeasureTick = xSystem.TickCount1Ms;
        alcoholMeter.RawIndex++;
        return;
    }
    else if (ABS(alcoholMeter.LastValue, alcoholMeter.CurrentValue) < ALCOHOL_RAWSAMPLE_WINDOW /*&& alcoholMeter.CurrentValue < ALCOHOL_RAWSAMPLE_WINDOW*/)
    {
        if (alcoholMeter.rawSampleCount < 10)
        {
            DebugPrint("%d Index1 [%d:%d:%d - %d] %d\r\n", xSystem.TickCount1Ms, alcoholMeter.rawSampleCount, alcoholMeter.RawIndex, alcoholMeter.CurrentValue, alcoholMeter.tmpPeakValue, alcoholMeter.LastValue);
        }
        alcoholMeter.RawIndex++;
    }
    else
    {
        if (alcoholMeter.rawSampleCount < 10)
        {
            DebugPrint("%d Index2 [%d:%d:%d - %d] %d\r\n", xSystem.TickCount1Ms, alcoholMeter.rawSampleCount, alcoholMeter.RawIndex, alcoholMeter.CurrentValue, alcoholMeter.tmpPeakValue, alcoholMeter.LastValue);
        }
        if (alcoholMeter.InvalidDataClk++ > 2000)
        {
            xSystem.GLStatus.Al_Step = MEASURE_FAIL;
            DebugPrint("%d Switch to MEASURE_FAIL, sampleCnt:%d, ind: %d, after:%d, invalid:%d\r\n", xSystem.TickCount1Ms,
                       alcoholMeter.rawSampleCount, alcoholMeter.RawIndex,
                       alcoholMeter.CleanSensor_CleanTick, alcoholMeter.InvalidDataClk);
        }
        alcoholMeter.LastValue = alcoholMeter.CurrentValue;
        return;
    }
#if 0
    if (Stage == MEASURE_ZERO_POINT)
    {
        alcoholMeter.ZeroPeakTmp = (int32_t)kalman_update(&km_zero_point, alcoholMeter.CurrentValue);
    }
		int32_t normalValue = (int32_t)kalman_update(&km_normal, alcoholMeter.CurrentValue);
		// Set isOutputForChart = 1 to print all measure data
		if (isOutputForChart == 0)
			isOutputForChart = 1;
		if (isOutputForChart) 
		{
			UART_Printf(DEBUG_UART,"%d,%d,%d\r\n",alcoholMeter.ZeroPeakTmp, normalValue, alcoholMeter.CurrentValue);
		}
#endif
    //UART_Printf(DEBUG_UART,"%d Index3 [%d:%d - %d] %d\r\n", xSystem.TickCount1Ms, alcoholMeter.rawSampleCount, alcoholMeter.CurrentValue, alcoholMeter.tmpPeakValue, alcoholMeter.LastValue);
    memmove(&alcoholMeter.Alcohol_RawSampleBuffer[0],
            &alcoholMeter.Alcohol_RawSampleBuffer[1],
            sizeof(alcoholMeter.Alcohol_RawSampleBuffer) - sizeof(alcoholMeter.Alcohol_RawSampleBuffer[0]));

    // Push to back
    alcoholMeter.Alcohol_RawSampleBuffer[RAW_SAMPLE_BUFFER_SIZE - 1] = alcoholMeter.CurrentValue;

    /*Get FIR Data*/
    //  if(RawIndex >= 100 && RawIndex < 1900){
    //      for(uint8_t i = 0; i < 100; i++){
    //          tmp += Alcohol_RawSampleBuffer[RawIndex - 1 - i];
    //      }
    //      Alcohol_FIRSampleBuffer[FIRIndex] = tmp / 100;
    //      FIRIndex++;
    //  }
    /*Scanning the trend and peak*/
    //  if(FIRIndex > 2){
    //      if(Alcohol_FIRSampleBuffer[FIRIndex] > Alcohol_FIRSampleBuffer[FIRIndex - 1])
    //          tmpPeakValue = Alcohol_FIRSampleBuffer[FIRIndex];
    //
    //      if(MAX(Alcohol_FIRSampleBuffer[FIRIndex], Alcohol_FIRSampleBuffer[FIRIndex - 1]) == Alcohol_FIRSampleBuffer[FIRIndex]){
    //          UptrendCnt++;
    //          if(UptrendCnt > 10) DowntrendCnt = 0;
    //          TmpTrend = UP_TREND;
    //      }
    //      else if(MAX(Alcohol_FIRSampleBuffer[FIRIndex], Alcohol_FIRSampleBuffer[FIRIndex - 1]) == Alcohol_FIRSampleBuffer[FIRIndex - 1]){
    //          DowntrendCnt++;
    //          if(DowntrendCnt > 10) UptrendCnt = 0;
    //          TmpTrend = DOWN_TREND;
    //      }
    //  }
    /*Without FIR filter*/
    int peakPos = (MAX_PEAK_BUFFER_SIZE / 2);
    if (alcoholMeter.RawIndex > 0)
    {
        if (alcoholMeter.CurrentValue >= alcoholMeter.tmpPeakValue)
        {
            alcoholMeter.tmpPeakValue = alcoholMeter.CurrentValue;
#if USE_DYNAMIC_PEAK_BUFFER
            for (int i = 0; i < peakPos; i++)
            {
                alcoholMeter.PeakBuff[i] = alcoholMeter.PeakBuff[i + 1];
            }
            alcoholMeter.PeakBuff[peakPos] = alcoholMeter.tmpPeakValue;
            alcoholMeter.PeakIndex = peakPos;
#else
            alcoholMeter.PeakBuff[0] = alcoholMeter.PeakBuff[1];
            alcoholMeter.PeakBuff[1] = alcoholMeter.PeakBuff[2];
            alcoholMeter.PeakBuff[2] = alcoholMeter.tmpPeakValue;
            alcoholMeter.PeakIndex = 3;
#endif
            alcoholMeter.UptrendCnt++;
            if (alcoholMeter.UptrendCnt > TREND_CONFIRM / 2)
            {
                alcoholMeter.DowntrendCnt = 0;
            }
        }
        else if (alcoholMeter.CurrentValue < alcoholMeter.tmpPeakValue)
        {
            alcoholMeter.DowntrendCnt++;
#if USE_DYNAMIC_PEAK_BUFFER
            if (alcoholMeter.PeakIndex < MAX_PEAK_BUFFER_SIZE)
            {
                alcoholMeter.PeakBuff[alcoholMeter.PeakIndex++] = alcoholMeter.CurrentValue;
            }
#else
            if (alcoholMeter.PeakIndex < 5)
                alcoholMeter.PeakBuff[alcoholMeter.PeakIndex++] = alcoholMeter.CurrentValue;
#endif
            if (alcoholMeter.DowntrendCnt > TREND_CONFIRM / 2)
                alcoholMeter.UptrendCnt = 0;
        }

        // Lay 100 sample???
        if (alcoholMeter.CurrentValue == 0 && alcoholMeter.RawIndex > 100)
        {
            alcoholMeter.UptrendCnt = 0;
            alcoholMeter.DowntrendCnt++;
#if USE_DYNAMIC_PEAK_BUFFER
            if (alcoholMeter.PeakIndex < MAX_PEAK_BUFFER_SIZE)
            {
                alcoholMeter.PeakBuff[alcoholMeter.PeakIndex++] = alcoholMeter.CurrentValue;
            }
#else
            if (alcoholMeter.PeakIndex < 5)
            {
                alcoholMeter.PeakBuff[alcoholMeter.PeakIndex++] = alcoholMeter.CurrentValue;
            }
#endif
        }

        if (alcoholMeter.RawIndex % 100 == 0 || alcoholMeter.oldTmpPeakValue != alcoholMeter.tmpPeakValue)
        {
            //DebugPrint("\rp[%d:%d-%d]", alcoholMeter.RawIndex, alcoholMeter.CurrentValue, alcoholMeter.tmpPeakValue);
        }
        alcoholMeter.oldTmpPeakValue = alcoholMeter.tmpPeakValue;
    }
#if USE_DYNAMIC_PEAK_BUFFER
    uint16_t max_value = alcoholMeter.PeakBuff[peakPos];
#else
    uint16_t max_value = alcoholMeter.PeakBuff[2];
#endif
    if (alcoholMeter.UptrendCnt > TREND_CONFIRM / 10 && alcoholMeter.GraphTrend == UNKNOWN_TREND)
    {
        DebugPrint("\rUptrend Confirm !!!!\r\n");
        Beeps(2);
        alcoholMeter.GraphTrend = UP_TREND;
        alcoholMeter.DowntrendCnt = 0;
    }
    if ((alcoholMeter.DowntrendCnt > TREND_CONFIRM && alcoholMeter.GraphTrend == UP_TREND) || alcoholMeter.DowntrendCnt > 500)
    {
        alcoholMeter.GraphTrend = DOWN_TREND;
        DebugPrint("\r!!!! Downtrend Confirm\r\n");
        //Beeps(3);
        alcoholMeter.UptrendCnt = 0;
    }

    if (alcoholMeter.GraphTrend == DOWN_TREND)
    {
        alcoholMeter.PeakValue = alcoholMeter.tmpPeakValue;
        //DebugPrint("\rPeak Detected: %d\r\n", alcoholMeter.PeakValue);
    }

    alcoholMeter.LastValue = alcoholMeter.CurrentValue;

    // Measure zero point, at least 2 seconds
    uint32_t elapsed = xSystem.TickCount1Ms - alcoholMeter.startMeasureTick;

    // Check codition: measure for 2s or value < 90%
    //  if (alcoholMeter.RawIndex >= 100 || alcoholMeter.GraphTrend == DOWN_TREND)
    if (elapsed >= 2000 || alcoholMeter.GraphTrend == DOWN_TREND)
    {
        //alcoholMeter.PeakValue = alcoholMeter.tmpPeakValue;
        // Cal AVG
        count = 0;
        avgPeak = 0;
        int finishMeasure = 0;
        if (elapsed >= 2000)
        {
            // Mesure zero point or pasive mesuring must be finish after 2 sec
            //if ((Stage == MEASURE_ZERO_POINT) || (AlMManage.Task == ALM_DOTHUDONG) || (AlMManage.Task == ALM_DOADC))
            {
                finishMeasure = 1;
            }
        }
#if USE_DYNAMIC_PEAK_BUFFER
        for (i = peakPos; i < (alcoholMeter.PeakIndex < MAX_PEAK_BUFFER_SIZE ? alcoholMeter.PeakIndex : MAX_PEAK_BUFFER_SIZE); i++)
        {
            DebugPrint(" %d.", alcoholMeter.PeakBuff[i]);
            Percent = (float)(1.0 * alcoholMeter.PeakBuff[i]) / (float)(1.0 * alcoholMeter.tmpPeakValue);
            if (Percent > 0.90)
            {
                avgPeak += alcoholMeter.PeakBuff[i];
                count++;
            }
            else
            {
                // Giam xuong duoi < 90%, viec do hoan tat.
                finishMeasure = 1;
                break;
            }
        }
#else
        for (i = 0; i < 5; i++)
        {
            DebugPrint(" %d.", alcoholMeter.PeakBuff[i]);
            Percent = (float)(1.0 * alcoholMeter.PeakBuff[i]) / (float)(1.0 * alcoholMeter.PeakValue);
            if (Percent > 0.95)
            {
                tmp += alcoholMeter.PeakBuff[i];
                count++;
            }
        }
#endif
        if (count > 0)
        {
            // Dinh la trung binh cua peak->90% hay chinh peak;
            //alcoholMeter.PeakValue = alcoholMeter.tmpPeakValue;
            alcoholMeter.PeakValue = avgPeak / count;
        }
        if (alcoholMeter.GraphTrend == DOWN_TREND)
        {
            DebugPrint("\rPeak AVG[%d]: %d, Detected:[%d]\r\n", count, alcoholMeter.PeakValue, alcoholMeter.tmpPeakValue);
        }

        if (finishMeasure)
        {
            //alcoholMeter.LastValue = 0;
            Al_Data_Index = 0;
            AlcoholMetter_StopMeasure();
            DebugPrint("%d Switch to MEASURE_DONE after %d, totalSample:%d, invalid:%d\r\n",
                       xSystem.TickCount1Ms,
                       alcoholMeter.CleanSensor_CleanTick,
                       alcoholMeter.rawSampleCount, alcoholMeter.InvalidDataClk);
        }
    }
    RequestGetSample = 0;
}

static void AlcoholMetter_InitTimer(void)
{

    NVIC_InitTypeDef NVIC_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    //  RCC_ClocksTypeDef tstClock;

    /* Enable the TIM2 gloabal Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* TIM2 clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    /* Timer Setting Calculation tick 2 ms => 500 Hz
    Pulse-buzzer works well at frequencys 500 Hz
    TimerInterrupt  = f(TIM2)/((Period + 1)*(Prescaler + 1)*(Repetition + 1))
                    = 168.000.000/(168 * 2000 * 1)
                    ~ 500
    */
    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = 168 - 1; //100

    TIM_TimeBaseStructure.TIM_Prescaler = 2000 - 1;

    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    /* TIM IT enable */
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    /* TIM2 disable counter */
    TIM_Cmd(TIM2, ENABLE);
}

void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        if (!RequestGetSample)
            RequestGetSample = 1;
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}

static uint8_t CheckValidValue(uint16_t Value)
{
    return 1;
}

static uint16_t Al_ConvertAdcToMgL(uint16_t ADCValue)
{
    uint8_t i;
    float ratio;
    Al_CalibTable_t *pAlMCalibData = &xSystem.Parameters.AlMCalibData;
    if (pAlMCalibData->s.Al_MaxValue > AlM_Calib_MaxIndex - 1)
        pAlMCalibData->s.Al_MaxValue = AlM_Calib_MaxIndex - 1;
    if (CheckValidValue(ADCValue) == 0)
        return INVALID_VALUE;
    for (i = 0; i < pAlMCalibData->s.Al_MaxValue; i++)
    {
        if (ADCValue == pAlMCalibData->s.Data[i].ValueADC)
            return pAlMCalibData->s.Data[i].ValueMgL;
        if (i == (AlM_Calib_MaxIndex - 1))
            return INVALID_VALUE;
        if (ADCValue > pAlMCalibData->s.Data[i].ValueADC && ADCValue < pAlMCalibData->s.Data[i + 1].ValueADC)
        {
            ratio = (float)(ADCValue - pAlMCalibData->s.Data[i].ValueADC) * 10 / (pAlMCalibData->s.Data[i + 1].ValueADC - pAlMCalibData->s.Data[i].ValueADC);
            DebugPrint("\rADC[%d]:%d-%d MgL:%d-%d\r\n", ADCValue, pAlMCalibData->s.Data[i].ValueADC, pAlMCalibData->s.Data[i + 1].ValueADC, pAlMCalibData->s.Data[i].ValueMgL, pAlMCalibData->s.Data[i + 1].ValueMgL);
            return pAlMCalibData->s.Data[i].ValueMgL * 10 + (uint16_t)(ratio * (pAlMCalibData->s.Data[i + 1].ValueMgL - pAlMCalibData->s.Data[i].ValueMgL));
        }
    }
    return INVALID_VALUE;
}

static void TestChart()
{
#if 0
    static uint32_t tickCount = 0;
    tickCount++;
    //UART_Printf(DEBUG_UART, "%d \n", tickCount);

    if ((tickCount % 2) == 0)
    {

        //float v1= GetPressure();
        //uint16_t v2= ADS7822_GetOneSample();
        //uint16_t v3= GetTempSensor();
        //uint16_t v4= GetTempPrinter();
        //uint16_t v5= GetVBat();
        //float v6= GetVIn();
        //uint16_t v9 = updateEstimate(v5);
        //uint16_t v7= GetV_8V2();

        // uint16_t v8= GetVCCAirPump();
        if (isInitPressure == 0)
        {
            //updateEstimate(&km_pressure, v1);
            isInitPressure = 1;
            return;
        }
        //float k = kalman_update(&km_pressure, v8);

        //float k1 = kalman_update(&km_pressure1, v6);


        //uint16_t v2= ADS7822_GetOneSample();
        //uint16_t v3= GetTempSensor();
        //uint16_t v4= GetTempPrinter();
        //uint16_t v5= GetVBat();
        //uint16_t v6= GetVIn();
        //uint16_t v9 = kalman_update(v5);
        //uint16_t v7= GetV_8V2();
        //uint16_t v8= GetVCCAirPump();
        char buff[255] = {0};
        char buff1[255] = {0};

        //snprintf(buff, sizeof(buff), "%f,%f",v1, k);
        //snprintf(buff1, sizeof(buff1), "%f,%f",v6, k1);
        //UART_Printf(DEBUG_UART, "%s, %s\r\n", buff);


    }
    //return;
#endif
}