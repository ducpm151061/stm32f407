#ifndef __BLOWDETECT_H__
#define __BLOWDETECT_H__

#include <stdint.h>

uint16_t BlowDetect_GetOneSample(void);
uint8_t BlowDetect_BlowDetected(void);
uint8_t BlowDetect_GetCount(void);
void BlowDetect_Clear(void);

#endif
