/*
******************************************************************************
  * @file    	: Printer.h
  * @version 	: V1.0.0
  * @date    	: 10/01/2014
  * @brief   	: Khai bao cac macro va cac ham dung de giao tiep voi may in
******************************************************************************
*/

#ifndef __FONTS_H__
#define __FONTS_H__

/* Includes */
#include "stm32f4xx.h"

#define FONT_NORMAL 0
#define FONT_NORMAL_COLUMN 30 //*8 = 240
#define FONT_BIG 1
#define FONT_BIG_COLUMN 50

#define FONTS_NOMAL_START 65536
#define FONTS_BIG_START 77000
typedef struct Fonts
{
  uint16_t Start; //Vi tri bat dau cua chu cai can in
} Fonts_t;

extern Fonts_t Fonts;
extern Fonts_t Fonts_Info[];
extern Fonts_t Fonts_Info_Big[];
extern const uint8_t Fonts_Arial[];
extern const uint8_t Fonts_Arial_Big[];

#endif
