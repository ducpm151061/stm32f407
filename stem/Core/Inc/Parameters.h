#ifndef __PARAMETERS_H__
#define __PARAMETERS_H__

#include "DataDefine.h"
#include "AlcoholMeter.h"
#include "FlashInternal.h"

/*
	0x08000000	- Start BootProgram
	0x08004000	- Start Parameter
	0x08008000	- Start Application Program
*/

#define PARAMETERS_BASE (uint32_t)0x080E0000 // Move config to last sector

#define DEVICE_SERIAL_ADDR PARAMETERS_BASE
#define DEVICE_SERIAL_LEN 32
#define DEVICE_SERIAL_DF "AlM_2012001"

#define DEVICE_PRODUCTDATE_ADDR DEVICE_SERIAL_ADDR + DEVICE_SERIAL_LEN
#define DEVICE_PRODUCTDATE_LEN 12
#define DEVICE_PRODUCTDATE_DF "2021/05/10"

#define DEVICE_CALIBDATE_ADDR DEVICE_PRODUCTDATE_ADDR + DEVICE_PRODUCTDATE_LEN
#define DEVICE_CALIBDATE_LEN 12
#define DEVICE_CALIBDATE_DF "2021/05/10"

#define DEVICE_RESDATE_ADDR DEVICE_CALIBDATE_ADDR + DEVICE_CALIBDATE_LEN
#define DEVICE_RESDATE_LEN 12

#define DEVICE_BLOWTIME_ADDR DEVICE_RESDATE_ADDR + DEVICE_RESDATE_LEN
#define DEVICE_BLOWTIME_LEN 4

#define DEVICE_BLOWPRESS_ADDR DEVICE_BLOWTIME_ADDR + DEVICE_BLOWTIME_LEN
#define DEVICE_BLOWPRESS_LEN 4

#define DEVICE_TIMEOFF_ADDR DEVICE_BLOWPRESS_ADDR + DEVICE_BLOWPRESS_LEN
#define DEVICE_TIMEOFF_LEN 4

#define DEVICE_KEYTO_ADDR DEVICE_TIMEOFF_ADDR + DEVICE_TIMEOFF_LEN
#define DEVICE_KEYTO_LEN 4

#define DEVICE_PASSKEY_ADDR DEVICE_KEYTO_ADDR + DEVICE_KEYTO_LEN
#define DEVICE_PASSKEY_LEN 8

#define DEVICE_PARA5_ADDR DEVICE_PASSKEY_ADDR + DEVICE_PASSKEY_LEN
#define DEVICE_PARA5_LEN 40

#define DEVICE_ALM_CALIB_ADDR DEVICE_PARA5_ADDR + DEVICE_PARA5_LEN
#define DEVICE_ALM_CALIB_LEN 20 * 4

#define MEM_END_ADDR PARAMETERS_BASE

#define CFG_ResetDevice 0
#define CFG_DeviceSerial 1		 // Serial cua thiet bi
#define CFG_Device_ProductDate 2 // Ngay San Xuat
#define CFG_Device_CalibDate 3	 // Ngay hieu chuan
#define CFG_Device_ResDate 4	 // Ngay Du Phong
#define CFG_BlowTime 5			 // Do Dai Thoi Gian thoi truoc khi lay mau
#define CFG_BlowPressure 6		 // Ap luc de tinh dang thoi hoi
#define CFG_TimeAutoTurnOff 7	 // Thoi gian tu dong tat may
#define CFG_TimeoutKey 8		 // Thoi gian tu dong tat may
#define CFG_SetupKey 9			 // Key cai dat nang cao

#define CFG_ALCalib_Table 20 //

void ReadConfig(uint8_t ParamNum);
void SaveConfigToFlash(void);
uint8_t GetConfig(uint8_t ParamNum, uint8_t *Buff);
void ProcessGetCFG(uint8_t sock, uint8_t Source, uint8_t *Buff, uint8_t RequireResponse, uint8_t *Response, uint8_t *ptrLen);
uint8_t Param_Set_Cfg(uint8_t MaLenh, uint8_t *Buffer, uint8_t ResponseRequire, char *BufferToResponse, uint8_t *ptrLenght);
extern uint8_t SendSMS(char *PhoneNumber, char *Message);
extern uint8_t rtc_settime(DateTime_t *rtc, uint32_t HieuChinhMuiGio);

extern void RF_GateWay_ProcessData(uint8_t Source, uint8_t *Str);
extern void TCP_SendData(uint8_t sock, uint8_t *Buffer, uint8_t Len);
extern void FLASH_PRG_Loop_Data(void);
extern void FLASH_READ_Loop_Data(void);

void Parameter_Tick(void);

#endif // __PARAMETERS_H__
