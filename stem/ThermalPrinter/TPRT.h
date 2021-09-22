#ifndef __TPRT_H
#define __TPRT_H

#include "Fonts.h"
#include "string.h"
#include "stdio.h"
#include "Motor_Controler.h"
#include "DataDefine.h"
#include "TPRT_Driver.h"

/* Khai bao mang du lieu gioi han in */
#define PRINTER_BUFFER 256
#define PRINTER_TXMAX 100
#define PRINTER_NBOF_LINE 150	   // Maximum Line of string text
#define PRINTER_NBOF_CHARINLINE 32 // Maximum Char in a Line text
#define PRINTER_UART USART1

typedef enum
{
	MODE_FREE = 0,
	MODE_TEXT,
	MODE_IMAGE,
	MODE_BARCODE,
} PRT_MODE;

typedef struct
{
	uint8_t Text[PRINTER_NBOF_CHARINLINE];
	uint8_t Font_Size;
} DuLieuIn_t;

/* Khai bao cau truc du lieu de in */
typedef struct
{
	//    uint8_t			Buffer[PRINTER_BUFFER];
	uint8_t Type;
	uint16_t RxCounter;
	uint8_t RxBuffer[PRINTER_BUFFER];
	uint16_t TimeOutReceive;
	uint8_t IsPrinting;
	//	uint8_t 		TxBuffer[PRINTER_TXMAX];
	uint8_t Mode;
	uint8_t Flag_ReqNewLine;
	uint8_t ThuTu;
	uint8_t StepDot;
	uint8_t Fonts_Size_Old;
	uint8_t SoKyTu_MotDongIn;
	uint8_t Action;
	uint8_t HeatStart;
	uint8_t NbOfStrobe;
	uint8_t HeatingTimeMax;
	uint8_t HeatingTimeClk;
	uint8_t SoDongDaLuu;
	uint8_t BamInLai;
	uint16_t Length;
	uint8_t LinePrinted;
	uint8_t LineNeedPrint;
	uint8_t ThoiGianQuayDongCo;
	uint8_t TimeOut;
} Printer_t;

extern Printer_t Printer;
extern DuLieuIn_t DuLieuIn[PRINTER_NBOF_LINE];

void TPRT_Initialize(void);
void TPRT_AlM_StartPrint(void);
void TPRT_AlM_StopPrint(void);
void TPRT_AlM_AddLine(uint8_t *Data);
void TPRT_AlM_ClearData(void);
void TPRT_Timer_Disable(void);
void TPRT_Timer_Enable(void);

int8_t GetTPRTTemp(void);

#endif
