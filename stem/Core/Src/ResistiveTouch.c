#include "ResistiveTouch.h"
#include "ILI9488.h"
#include "GUI.h"
#include "UserGUIService.h"

#define CMD_RDX 0xD3
#define CMD_RDY 0x93

#define NBROFSAMPLE_READ 15
#define NBROFSAMPLE_GET 5
#define TOUCH_ADC_WINDOW 500

extern System_t xSystem;

void RTP_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = RTP_BUSY_Pin;

    GPIO_Init(RTP_BUSY_Port, &GPIO_InitStructure);

    /*Load from FRAM*/
    xSystem.Parameters.Offset_X_ADC = 0;
    xSystem.Parameters.Offset_Y_ADC = 0;
    xSystem.Parameters.Scale_X_ADC = 120;
    xSystem.Parameters.Scale_Y_ADC = 83;

    //  xSystem.Parameters.Scale_X_ADC = 0xFFFF;
    //  xSystem.Parameters.Scale_Y_ADC = 0xFFFF;
    /*Init BUSY pin as input*/

    xSystem.SPI->Init(RTP_SPI);
}

void RTP_CSEnable(uint8_t enable)
{
    GPIO_WriteBit(RTP_CS_Port, RTP_CS_Pin, (1 - enable));
}

uint16_t RTP_Read_X(void)
{
    uint8_t byte_array[2];
    uint16_t value;

    RTP_CSEnable(1);
    //  while(GPIO_ReadInputDataBit(RTP_BUSY_Port, RTP_BUSY_Pin) == 1);
    /*Send cmd*/
    xSystem.SPI->SendByte(RTP_SPI, CMD_RDX);

    //  while(GPIO_ReadInputDataBit(RTP_BUSY_Port, RTP_BUSY_Pin) == 1);

    byte_array[0] = xSystem.SPI->SendByte(RTP_SPI, 0xFF) & 0x7F;
    byte_array[1] = xSystem.SPI->SendByte(RTP_SPI, 0xFF);

    RTP_CSEnable(0);

    //  value = (uint16_t)(byte_array[0] << 5) | (byte_array[1]>>3);
    //  value &= 0xFFF;
    value = byte_array[0];
    value <<= 8;
    value |= byte_array[1];
    value >>= 3;
    //  DebugPrint("\rREAD ADC X: raw: %d", value);
    DelayMS(1);
    return value;
}

uint16_t RTP_Read_Y(void)
{
    uint8_t byte_array[2];
    uint16_t value;

    RTP_CSEnable(1);
    //  while(GPIO_ReadInputDataBit(RTP_BUSY_Port, RTP_BUSY_Pin) == 1);
    /*Send cmd*/
    xSystem.SPI->SendByte(RTP_SPI, CMD_RDY);

    //  while(GPIO_ReadInputDataBit(RTP_BUSY_Port, RTP_BUSY_Pin) == 1);

    byte_array[0] = xSystem.SPI->SendByte(RTP_SPI, 0xFF) & 0x7F;
    byte_array[1] = xSystem.SPI->SendByte(RTP_SPI, 0xFF);

    RTP_CSEnable(0);

    //  value = (uint16_t)(byte_array[0] << 5) | (byte_array[1]>>3);
    //  value &= 0xFFF;
    value = byte_array[0];
    value <<= 8;
    value |= byte_array[1];
    value >>= 3;
    //  DebugPrint("\rREAD ADC Y: raw: %d", value);
    DelayMS(1);
    return value;
}

extern uint8_t RTP_ReqNew;
uint16_t RTP_Get_X(void)
{
    uint8_t i, j;
    uint16_t tmpBuf[NBROFSAMPLE_READ];
    uint32_t Xsum = 0;
    uint16_t tmp;
    uint8_t index;

    /*Get sample*/
    for (i = 0; i < NBROFSAMPLE_READ; i++)
    {

        tmp = RTP_Read_X();

        //      if(tmp){
        index++;
        tmpBuf[i] = tmp;
        //      }

        //      if(i != 0 &&  ABS(tmpBuf[i] , tmpBuf[i-1]) > TOUCH_ADC_WINDOW){
        ////            DebugPrint("\rGet ADC X: medium: %d", 0);
        //          return 0;
        //      }
    }

    //  if(index < NBROFSAMPLE_READ) return 0;

    /*Sort sample from low to high*/
    for (i = 0; i < NBROFSAMPLE_READ - 1; i++)
    {
        for (j = i + 1; j < NBROFSAMPLE_READ; j++)
        {
            if (tmpBuf[i] > tmpBuf[i])
            {
                tmp = tmpBuf[i];
                tmpBuf[i] = tmpBuf[j];
                tmpBuf[j] = tmp;
            }
        }
    }

    /* Chi lay 5 sample o giua */
    for (i = NBROFSAMPLE_GET; i < NBROFSAMPLE_READ - NBROFSAMPLE_GET; i++)
    {

        Xsum += tmpBuf[i];
    }

    Xsum = Xsum / (NBROFSAMPLE_READ - (2 * NBROFSAMPLE_GET));

    //  if(Xsum&&RTP_ReqNew)
    //      {
    //          DebugPrint("\rX: %d", Xsum);
    //          RTP_ReqNew=0;
    //      }

    return Xsum;
}

uint16_t RTP_Get_Y(void)
{
    uint8_t i, j;
    uint16_t tmpBuf[NBROFSAMPLE_READ];
    uint32_t Ysum = 0;
    uint16_t tmp;
    uint8_t index;

    /*Get sample*/
    for (i = 0; i < NBROFSAMPLE_READ; i++)
    {
        tmp = RTP_Read_Y();

        //      if(tmp){
        index++;
        tmpBuf[i] = tmp;
        //      }

        //      if(i != 0 &&  ABS(tmpBuf[i] , tmpBuf[i-1]) > TOUCH_ADC_WINDOW){
        //          DebugPrint("\rGet ADC Y: medium: %d", 0);
        //          return 0;
        //      }
    }

    //  if(index < NBROFSAMPLE_READ) return 0;

    /*Sort sample from low to high*/
    for (i = 0; i < NBROFSAMPLE_READ - 1; i++)
    {
        for (j = i + 1; j < NBROFSAMPLE_READ; j++)
        {
            if (tmpBuf[i] > tmpBuf[i])
            {
                tmp = tmpBuf[i];
                tmpBuf[i] = tmpBuf[j];
                tmpBuf[j] = tmp;
            }
        }
    }

    /* Chi lay 5 sample o giua */
    for (i = NBROFSAMPLE_GET; i < NBROFSAMPLE_READ - NBROFSAMPLE_GET; i++)
    {
        Ysum += tmpBuf[i];
    }
    Ysum = Ysum / (NBROFSAMPLE_READ - (2 * NBROFSAMPLE_GET));
    //  if(Ysum)
    //      DebugPrint("\rGet ADC Y: medium: %d", Ysum);
    return Ysum;
}

#define ERR_RANGE 20 // Chap nhan 2 pixel nhay trong khoang +/- 20
uint8_t RTP_ReadXY(uint16_t *X, uint16_t *Y)
{

    uint16_t x1, y1, x2, y2;

    /*Get 2 sample X Y */
    // Get X1,Y1
    x1 = RTP_Get_X();
    y1 = RTP_Get_Y();
    // Valid x1,y1 value
    //  if(x1 == 0 || y1 == 0) return 0;
    //Get X2,Y2
    x2 = RTP_Get_X();
    y2 = RTP_Get_Y();

    if (RTP_ReqNew)
    {
        RTP_ReqNew = 0;
        DebugPrint("\rADC XY: %d/%d-%d/%d", x1, x2, y1, y2);
    }

    //  if(x2 < 500 || y2 < 600) return 0;
    //Valid x2,y2 value
    if (((x2 <= x1 && x1 < x2 + ERR_RANGE) || (x1 <= x2 && x2 < x1 + ERR_RANGE)) //ǰ�����β�����+-ERR_RANGE��
        && ((y2 <= y1 && y1 < y2 + ERR_RANGE) || (y1 <= y2 && y2 < y1 + ERR_RANGE)))
    {
        *X = (x1 + x2) / 2;
        *Y = (y1 + y2) / 2;
        return 1;
    }
    else
        return 0;
}

/*Tick 10ms*/
#define ERR_RATIO 5
TouchPoint_t lastTouchPoint;
uint8_t isTouching = 0;
uint8_t TouchingCnt = 0;

uint8_t RTP_GetTouchPointADC(uint16_t *X, uint16_t *Y)
{
    TouchPoint_t tmpTouchPoint;
    /*Get 1 touch point*/
    if (RTP_ReadXY(&tmpTouchPoint.X, &tmpTouchPoint.Y) == 1)
    {
        /*neu luc truoc chua co touch*/
        if (isTouching == 0)
        {
            lastTouchPoint.X = tmpTouchPoint.X;
            lastTouchPoint.Y = tmpTouchPoint.Y;
            isTouching = 1;
            //          DebugPrint("\rTouch touch touch");
        }
        /*Truoc do da co 1 touch*/
        else
        {
            /*Kiem tra xem touch sau co trong vung cho phep ko*/
            if (ABS(lastTouchPoint.X, tmpTouchPoint.X) * ABS(lastTouchPoint.X, tmpTouchPoint.X) + ABS(lastTouchPoint.Y, tmpTouchPoint.Y) * ABS(lastTouchPoint.Y, tmpTouchPoint.Y) < ERR_RATIO / 2 /** ERR_RATIO*/)
            {

                TouchingCnt++;
                lastTouchPoint.X = tmpTouchPoint.X;
                lastTouchPoint.Y = tmpTouchPoint.Y;
            }
            else
            {
                TouchingCnt = 0;
                isTouching = 0;
                //              DebugPrint("\rTADC: Err ratio:%d,%d/%d,%d",lastTouchPoint.X , lastTouchPoint.Y, tmpTouchPoint.X, tmpTouchPoint.Y);
            }
        }

        if (1) //TouchingCnt >= 3)
        {
            //          DebugPrint("\rTADC:%d:%d",tmpTouchPoint.X , tmpTouchPoint.Y);
            *X = tmpTouchPoint.X;
            *Y = tmpTouchPoint.Y;
            return 1;
        }
    }
    else
    {
        isTouching = 0;
        TouchingCnt = 0;
    }
    return 0;
}

void RTP_ConvertADCtoPosition(uint16_t X_ADC, uint16_t Y_ADC, uint16_t *X, uint16_t *Y)
{

    uint16_t tmpX, tmpY;

    tmpX = X_ADC > xSystem.Parameters.Offset_X_ADC ? (X_ADC - xSystem.Parameters.Offset_X_ADC) * 10 / xSystem.Parameters.Scale_X_ADC : xSystem.Parameters.Offset_X_ADC * 10 / xSystem.Parameters.Scale_X_ADC;

    tmpY = Y_ADC > xSystem.Parameters.Offset_Y_ADC ? (Y_ADC - xSystem.Parameters.Offset_Y_ADC) * 10 / xSystem.Parameters.Scale_Y_ADC : (xSystem.Parameters.Offset_Y_ADC) * 10 / xSystem.Parameters.Scale_Y_ADC;

    *X = tmpX;
    *Y = tmpY;

    //  DebugPrint("\rTouch point: %d : %d", tmpX, tmpY);
    ILI9488_LCD_Fill_Area(tmpX, tmpY, tmpX + 10, tmpY + 10, 0xF800);
}

uint8_t TouchPointCalibStep = 0;
TouchPoint_t TouchPointCalib[4];
void RTP_TouchCalib(void)
{

    switch (TouchPointCalibStep)
    {
    /*Calib TP 0,0*/
    case 0:
        DebugPrint("\rTouchPoint Calib: Cham vao diem 0,0");
        if (RTP_GetTouchPointADC(&TouchPointCalib[0].X, &TouchPointCalib[0].Y))
        {
            xSystem.Parameters.Offset_X_ADC = TouchPointCalib[0].X;
            xSystem.Parameters.Offset_Y_ADC = TouchPointCalib[0].Y;
            TouchPointCalibStep = 1;
        }
        break;
    /*Calib TP 320,480*/
    case 1:
        DebugPrint("\rTouchPoint Calib: Cham vao diem 320,480");
        if (RTP_GetTouchPointADC(&TouchPointCalib[1].X, &TouchPointCalib[1].Y))
        {
            TouchPointCalibStep = 2;
        }
        break;
    /*Calculation scale*/
    case 2:
        xSystem.Parameters.Scale_X_ADC = (TouchPointCalib[1].X - xSystem.Parameters.Offset_X_ADC) * 1000 / 320;
        xSystem.Parameters.Scale_Y_ADC = (TouchPointCalib[1].Y - xSystem.Parameters.Offset_Y_ADC) * 1000 / 480;
        /*Save to FRAM*/
        TouchPointCalibStep = 0;
        break;
    }
}

/*Tick 10ms*/
void RTP_GetTouchPoint_Tick(void)
{

    TouchPoint_t ADC_TouchPoint;
    TouchPoint_t tmp_TouchPoint;
    GUI_PID_STATE State;
    //  if(xSystem.Parameters.Scale_X_ADC == 0xFFFF || xSystem.Parameters.Scale_Y_ADC == 0xFFFF){
    //      RTP_TouchCalib();
    //      return;
    //  }

    if (xSystem.GLStatus.RequestCheckRTP == 0 || xSystem.GLStatus.AlcoholMeasureState == MEASURING)
    {
        xSystem.GLStatus.RequestCheckRTP = 0;
        return;
    }
    xSystem.GLStatus.RequestCheckRTP = 0;

    if (RTP_GetTouchPointADC(&ADC_TouchPoint.X, &ADC_TouchPoint.Y))
    {
        RTP_ConvertADCtoPosition(ADC_TouchPoint.X, ADC_TouchPoint.Y, &tmp_TouchPoint.X, &tmp_TouchPoint.Y);
        //      DebugPrint("\rTouchPoint: %d,%d", ADC_TouchPoint.X, ADC_TouchPoint.Y);

        xSystem.GLStatus.TouchPoint.X = tmp_TouchPoint.X;
        xSystem.GLStatus.TouchPoint.Y = tmp_TouchPoint.Y;

        State.Pressed = 1;
        State.x = tmp_TouchPoint.X;
        State.x = tmp_TouchPoint.Y;

        GUI_TOUCH_StoreState(tmp_TouchPoint.X, tmp_TouchPoint.Y);

        GUI_Exec();
    }
    if (isTouching == 1)
    {
        //  DebugPrint("\rTouching");
    }
    else
    {
        xSystem.GLStatus.TouchPoint.X = 0xFFFF;
        xSystem.GLStatus.TouchPoint.Y = 0xFFFF;
        GUI_TOUCH_StoreState(-1, -1);
    }
}
