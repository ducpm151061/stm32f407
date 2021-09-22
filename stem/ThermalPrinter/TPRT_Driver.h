/*
******************************************************************************
  * @file    	: TPRT_Driver.h
  * @version 	: V1.0.0
  * @date    	: 22/02/2014
  * @brief   	: Khai bao cac macro va cac ham dung trong module giao tiep voi HeadPrinter
******************************************************************************
*/

#ifndef __TPRT_DRIVER_H__
#define __TPRT_DRIVER_H__

/* Includes */
#include "stm32f4xx.h"

/*Power control*/
#define TPRT_POWER_GPIO GPIOC
#define TPRT_POWER_GPIO_CLK RCC_AHB1Periph_GPIOC
#define TPRT_POWER_PIN GPIO_Pin_6

/* Khai bao SPI */
#define TPRT_SPI SPI2
#define TPRT_SPI_CLK RCC_APB1Periph_SPI2

/* Khai bao phan cung giao tiep voi module */
#define TPRT_SPI_GPIO GPIOB
#define TPRT_SPI_GPIO_CLK RCC_AHB1Periph_GPIOB

#define TPRT_CLK_PIN GPIO_Pin_13
#define TPRT_CLK_PINSOURCE GPIO_PinSource13
#define TPRT_MISO_PIN GPIO_Pin_14
#define TPRT_MISO_PINSOURCE GPIO_PinSource14
#define TPRT_MOSI_PIN GPIO_Pin_15
#define TPRT_MOSI_PINSOURCE GPIO_PinSource15

#define TPRT_CS_PIN GPIO_Pin_8
#define TPRT_CS_GPIO GPIOD
#define TPRT_CS_GPIO_CLK RCC_AHB1Periph_GPIOD

#define TPRT_STB_GPIO GPIOD
#define TPRT_STB_GPIO_CLK RCC_AHB1Periph_GPIOD

#define TPRT_STB1_PIN GPIO_Pin_9
#define TPRT_STB23_PIN GPIO_Pin_13
#define TPRT_STB4_PIN GPIO_Pin_12
#define TPRT_STB56_PIN GPIO_Pin_11

#define TPRT_PAPER_DETECT_GPIO GPIOD
#define TPRT_PAPER_DETECT_GPIO_CLK RCC_AHB1Periph_GPIOD
#define TPRT_PAPER_DETECT_PIN GPIO_Pin_10

#define TPRT_Enable() GPIO_ResetBits(TPRT_CS_GPIO, TPRT_CS_PIN)
#define TPRT_Disable() GPIO_SetBits(TPRT_CS_GPIO, TPRT_CS_PIN)

#define Strobe1_Enable() GPIO_SetBits(TPRT_STB_GPIO, TPRT_STB1_PIN)
#define Strobe1_Disable() GPIO_ResetBits(TPRT_STB_GPIO, TPRT_STB1_PIN)

#define Strobe2_Enable() GPIO_SetBits(TPRT_STB_GPIO, TPRT_STB23_PIN)
#define Strobe2_Disable() GPIO_ResetBits(TPRT_STB_GPIO, TPRT_STB23_PIN)

#define Strobe3_Enable() GPIO_SetBits(TPRT_STB_GPIO, TPRT_STB23_PIN)
#define Strobe3_Disable() GPIO_ResetBits(TPRT_STB_GPIO, TPRT_STB23_PIN)

#define Strobe4_Enable() GPIO_SetBits(TPRT_STB_GPIO, TPRT_STB4_PIN)
#define Strobe4_Disable() GPIO_ResetBits(TPRT_STB_GPIO, TPRT_STB4_PIN)

#define Strobe5_Enable() GPIO_SetBits(TPRT_STB_GPIO, TPRT_STB56_PIN)
#define Strobe5_Disable() GPIO_ResetBits(TPRT_STB_GPIO, TPRT_STB56_PIN)

#define Strobe6_Enable() GPIO_SetBits(TPRT_STB_GPIO, TPRT_STB56_PIN)
#define Strobe6_Disable() GPIO_ResetBits(TPRT_STB_GPIO, TPRT_STB56_PIN)

#define FONT_COLUMN 22

/* KHAI BAO CAC HAM DUNG TRONG MODULE */
void TPRT_Initialize(void);
void TPRT_WriteLine(uint8_t *Buffer);
void TPRT_TestMotor(void);
void TPRT_InitHardware(void);
void TPRT_InitSoftware(void);
void TPRT_SPI_TransferByte(uint8_t Data);
void TPRT_DisableAllStrobe(void);
void TPRT_EnableAllStrobe(void);
void TPRT_SPI_I2S_SendByte(uint8_t Data);
uint8_t TPRT_PaPerDetect(void);
void TPRT_Disable3Strobe1(void);
void TPRT_Enable3Strobe1(void);
void TPRT_Enable3Strobe2(void);
void TPRT_Disable3Strobe2(void);
void TPRT_Enable2Strobe1(void);
void TPRT_Enable2Strobe2(void);
void TPRT_Enable2Strobe3(void);
void TPRT_Disable2Strobe1(void);
void TPRT_Disable2Strobe2(void);
void TPRT_Disable2Strobe3(void);
void TPRT_PowerControlInit(void);
void Printer_ReceiveData(uint8_t Data);
void TPRT_PrintString(char *Buffer, uint8_t Font_Size, uint8_t HeatingTime);
void TPRT_PrintText_Tick(void);

void TPRT_POW_ON(void);
void TPRT_POW_OFF(void);

#endif /*__TPRT_H__*/

/********************************* END OF FILE *******************************/
