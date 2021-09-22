#ifndef __ALCOHOL_METTER_H__
#define __ALCOHOL_METTER_H__

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#define ALCOHOL_RAWSAMPLE_WINDOW 4000

#define TREND_CONFIRM 50

#define UNKNOWN_TREND 0
#define UP_TREND 1
#define DOWN_TREND 2

typedef enum
{
    ALM_IDLE = 0,
    ALM_DOCHUDONG,
    ALM_DOTHUDONG,
    ALM_LISTRECORD,
    ALM_SETUP,
    ALM_DOADC,
    ALM_DO,
    ALM_XA
} ALM_Task;

typedef struct
{
    ALM_Task Task;
    uint8_t Stage;
    uint8_t Step;
} AlcoholManage_t;

//typedef struct{
//  uint8_t PrtID[30];
//  uint8_t PrtTime[30];
//  uint8_t Mode[30];
//  uint8_t PoliName[30];
//  uint8_t PoliID[30];
//  uint8_t DrvName[30];
//  uint8_t DrvLicense[30];
//  uint8_t VehiclePlate[30];
//  uint8_t VerifyPerson[30];
//  uint8_t Address[30];
//}Al_PrintInfo_t;

#define AlM_Calib_MaxIndex 20
typedef struct
{
    uint16_t ValueADC;
    uint16_t ValueMgL;
} Al_ValueInTable_t;

typedef union
{
    struct
    {
        uint16_t Al_MaxValue;
        Al_ValueInTable_t Data[AlM_Calib_MaxIndex];
    } s;
    uint8_t b[AlM_Calib_MaxIndex * 4 + 2];
} Al_CalibTable_t;

void AlcoholMetter_Init(void);
void AlcoholMetter_Tick(void);
void AlM_ChangeStage(uint8_t State);

// Record
void StoreIndexRecord(void);
void FullClear_Record(void);
void Record_Read(uint16_t Index);
void ReadIndexRecord(void);
int32_t GetIndexStore(void);

#endif
