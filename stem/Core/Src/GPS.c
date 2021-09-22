/******************************************************************************
 * @file        GPS.c
 * @author
 * @version     V1.0.0
 * @date        10/11/2014
 * @brief
 ******************************************************************************/

/******************************************************************************
                                   INCLUDES
 ******************************************************************************/
#include "GPS.h"
#include "Hardware.h"
#include "DriverUART.h"

#include "Utilities.h"

/******************************************************************************
                                   GLOBAL VARIABLES
 ******************************************************************************/
extern GPS_Manager_t GPS_Manager;
extern System_t xSystem;
extern uint8_t PublicBuffer[200];

/******************************************************************************
                                   GLOBAL FUNCTIONS
 ******************************************************************************/

/******************************************************************************
                                   DATA TYPE DEFINE
 ******************************************************************************/
#if (DEBUG_GPS_ROUTE == 1)

#define GPS_SIMULATION_TICK 2000 // Typical 10s
#define GPS_SIMULATION_LENGTH 280

static const float GpsSimulationData[GPS_SIMULATION_LENGTH] =
    {
        106.727203, 20.778078, 53, 0, 106.718918, 20.772717, 73, 0, 106.712349, 20.770863, 25, 0, 106.700752, 20.767405, 106, 0, 106.683846, 20.765259, 106, 0,
        106.666718, 20.765533, 106, 0, 106.649597, 20.765875, 107, 0, 106.632729, 20.768604, 106, 0, 106.615791, 20.770687, 106, 0, 106.59864, 20.770758, 106, 0,
        106.581947, 20.773127, 107, 0, 106.567528, 20.781769, 106, 0, 106.553108, 20.790464, 107, 0, 106.53833, 20.798599, 106, 0, 106.53569, 20.799601, 106, 0,
        106.506409, 20.810341, 106, 0, 106.474388, 20.821711, 107, 0, 106.46904, 20.82357, 106, 0, 106.442238, 20.832829, 107, 0, 106.425949, 20.837769, 107, 0,
        106.42038, 20.83885, 106, 0, 106.392242, 20.843496, 106, 0, 106.375702, 20.84778, 106, 0, 106.359802, 20.853722, 106, 0, 106.343445, 20.858389, 107, 0,
        106.326385, 20.857714, 107, 0, 106.309433, 20.857147, 107, 0, 106.292641, 20.860266, 106, 0, 106.275795, 20.863321, 106, 0, 106.258698, 20.863192, 106, 0,
        106.241577, 20.862019, 106, 0, 106.224503, 20.860849, 106, 0, 106.207863, 20.857391, 107, 0, 106.191963, 20.854637, 106, 0, 106.174896, 20.85511, 106, 0,
        106.157845, 20.854998, 106, 0, 106.140976, 20.852383, 106, 0, 106.124039, 20.850443, 107, 0, 106.10704, 20.849075, 106, 0, 106.090065, 20.847271, 106, 0,
        106.073013, 20.847521, 103, 0, 106.05603, 20.845543, 106, 0, 106.03936, 20.848055, 106, 0, 106.026161, 20.858192, 106, 0, 106.014206, 20.869095, 107, 0,
        106.002724, 20.881035, 107, 0, 105.993462, 20.894556, 106, 0, 105.984589, 20.908276, 106, 0, 105.976799, 20.922291, 92, 0, 105.972046, 20.931345, 38, 0,
        105.970032, 20.934717, 68, 0, 105.96357, 20.947927, 106, 0, 105.955956, 20.962301, 107, 0, 105.948189, 20.976696, 106, 0, 105.938553, 20.99, 107, 0,
        105.927559, 21.001991, 86, 0, 105.9217, 21.00881, 75, 0, 105.966187, 20.942533, 106, 0, 105.958763, 20.957056, 106, 0, 105.951027, 20.971422, 107, 0,
        105.942513, 20.98539, 106, 0, 105.931473, 20.997715, 106, 0, 105.923233, 21.007145, 68, 0, 105.924286, 21.019163, 59, 0, 105.929749, 21.02574, 51, 0,
        //105.929077,21.024982,0,0,105.923714,21.027073,43,0,105.916512,21.029366,41,0,105.912735,21.030582,30,0,105.909401,21.032179,8,0
        105.929077, 21.024982, 100, 0, 105.923714, 21.027073, 105, 0, 105.916512, 21.029366, 110, 0, 105.912735, 21.030582, 100, 0, 105.909401, 21.032179, 110, 0};

static uint16_t GpsSimulationIndex = 0xFFFF;

struct
{
    float KinhDo;
    float ViDo;
    uint16_t Course;
    uint8_t Speed;
} GpsSimulationPosition = {0, 0, 0, 0};

static void GPS_SimulationTick(void);

#endif

/******************************************************************************
                                   PRIVATE VARIABLES
 ******************************************************************************/
char GPS_Version[30];

/******************************************************************************
                                   LOCAL FUNCTIONS
 ******************************************************************************/
static void GPS_ResetTick(uint8_t ResetFromBeginning);

/*****************************************************************************/
/**
 * @brief   :   Tick every 100ms
 * @param   :
 * @retval  :
 * @author  :
 * @created :   10/11/2014
 * @version :
 * @reviewer:
 */

void GPS_IRQHandler(void)
{
    uint8_t gpsdata;
    if (USART_GetITStatus(GPS_UART, USART_IT_RXNE) != RESET)
    {
        gpsdata = USART_ReceiveData(GPS_UART);
        USART_ClearITPendingBit(GPS_UART, USART_IT_RXNE);

        // B? qua d? li?u kh�ng c� nghia
        if (gpsdata == 0x00 || gpsdata >= 0x7F)
            return;

        GPS_Manager.DataBuffer.Buffer[GPS_Manager.DataBuffer.BufferIndex++ % GPS_BUFFER_SIZE] = gpsdata;
        GPS_Manager.DataBuffer.Buffer[GPS_Manager.DataBuffer.BufferIndex % GPS_BUFFER_SIZE] = 0;
        GPS_Manager.DataBuffer.State = 2;
    }
}
static void GPS_Tick(void)
{
    uint8_t ret;
    static uint8_t ConfigNMEA = 45;
    static uint16_t GPS_ResetCount = 0;
    static uint16_t TimeOutUpdateTimeFromGPS = 0;

    if (GPS_Manager.DataBuffer.State > 0)
    {
        GPS_Manager.DataBuffer.State--;
        if (GPS_Manager.DataBuffer.State == 0)
        {
            //DebugPrint("\r%s",GPS_Manager.DataBuffer.Buffer);
            ret = DecodeRMC();

            DecodeGGA();

            GPS_Manager.ConnectTimeout = 100; // Timeout 10s
            GPS_Manager.DataBuffer.BufferIndex = 0;
            memset(GPS_Manager.DataBuffer.Buffer, 0, GPS_BUFFER_SIZE);
        }
    }

    if (ConfigNMEA > 0)
    {
        switch (ConfigNMEA)
        {
        case 40:
            //              SendCommandToGPS("$PMTK605*31\r\n");
            SendCommandToGPS("$PMTK353,1,1,0,0,0*2A\r\n");
            break;
        case 30:
            SendCommandToGPS("$PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28\r\n");
            break;
        case 20:
            SendCommandToGPS("$PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28\r\n");
            break;
        case 10:
            SendCommandToGPS("$PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28\r\n");
            break;
        case 1:
            SendCommandToGPS("$PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28\r\n");

            break;
        }

        ConfigNMEA--;
    }

    if (GPS_Manager.Valid > 0)
        GPS_Manager.Valid--;
    if (GPS_Manager.ConnectTimeout > 0)
        GPS_Manager.ConnectTimeout--;

    // Reset module GPS khi mat GPS hoac mat ket noi qua lau
    if (GPS_Manager.Valid == 0 || GPS_Manager.ConnectTimeout == 0)
        GPS_ResetCount++;
    else
        GPS_ResetCount = 0;

    // Sau 180s ma ko co GPS thi thuc hien reset GPS
    if (GPS_ResetCount >= 1800)
    {
        ConfigNMEA = 45;
        GPS_ResetCount = 0;
        GPS_ResetTick(1);
        return;
    }

    if (++TimeOutUpdateTimeFromGPS > 30 * 10)
    {
        TimeOutUpdateTimeFromGPS = 0;
        if ((GPS_Manager.Valid != 0) && (xSystem.GLStatus.SyncTimeGps == 1))
        // DebugPrint("\rSet Date Time from GPS\r\n");
        {
            xSystem.Rtc->SetDateTime(GPS_Manager.Time, 7 * 60 * 60);
            // xSystem.GLStatus.SyncTimeGps = 0;
            xSystem.GLStatus.DoneSync = 1;
        }
    }

    GPS_ResetTick(0);
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
void InitGPS(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Init GSM Hardware */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Pin = GPS_PWR_PIN;
    GPIO_Init(GPS_PWR_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPS_RESET_PIN;
    GPIO_Init(GPS_RESET_PORT, &GPIO_InitStructure);

    GPIO_SetBits(GPS_PWR_PORT, GPS_PWR_PIN);
    GPIO_ResetBits(GPS_RESET_PORT, GPS_RESET_PIN);

    InitUART(GPS_UART, 9600); // 9600
}

/*****************************************************************************/
/**
 * @brief   :   Reset GPS Module
 * @param   :
 * @retval  :
 * @author  :
 * @created :   15/01/2014
 * @version :
 * @reviewer:
 */
static void GPS_ResetTick(uint8_t ResetFromBeginning)
{
    static uint8_t GPS_ResetState = 2;

    if (ResetFromBeginning)
    {
        GPS_ResetState = 0;
        DebugPrint("\rThuc hien reset module GPS\r\n");
    }

    switch (GPS_ResetState)
    {
    case 0:
        GPIO_ResetBits(GPS_PWR_PORT, GPS_PWR_PIN);
        GPIO_SetBits(GPS_RESET_PORT, GPS_RESET_PIN);
        GPS_ResetState++;
        break;
    case 1:
        GPS_ResetState++;
        break;
    case 2:
        GPIO_SetBits(GPS_PWR_PORT, GPS_PWR_PIN);
        GPIO_ResetBits(GPS_RESET_PORT, GPS_RESET_PIN);
        GPS_ResetState++;
        break;
    }
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
Point_t GetPosition(void)
{
    Point_t tmpPoint;

    if (GPS_Manager.Valid == 0)
    {
        tmpPoint.KinhDo.value = 0;
        tmpPoint.ViDo.value = 0;
    }
    else
    {
        tmpPoint.KinhDo = GPS_Manager.Longitude;
        tmpPoint.ViDo = GPS_Manager.Latitude;
    }
    return tmpPoint;
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
uint32_t GetBusLongitude(void)
{
    if (GPS_Manager.BusSimulator == 1)
    {
        return GPS_Manager.Simu_ToaDoX;
    }

    if (GPS_Manager.Valid == 0)
        return 0;
    return GPS_Manager.BusX;
}

uint32_t GetBusLatitude(void)
{
    if (GPS_Manager.BusSimulator == 1)
    {
        return GPS_Manager.Simu_ToaDoY;
    }

    if (GPS_Manager.Valid == 0)
        return 0;
    return GPS_Manager.BusY;
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
uint8_t GetSpeed(void)
{
#if (DEBUG_GPS_ROUTE == 1)
    return GpsSimulationPosition.Speed;
#else
    uint8_t rtnValue = 0;

    if (GPS_Manager.BusSimulator == 1)
    {
        return 20;
    }

    if (GPS_Manager.Valid > 0)
        rtnValue = GPS_Manager.Speed;

    return rtnValue;
#endif
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
DateTime_t GetDateTime(void)
{
    DateTime_t rtnValue = {0, 0, 0, 0, 0, 0};

    if (GPS_Manager.Valid > 0)
    {
        rtnValue.Day = GPS_Manager.Time.Day;
        rtnValue.Month = GPS_Manager.Time.Month;
        rtnValue.Year = GPS_Manager.Time.Year;
        rtnValue.Hour = GPS_Manager.Time.Hour;
        rtnValue.Minute = GPS_Manager.Time.Minute;
        rtnValue.Second = GPS_Manager.Time.Second;
    }

    return rtnValue;
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
uint16_t GetGpsCourse(void)
{
#if (DEBUG_GPS_ROUTE == 1)
    return GpsSimulationPosition.Course;
#else
    uint16_t rtnValue = 0;

    if (GPS_Manager.Valid > 0)
        rtnValue = GPS_Manager.Course;

    return rtnValue;
#endif
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
uint8_t GetSV(void)
{
    return GPS_Manager.NbOfSV;
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
uint8_t IsValid(void)
{
#if (DEBUG_GPS_ROUTE == 1)
    if (GpsSimulationIndex == 0xFFFF)
        return (GPS_Manager.Valid > 0);
    else
        return 1;
#else
    if (GPS_Manager.BusSimulator == 1)
    {
        return 1;
    }

    return (GPS_Manager.Valid > 0);
#endif
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
uint8_t IsError(void)
{
    return (GPS_Manager.ConnectTimeout == 0);
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
char *GetVersion(void)
{
    if (GPS_Manager.Valid > 0)
        return GPS_Version;

    return NULL;
}

void SendCommandToGPS(char *Cmd)
{
    UART_Puts(GPS_UART, Cmd);

#if (DEBUG_GPS > 0)
    UART_Putb(DEBUG_UART, (uint8_t *)Cmd, strlen(Cmd));
#endif
}

//static void SwtichBaudrateGPS(uint32_t Baudrate)
//{
//  if (Baudrate == 9600)
//  {
//      InitUART(GPS_UART, 115200);
//      SendCommandToGPS("$PMTK251,9600*17\r\n");
//
//      InitUART(GPS_UART, 9600);
//  }
//  else if (Baudrate == 115200)
//  {
//      InitUART(GPS_UART, 9600);
//      SendCommandToGPS("$PMTK251,115200*1F\r\n");
//
//      InitUART(GPS_UART, 115200);
//  }
//}

#if (DEBUG_GPS_ROUTE == 1)
void GpsSimulationToggle(void)
{
    if (GpsSimulationIndex == 0xFFFF)
        GpsSimulationIndex = 0;
    else
        GpsSimulationIndex = 0xFFFF;

    DebugPrint("\r%s mo phong toa do GPS\r\n", (GpsSimulationIndex == 0xFFFF) ? "Tat" : "Bat");
}
#endif

GPS_State_Typedef GetState(void)
{
    if (IsValid() != 0)
        return GPS_ST_OK;
    else if (IsError())
        return GPS_ST_ERR;
    else
        return GPS_ST_POOR;
}

GPS_PERIPHERAL Driver_GPS =
    {
        InitGPS,
        GPS_Tick,
        GetPosition,
        GetBusLongitude,
        GetBusLatitude,
        GetSpeed,
        GetDateTime,
        GetGpsCourse,
        GetSV,
        IsValid,
        IsError,
        GetVersion,
        GetState,
};

/********************************* END OF FILE *******************************/
