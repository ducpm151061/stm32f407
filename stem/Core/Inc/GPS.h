#ifndef __GPS_H__
#define __GPS_H__

#include "DataDefine.h"

#define MAX_GPRMC_BUFFER 80
#if (DEBUG_GPS > 0)
#define GPS_BUFFER_SIZE 650
#else
#define GPS_BUFFER_SIZE 200
#endif

typedef struct
{
    uint8_t Buffer[GPS_BUFFER_SIZE];
    uint16_t BufferIndex;
    uint8_t State;
} GPS_Buffer_t;

typedef struct
{
    Float_t Longitude; //Kinh do
    Float_t Latitude;  //Vi do

    uint32_t BusX;
    uint32_t BusY;
    uint32_t Simu_ToaDoX;
    uint32_t Simu_ToaDoY;
    uint8_t BusSimulator;

    DateTime_t Time; // ThoiGian
    uint8_t Speed;   // Van toc GPS
    uint16_t Course; // Huong di chuyen
    uint8_t NbOfSV;  // So ve tinh quan sat duoc

    uint8_t ConnectTimeout; // Theo doi ket noi GPS, thong bao GPS hong khi khong co ket noi
    uint8_t Valid;          // Co valid hay khong
    uint8_t DebugMode;      // Co phai trong che do debug hay khong

    uint8_t LastCRC; // Co bao CRC cuoi cung nhan duoc

    // Co bao loi tinh CRC, neu CRC loi, hoac trung CRC
    // voi ban tin truoc thi tang them 1 don vi
    uint8_t CRCErrorCount;
    GPS_Buffer_t DataBuffer;

} GPS_Manager_t;

void GetDataFromGPSModule(uint8_t gpsdata);
void InitGPS(void);
void GPS_ManagerTick(void);
uint8_t DecodeRMC(void);
uint8_t DecodeGGA(void);

DateTime_t GPS_GetDateTime(void);
void UpdateTimeFromGPS(void);

uint32_t GetBusLongitude(void);
uint32_t GetBusLatitude(void);

void GpsSimulationToggle(void);
void SendCommandToGPS(char *Cmd);

//#warning "Dang debug GPS"
//void SwtichBaudrateGPS(uint32_t Baudrate);

#endif // __GPS_H__
