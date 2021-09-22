#ifndef __RESISTIVETOUCH_H__
#define __RESISTIVETOUCH_H__

#include "DataDefine.h"
#include "Hardware.h"

void RTP_GetTouchPoint_Tick(void);
void RTP_Init(void);
uint16_t RTP_Get_X(void);
uint16_t RTP_Get_Y(void);
#endif
