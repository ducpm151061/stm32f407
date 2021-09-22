#ifndef __INIT_SYSTEM_H__
#define __INIT_SYSTEM_H__

void InitSystem(void);
void InitUSB(void);
void DeInitUSB(void);
void InitFileSystem(void);
void InitTimerGPSUpdate(void);
void InitTimerTransferData(void);
void InitTimerBatteryUpdate(void);

#endif // __INIT_SYSTEM_H__
