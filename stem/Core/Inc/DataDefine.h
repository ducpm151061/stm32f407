#ifndef __DATA_DEFINE_H__
#define __DATA_DEFINE_H__

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "Hardware.h"
#include "AlcoholMeter.h"
#include "GUI.h"
#include "DIALOG.h"

#define ABS(A, B) (A > B ? A - B : B - A)
#define MAX(A, B) (A > B ? A : B)
#define MIN(A, B) (A < B ? A : B)
extern void UART_Printf(USART_TypeDef *USARTx, const char *str, ...);
// #define DebugPrint(String...) UART_Printf(DEBUG_UART, String)

#define DebugPrint(...)

#define BUFFER_STATE_BUSY 1		  // Trang thai dang them du lieu
#define BUFFER_STATE_IDLE 2		  // Trang thai cho
#define BUFFER_STATE_PROCESSING 3 // Trang thai du lieu dang duoc xu ly

#define NUM_OF_GPRS_BUFFER 5
#define LARGE_BUFFER_SIZE 1300
#define SMALL_BUFFER_SIZE 200
#define KPI_DELAY_MS 70

typedef enum
{
	BATTERY,
	PRESSURE_SENSOR,
	TEMPERATURE_SENSOR,
	VIN,
	VCC_AIR_PUMP,
	V8_2
} ADC_Table_e;

typedef enum
{
	EVENT_ERROR,
	EVENT_OK,
	EVENT_DONE
} Event_e;

typedef enum
{
	NO_MEASURE,
	MEASURING,
	MEASURE_DONE,
	MEASURE_FAIL,
} MeasureState_e;

typedef enum
{
	SENSOR_IDLE,
	SENSOR_CLEAN,
	MEASURE_ZERO_POINT,
	BLOW_DETECT,
	MEASURE_SAMPLE,
	CLEAN_SENSOR,
	SENSOR_AIR_PULL,
	SENSOR_AIR_PUSH,
} Al_State_e;

typedef struct
{
	uint8_t Year;
	uint8_t Month;
	uint8_t Day;
	uint8_t Hour;
	uint8_t Minute;
	uint8_t Second;
} DateTime_t;

typedef union
{
	float value;
	uint8_t bytes[4];
	float value1;
} Float_t;

typedef union
{
	uint16_t peak;
	uint8_t bytes[2];
} Int1_t;
typedef union
{
	uint32_t value;
	uint8_t bytes[4];
} Long_t;

typedef union
{
	uint16_t value;
	uint8_t bytes[2];
} Int_t;

typedef struct PeakData
{
	uint16_t maxPeak;
	uint16_t peak;
	uint16_t max90;
} PeakData_t;

typedef struct
{
	uint16_t X;
	uint16_t Y;
	uint8_t State;
} TouchPoint_t;

typedef struct
{
	uint8_t CurrentWindowID;
	uint8_t NextWindowID;
	void (*Function)(void);
	uint8_t State;
} GUI_Manager_t;

typedef struct
{
	uint32_t Bit0 : 1;
	uint32_t Bit1 : 1;
	uint32_t Bit2 : 1;
	uint32_t Bit3 : 1;
	uint32_t Bit4 : 1;
	uint32_t Bit5 : 1;
	uint32_t Bit6 : 1;
	uint32_t Bit7 : 1;
	uint32_t Bit8 : 1;
	uint32_t Bit9 : 1;
	uint32_t Bit10 : 1;
	uint32_t Bit11 : 1;
	uint32_t Bit12 : 1;
	uint32_t Bit13 : 1;
	uint32_t Bit14 : 1;
	uint32_t Bit15 : 1;
} LCD_Bit_t;

typedef union
{
	LCD_Bit_t LCD_Bit;
	uint16_t Value;
} LCD_Bit_u;

typedef struct
{
	Float_t KinhDo;
	Float_t ViDo;
} Point_t;

typedef struct
{
	uint8_t Buffer[LARGE_BUFFER_SIZE];
	uint16_t BufferIndex;
	uint8_t State;
} LargeBuffer_t;

typedef struct
{
	uint8_t Buffer[SMALL_BUFFER_SIZE];
	uint16_t BufferIndex;
	uint8_t State;
} SmallBuffer_t;

typedef struct _RTC_PERIPHERAL
{
	void (*Init)(void);
	void (*SetDateTime)(DateTime_t Input, int32_t GMT_Adjust);
	DateTime_t (*GetDateTime)(void);
	void (*UpdateTimeFromServer)(uint8_t *Buffer);
	uint32_t (*GetCounter)(void);
	void (*Tick)(void);
	uint8_t (*ValidDateTime)(DateTime_t ThoiGian);
	uint8_t (*GetState)(void);
} const RTC_PERIPHERAL;

typedef struct
{
	void (*Init)(SPI_TypeDef *SPIx);
	uint16_t (*SendByte)(SPI_TypeDef *SPIx, uint8_t Data);

} SPI_Driver_t;

typedef struct
{
	uint8_t DebugMode;
	uint8_t DebugTick;
	uint16_t DebugTimeOut;
	uint8_t InitSystemDone;
	MeasureState_e Al_Step;
	TouchPoint_t TouchPoint;
	uint8_t RequestCheckTP;
	uint8_t DoneSend;			 // Gui xong du lieu
	uint8_t SyncTimeGps;		 // Dong bo thoi gian tu GPS
	uint8_t DoneSync;			 // Dong bo thoi gian tu GPS hoan thanh
	uint8_t TmpTime;			 // Thoi gian tam
	uint8_t TmpParam;			 // Tham so tam
	uint8_t RequestCheckBattery; // Gui yeu cau kiem tra battery
} GlobalStatus_t;

typedef struct
{
	uint8_t BootloaderVersion;
	uint8_t DeviceID[32];
	uint8_t NgayXuatXuong[12];
	uint8_t NgayHieuChinh[12]; // Calib Data
	uint8_t NgayDuPhong[12];   // RESS Date
	uint8_t DonVi[30];		   // Ma don vi cap thiet bi
	uint8_t NguoiThucHien[30]; // Nguoi thuc hien
	uint8_t BlowTime;
	uint8_t BlowPressure;
	uint8_t TimeAutoTurnOff;
	uint8_t TimeoutKeypad;
	uint8_t SetupKey[16];
	Al_CalibTable_t AlMCalibData;
	uint8_t Pad[256];
	uint16_t CRC16;
} Parameter_t;

typedef struct
{
	uint8_t Clk1s_UpdateCFGToFlash;
	uint16_t Clk1s_CountToTurnOff;
	uint8_t Clk1s_JumpToDFU;
} Timeout_t;

typedef struct _INTERNAL_FLASH
{
	void (*Init)(void);
	uint8_t (*Prepare)(uint32_t Sector);
	uint8_t (*WriteBytes)(uint32_t Address, uint8_t *Buffer, uint32_t Length);
	void (*ReadBytes)(uint32_t Address, uint8_t *Buffer, uint32_t Length);
	uint8_t (*CopyData)(uint32_t SrcAddress, uint32_t DstAddress, uint32_t Length);
	uint8_t (*WriteByte)(uint32_t Address, uint8_t Data);
	uint8_t (*WriteInt)(uint32_t Address, uint16_t Data);
	uint8_t (*WriteLong)(uint32_t Address, uint32_t Data);
	uint32_t (*GetSectorIndex)(uint32_t Address);
} const INTERNAL_FLASH_t;

typedef struct
{
	uint8_t (*Init)(void);
	void (*WriteBytes)(uint32_t Address, uint8_t *Buffer, uint16_t Length);
	void (*ReadBytes)(uint32_t Address, uint8_t *Buffer, uint16_t Length);
	void (*EraseSector)(uint16_t SectorCount);
	uint32_t (*GetSectorSize)(void);
	uint32_t (*GetDensity)(void);
	uint8_t (*GetType)(void);
} const Flash_External_t;

typedef enum
{
	GPS_ST_OK = 0,
	GPS_ST_POOR,
	GPS_ST_ERR
} GPS_State_Typedef;

typedef struct _GPS_PERIPHERAL
{
	void (*Init)(void);
	void (*Tick)(void);
	Point_t (*GetPosition)(void);
	uint32_t (*GetBusLongitude)(void);
	uint32_t (*GetBusLatitude)(void);
	uint8_t (*GetSpeed)(void);
	DateTime_t (*GetDateTime)(void);
	uint16_t (*GetCourse)(void);
	uint8_t (*GetSV)(void);
	uint8_t (*IsValid)(void);
	uint8_t (*IsError)(void);
	char *(*GetVersion)(void);
	GPS_State_Typedef (*GetState)(void);
} const GPS_PERIPHERAL;

typedef enum
{
	WD_HEADER = 0,
	WD_MAIN_MENU,
	WD_KHOITAO_CAMBIEN,
	WD_DO_CHU_DONG,
	WD_KQ_DO_CHU_DONG,
	WD_KQ_REPORT,
	WD_DO_THU_DONG,
	WD_INFO_ENTER,
	WD_MTAB_SETUP,
	WD_MTAB_SETUP_BASIC,
	WD_MTAB_SETUP_CALIB,
	WD_MTAB_SETUP_OTHER,
	WD_KEY_NUM,
	WD_KEYPAD,
	WD_RECORDLIST,
	WD_NOTIFI,
	WD_NOTIFISIMPLE,
	WD_DOADC,
	WD_KETQUADOADC,
	WD_SETTING,
	WD_TRUYENDULIEU,
	WD_SAI_MK_TDL,
	WD_SAI_MK,
	WD_XOADULIEU,
	WD_NOTIOK,
	WD_CaiDatChung,
	WD_HC,
	WD_HIEUCHINH,
	WD_HIEUCHINH_BASIC,
	WD_HIEUCHINH_CALIB,
	WD_SAI_MK_HC,
	WD_PINYEU,
	WD_KQDCD,
	WD_CDC,
	WD_MAX_WINDOWS
} WindowMenu_e;

typedef enum
{
	KP_NULL = 0,
	KP_ENTER,
	KP_CANCEL,
} Keypad_Input_e;

typedef enum
{
	KP_REQ_PASS = 0,
	KP_REQ_VALUE,
	KP_REQ_TEXT,
} Keypad_TypeReq_e;

#define KPI_HEADERSIZE 30
#define KPI_CONTENTSIZE 100
typedef struct
{
	uint16_t WD_Req;
	Keypad_TypeReq_e Type_Req;
	uint8_t Header[KPI_HEADERSIZE];
	uint8_t Str[KPI_CONTENTSIZE];
	uint8_t Index;
	uint8_t StrLen;
	uint8_t Status;
	uint8_t CharLen;
	uint16_t Code;
	uint8_t CAP_State;
} KeyInputReq_t;

typedef struct
{
	uint16_t CTP_Sens;
	uint16_t CTP_Sens_DF;
} CTP_t;

typedef struct
{
	uint8_t ExFlash;
} Debug_t;

typedef struct
{

	void (*Delayms)(uint16_t ms);
	void (*WdtRst)(void);
} const Utility_t;

#define RECORD_SIZE 256 //16byte

//0	1	2	3	4	5	6	7	8	9	10	11	12	13	14	15	16	17	18	19	20	21	22	23	24	25	26	27	28	29	30	31	32	33	34	35	36	37	38	39	40	41	42	43	44	45	46	47	48	49	50	51	52	53	54	55	56	57	58	59	60	61	62	63	64	65	66	67	68	69	70	71	72	73	74	75	76	77	78	79	80	81	82	83	84	85	86	87	88	89	90	91	92	93	94	95	96	97	98	99	100	101	102	103	104	105	106	107	108	109	110	111	92	93	94	95	96	97	98	99	100	101	102	103	104	105	106	107	108	109	110	111	112	113	114	115	116	117	118	119	120	121	122	123	124	125	126	127
//u8	u8	u8	u8	u8	u8	u8	u8	float				16																16																20																				X				Y				24																								12												20
//FE	M	YY	MM	DD	hh	mm	ss	K?t Qu?				N	G	U	O	I	 	T	H	U	C		H	I	E	N	N	D	O	N		V	I		C	O	N	G		T	A	C	 	G	I	A	I		P	H	O	N	G																			N	G	U	Y	E	N		H	O	A	N	G		T	H	A	N	H		K	H	A	N	H	3	0	A	-	8	9	.	9	9				N	G	U	O	I 		L	A	M		C	H	U	N	G

typedef union
{
	struct
	{
		uint8_t Hearder;
		uint8_t Mode;
		uint32_t Epoch;
		DateTime_t Time;
		Float_t KetQua;
		Float_t ViDo;
		Float_t KinhDo;
		Int1_t KetQuaADC;
		uint8_t BienSo[12];
		uint8_t NguoiThucHien[30];
		uint8_t DonViCongTac[16]; // Ma Don Vi
		uint8_t DiaDiemKiemTra[40];
		uint8_t TenLaiXe[40];
		uint8_t NguoiLamChung[30];
		int16_t IndexStore;
		int16_t crc16;
	} Name;
	uint8_t Buff[RECORD_SIZE];
} Record_Typedef;

#define RECORD_MAXIMUM_STRORE_INFLASH 30000 // 65536

typedef struct
{
	DateTime_t Time;
	DateTime_t TmpTime;
	GlobalStatus_t GLStatus;
	Parameter_t Parameters;
	Parameter_t TmpParameters;
	GUI_Manager_t GUI_Manager;
	WM_HWIN GUI_Window[WD_MAX_WINDOWS];
	KeyInputReq_t Keypad;
	RTC_PERIPHERAL *Rtc;
	INTERNAL_FLASH_t *InternalFlash;
	GPS_PERIPHERAL *GPS;
	SPI_Driver_t *SPI;
	CTP_t CTP;
	Timeout_t TimeOut;
	Debug_t Debug;
	Flash_External_t *ExFlash;
	Utility_t *Utility;
	uint32_t TickCount1Ms;
	uint32_t TickCount1s;
	Record_Typedef Record;
} System_t;

void ResetWatchdog(void);
void SystemReset(uint8_t NguyenNhanReset);
void DelayMS(uint16_t ms);
void Beep(uint16_t Length);
void Beeps(uint8_t Count);

extern System_t xSystem;
extern uint32_t RecordCount;
void BeepKeypad(void);

#endif // __DATA_DEFINE_H__
