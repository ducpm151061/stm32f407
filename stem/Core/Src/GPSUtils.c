/******************************************************************************
 * @file        GPSUtils.c
 * @author
 * @version     V1.0.0
 * @date        10/11/2014
 * @brief
 ******************************************************************************/

/******************************************************************************
                                   INCLUDES
 ******************************************************************************/
#include "GPS.h"
#include "Utilities.h"
#include <stdlib.h>
#include "string.h"

/******************************************************************************
                                   GLOBAL VARIABLES
 ******************************************************************************/
GPS_Manager_t GPS_Manager;
extern System_t xSystem;

/******************************************************************************
                                   GLOBAL FUNCTIONS
 ******************************************************************************/

/******************************************************************************
                                   DATA TYPE DEFINE
 ******************************************************************************/

/******************************************************************************
                                   PRIVATE VARIABLES
 ******************************************************************************/
static SmallBuffer_t GPSMessageBuffer;

/******************************************************************************
                                   LOCAL FUNCTIONS
 ******************************************************************************/

/*****************************************************************************/
/**
 * @brief   :   Nhan du lieu tu module GPS
 * @param   :
 * @retval  :
 * @author  :
 * @created :   10/11/2014
 * @version :
 * @reviewer:
 */
void GetDataFromGPSModule(uint8_t gpsdata)
{
    // Bỏ qua dữ liệu không có nghĩa
    if (gpsdata == 0x00 || gpsdata >= 0x7F)
        return;

    if (GPS_Manager.DataBuffer.BufferIndex >= GPS_BUFFER_SIZE)
        GPS_Manager.DataBuffer.BufferIndex = GPS_Manager.DataBuffer.BufferIndex / 2;

    GPS_Manager.DataBuffer.Buffer[GPS_Manager.DataBuffer.BufferIndex++] = gpsdata;
    GPS_Manager.DataBuffer.Buffer[GPS_Manager.DataBuffer.BufferIndex] = 0;
    GPS_Manager.DataBuffer.State = 2;
}

/*****************************************************************************/
/**
 * @brief   :   Valid checksum from GPS message
 * @param   :
 * @retval  :
 * @author  :
 * @created :   10/11/2014
 * @version :
 * @reviewer:
 */
uint8_t ValidCheckSum(uint8_t *GPSMessageBuffer)
{
    uint8_t GPSCRC = 0;
    uint8_t MessageCRC = 0;
    uint8_t i = 0;

    //Tinh checksum
    MessageCRC = 0;
    for (i = 1; i < MAX_GPRMC_BUFFER; i++)
    {
        if (GPSMessageBuffer[i] == '*')
        {
            MessageCRC = 0xFF; //Bien tam
            break;
        }

        GPSCRC ^= GPSMessageBuffer[i];
    }

    //Ban tin khong co dau *
    if (MessageCRC != 0xFF)
    {
        GPS_Manager.CRCErrorCount++;
        return 0;
    }

    //Lay gia tri checksum trong ban tin
    MessageCRC = GetHexNumberFromString(i + 1, (char *)GPSMessageBuffer) & 0xFF;

    //Dung checksum
    if (MessageCRC == GPSCRC)
    {
        if ((GPSCRC == GPS_Manager.LastCRC) && (GPS_Manager.LastCRC != 0))
        {
            GPS_Manager.CRCErrorCount++;
        }
        else
        {
            GPS_Manager.LastCRC = GPSCRC;
            GPS_Manager.CRCErrorCount = 0;
        }
        return 1;
    }

    return 0;
}
/*****************************************************************************/
/**
 * @brief:      Copy ban tin GPRMC tu bo dem GPS vao bo dem xu ly du lieu
 * @param   :
 * @retval  :
 * @author  :
 * @created :   10/11/2014
 * @version :
 * @reviewer:
 */
void CopyGPSSentence(char *Buffer)
{
    uint8_t i = 0;

    //Copy du lieu vao bo dem xu ly
    while (Buffer[i] != 13 && Buffer[i + 1] != 10)
    {
        GPSMessageBuffer.Buffer[i] = Buffer[i];
        if (++i >= MAX_GPRMC_BUFFER)
            break;
    }

    GPSMessageBuffer.Buffer[i] = 0;
}
/*****************************************************************************
 * $GPRMC,053740.000,A,2503.6319,N,12136.0099,E,2.69,79.65,100106,,,A*53
 * $GPRMC,120650.000,A,2058.472447,N,10550.829105,E,0.000,0,071012,,,A*7A  //SIM908
 * $GPRMC,044518.0,A,2058.441820,N,10550.844794,E,0.0,119.2,170914,,,A*64 // UC20
 * RMC Data Format
 * Name                 Example             Units           Description
 * Message ID           $GPRMC                              RMC protocol header
 * UTC Time             053740.000                          hhmmss.sss
 * Status               A                                   A=data valid or V=data not valid
 * Latitude             2503.6319                           ddmm.mmmm
 * N/S Indicator        N                                   N=north or S=south
 * Longitude            12136.0099                          dddmm.mmmm
 * E/W Indicator        E                                   E=east or W=west
 * Speed over ground    2.69                knots           True
 * Course over ground   79.65               degrees
 * Date                 100106                              ddmmyy
 * Magnetic variation                       degrees
 * Variation sense                                          E=east or W=west (Not shown)
 * Mode                 A                                   A=autonomous, D=DGPS, E=DR
 * Checksum             *53
 * <CR> <LF>                                                End of message termination
 *
 */
uint8_t DecodeRMC(void)
{
    uint8_t Pos = 0;
    uint8_t i = 0;
    uint32_t temp = 0;
    uint8_t Length = 0;
    float tempf = 0;
    GPS_Manager_t GPSTemp;
    char GPSConvert[12];
    char *pBuffer;

    //Kiểm tra định dạng bản tin
    pBuffer = strstr((char *)GPS_Manager.DataBuffer.Buffer, "$GPRMC");
    if (pBuffer == NULL)
        pBuffer = strstr((char *)GPS_Manager.DataBuffer.Buffer, "$GNRMC");
    if (pBuffer == NULL)
        return 0;

    CopyGPSSentence(pBuffer);

    //Tinh checksum du lieu
    if (ValidCheckSum(GPSMessageBuffer.Buffer) == 0)
        return 0;

    //Bo qua phan $GPRMC dau tien, chay qua day phay thu nhat
    while (GPSMessageBuffer.Buffer[Pos] != ',' && Pos < MAX_GPRMC_BUFFER)
        Pos++;
    Pos++;
    if (Pos >= MAX_GPRMC_BUFFER)
        return 0;

    //Thoi gian
    GPSTemp.Time.Hour = (GPSMessageBuffer.Buffer[Pos] - 48) * 10 + (GPSMessageBuffer.Buffer[Pos + 1] - 48);
    Pos += 2;
    GPSTemp.Time.Minute = (GPSMessageBuffer.Buffer[Pos] - 48) * 10 + (GPSMessageBuffer.Buffer[Pos + 1] - 48);
    Pos += 2;
    GPSTemp.Time.Second = (GPSMessageBuffer.Buffer[Pos] - 48) * 10 + (GPSMessageBuffer.Buffer[Pos + 1] - 48);
    Pos += 2;

    //Valid ?
    while (GPSMessageBuffer.Buffer[Pos] != ',' && Pos < MAX_GPRMC_BUFFER)
        Pos++;
    Pos++;
    if (Pos >= MAX_GPRMC_BUFFER)
        return 0;

    if (GPSMessageBuffer.Buffer[Pos] != 'A')
    {
        GPS_Manager.Valid = 0;

        //Phan tich xong
        return 1;
    }
    if (GPSMessageBuffer.Buffer[Pos] == 'A')
        GPSTemp.Valid = 1;

    //Vi do, bo qua den het dau phay
    while (GPSMessageBuffer.Buffer[Pos] != ',' && Pos < MAX_GPRMC_BUFFER)
        Pos++;
    Pos++;

    //Tinh toa do BUS
    GPSTemp.BusY = 0;
    GPSTemp.BusY += (GPSMessageBuffer.Buffer[Pos + 1] - 48) * 60000;
    GPSTemp.BusY += (GPSMessageBuffer.Buffer[Pos + 2] - 48) * 10000;
    GPSTemp.BusY += (GPSMessageBuffer.Buffer[Pos + 3] - 48) * 1000;
    GPSTemp.BusY += (GPSMessageBuffer.Buffer[Pos + 5] - 48) * 100;
    GPSTemp.BusY += (GPSMessageBuffer.Buffer[Pos + 6] - 48) * 10;
    GPSTemp.BusY += (GPSMessageBuffer.Buffer[Pos + 7] - 48);
    GPSTemp.BusY -= 30000;

    //Dem do dai du lieu, dem den khi gap dau cham
    Length = 0;
    while (GPSMessageBuffer.Buffer[Pos + Length] != '.' && Length < 6)
        Length++;
    if (Length > 5)
        return 0;

    //Xoa du lieu
    GPSTemp.Latitude.value = 0;

    //2 ky tu phan degree
    Length -= 2;
    for (i = 0; i < Length; i++)
    {
        GPSTemp.Latitude.value = GPSTemp.Latitude.value * 10;
        GPSTemp.Latitude.value += GPSMessageBuffer.Buffer[Pos++] - 48;
    }

    //Chuyen doi phan sau thanh so thap phan
    i = 0;
    while (GPSMessageBuffer.Buffer[Pos] != ',' && Pos < MAX_GPRMC_BUFFER)
        GPSConvert[i++] = GPSMessageBuffer.Buffer[Pos++];
    Pos++;
    if (Pos >= MAX_GPRMC_BUFFER)
        return 0;

    GPSConvert[i] = 0;
    tempf = atof(GPSConvert);
    GPSTemp.Latitude.value += (float)tempf / 60;

    //Vi do nam
    if (GPSMessageBuffer.Buffer[Pos] == 'S')
        GPSTemp.Latitude.value = GPSTemp.Latitude.value * -1;

    //Bo qua phuong
    while (GPSMessageBuffer.Buffer[Pos] != ',' && Pos < MAX_GPRMC_BUFFER)
        Pos++;
    Pos++;
    if (Pos >= MAX_GPRMC_BUFFER)
        return 0;

    GPSTemp.BusX = 0;
    GPSTemp.BusX += ((GPSMessageBuffer.Buffer[Pos + 2] - 48) - 5) * 60000;
    GPSTemp.BusX += (GPSMessageBuffer.Buffer[Pos + 3] - 48) * 10000;
    GPSTemp.BusX += (GPSMessageBuffer.Buffer[Pos + 4] - 48) * 1000;
    GPSTemp.BusX += (GPSMessageBuffer.Buffer[Pos + 6] - 48) * 100;
    GPSTemp.BusX += (GPSMessageBuffer.Buffer[Pos + 7] - 48) * 10;
    GPSTemp.BusX += (GPSMessageBuffer.Buffer[Pos + 8] - 48);
    GPSTemp.BusX -= 20000;

    //Dem do dai du lieu, dem den khi gap dau cham
    Length = 0;
    while (GPSMessageBuffer.Buffer[Pos + Length] != '.' && Length < 6)
        Length++;
    if (Length > 5)
        return 0;

    //Xoa du lieu
    GPSTemp.Longitude.value = 0;

    //2 ky tu phan degree
    Length -= 2;
    for (i = 0; i < Length; i++)
    {
        GPSTemp.Longitude.value = GPSTemp.Longitude.value * 10;
        GPSTemp.Longitude.value += GPSMessageBuffer.Buffer[Pos++] - 48;
    }

    i = 0;
    while (GPSMessageBuffer.Buffer[Pos] != ',' && Pos < MAX_GPRMC_BUFFER)
        GPSConvert[i++] = GPSMessageBuffer.Buffer[Pos++];
    Pos++;

    //Chuyen doi phan kinh do
    GPSConvert[i] = 0;
    tempf = atof(GPSConvert);
    GPSTemp.Longitude.value += (float)tempf / 60;

    //Chuyen doi phuong
    if (GPSMessageBuffer.Buffer[Pos] == 'W')
        GPSTemp.Longitude.value = GPSTemp.Longitude.value * -1;

    //Bo qua phuong
    while (GPSMessageBuffer.Buffer[Pos] != ',' && Pos < MAX_GPRMC_BUFFER)
        Pos++;
    Pos++;
    if (Pos >= MAX_GPRMC_BUFFER)
        return 0;

    //Tinh van toc tu Knot sang km/h
    temp = GetNumberFromString(Pos, (char *)(GPSMessageBuffer.Buffer));
    temp = temp * 100;

    while (GPSMessageBuffer.Buffer[Pos] != '.' && Pos < MAX_GPRMC_BUFFER)
        Pos++;
    Pos++;
    temp += GetNumberFromString(Pos, (char *)(GPSMessageBuffer.Buffer));
    temp = temp * 13;
    temp = temp / 702;
    GPSTemp.Speed = temp & 0xFFFF;

    // COG
    while (GPSMessageBuffer.Buffer[Pos] != ',' && Pos < MAX_GPRMC_BUFFER)
        Pos++;
    Pos++;

    i = 0;
    while (GPSMessageBuffer.Buffer[Pos] != ',' && Pos < MAX_GPRMC_BUFFER)
        GPSConvert[i++] = GPSMessageBuffer.Buffer[Pos++];

    GPSConvert[i] = 0;
    tempf = atof(GPSConvert);
    GPSTemp.Course = (uint16_t)tempf;

    if (Pos >= MAX_GPRMC_BUFFER)
        return 0;

    //Ngay thang
    while (GPSMessageBuffer.Buffer[Pos] != ',' && Pos < MAX_GPRMC_BUFFER)
        Pos++;
    Pos++;
    GPSTemp.Time.Day = (GPSMessageBuffer.Buffer[Pos] - 48) * 10 + (GPSMessageBuffer.Buffer[Pos + 1] - 48);
    Pos += 2;
    GPSTemp.Time.Month = (GPSMessageBuffer.Buffer[Pos] - 48) * 10 + (GPSMessageBuffer.Buffer[Pos + 1] - 48);
    Pos += 2;
    GPSTemp.Time.Year = (GPSMessageBuffer.Buffer[Pos] - 48) * 10 + (GPSMessageBuffer.Buffer[Pos + 1] - 48);
    Pos += 2;

    //Van toc qua cao thi bo luon du lieu
    if (GPSTemp.Speed >= 125)
    {
        GPSTemp.Valid = 0;

        //Phan tich xong
        return 0;
    }

    //Copy du lieu vao cau truc chinh luu du lieu xu ly
    GPS_Manager.Longitude.value = GPSTemp.Longitude.value;
    GPS_Manager.Latitude.value = GPSTemp.Latitude.value;

#ifdef __BUS_HN__
    GPS_Manager.BusX = GPSTemp.BusX;
    GPS_Manager.BusY = GPSTemp.BusY;
#else
    GPS_Manager.BusX = GPSTemp.Longitude.value * 100000;
    GPS_Manager.BusY = GPSTemp.Latitude.value * 100000;
#endif

    GPS_Manager.Time.Year = GPSTemp.Time.Year;
    GPS_Manager.Time.Month = GPSTemp.Time.Month;
    GPS_Manager.Time.Day = GPSTemp.Time.Day;
    GPS_Manager.Time.Hour = GPSTemp.Time.Hour;
    GPS_Manager.Time.Minute = GPSTemp.Time.Minute;
    GPS_Manager.Time.Second = GPSTemp.Time.Second;

    GPS_Manager.Speed = GPSTemp.Speed;
    GPS_Manager.Course = GPSTemp.Course;
    GPS_Manager.Valid = 100;

    return 1;
}

// $GPGGA,094627.000,2058.4630,N,10550.7995,E,1,10,0.82,11.1,M,-20.6,M,,*7C
uint8_t DecodeGGA(void)
{
    char *pBuffer;
    uint8_t i, count = 0;
    ;

    GPS_Manager.NbOfSV = 0;

    //Kiểm tra định dạng bản tin
    pBuffer = strstr((char *)GPS_Manager.DataBuffer.Buffer, "$GPGGA");
    if (pBuffer == NULL)
        pBuffer = strstr((char *)GPS_Manager.DataBuffer.Buffer, "$GNGGA");
    if (pBuffer == NULL)
        return 0;

    CopyGPSSentence(pBuffer);

    //Tinh checksum du lieu
    if (ValidCheckSum(GPSMessageBuffer.Buffer) == 0)
        return 0;

    for (i = 0; i < MAX_GPRMC_BUFFER; i++)
    {
        if (pBuffer[i] == ',')
            count++;
        if (count == 7)
        {
            GPS_Manager.NbOfSV = GetNumberFromString(i + 1, pBuffer);
            break;
        }
    }

    return 1;
}

/*****************************************************************************/
/**
  * @brief  Update date time for RTC from GPS
  * @param  None
  * @retval None
  */
void UpdateTimeFromGPS(void)
{
    uint8_t UpdateEnable = 0;
    static uint8_t UpdateTick = 0;

    UpdateTick++;

    if (xSystem.Rtc->GetDateTime().Year > 17 && GPS_Manager.Time.Year > 30)
        return;

    if (UpdateTick >= 30)
    {
        if (GPS_Manager.Valid)
        {
            UpdateEnable = 1;
        }

        UpdateTick = 0;
    }

    if (GPS_Manager.Valid && xSystem.Rtc->GetDateTime().Year < 17)
    {
        UpdateEnable = 1;
    }

    if ((UpdateEnable) && (xSystem.GLStatus.SyncTimeGps == 1))
    {
        DebugPrint("\rCap nhat thoi gian tu GPS : %u %u %u %u %u %u \r\n",
                   GPS_Manager.Time.Hour, GPS_Manager.Time.Minute, GPS_Manager.Time.Second,
                   GPS_Manager.Time.Day, GPS_Manager.Time.Month, GPS_Manager.Time.Year);

        // Convert to GMT +7. 25200 = 7 * 3600
        xSystem.Rtc->SetDateTime(GPS_Manager.Time, 25200);
        // xSystem.GLStatus.SyncTimeGps == 0;
        xSystem.GLStatus.DoneSync = 1;
    }
}
/********************************* END OF FILE *******************************/
