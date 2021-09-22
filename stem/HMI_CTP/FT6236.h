#ifndef __FT6236_H
#define __FT6236_H
//===========================================================================================================
//===========================================================================================================
#include "stm32f4xx.h"
//===========================================================================================================
//#define FT6236_DEBUG		1
//===========================================================================================================
#define FT6236_FALSE 0
#define FT6236_TRUE 1
//===========================================================================================================
//Defines I2C Communicate With FT6236
#define I2C_COMMUNICATE_FT6236 I2C1
#define I2C_COMMUNICATE_FT6236_RCC RCC_APB2Periph_SPI1
#define FT6236_ADDRESS 0x70
//===========================================================================================================
//Defines FT6236's Connection Pins
#define FT6236_RST GPIO_Pin_3
#define FT6236_RST_PORT GPIOD
#define FT6236_RST_PORT_RCC RCC_AHB1Periph_GPIOD

#define FT6236_IRQ GPIO_Pin_4
#define FT6236_IRQ_PORT GPIOD
#define FT6236_IRQ_PORT_RCC RCC_AHB1Periph_GPIOD

#define FT6236_SCL GPIO_Pin_8
#define FT6236_SCL_PORT GPIOB
#define FT6236_SCL_PORT_RCC RCC_AHB1Periph_GPIOB

#define FT6236_SDA GPIO_Pin_9
#define FT6236_SDA_PORT GPIOB
#define FT6236_SDA_PORT_RCC RCC_AHB1Periph_GPIOB
//===========================================================================================================
//Defines FT6236's Working Mode Register Map
#define FT6236_DEV_MODE 0x00
#define FT6236_GEST_ID 0x01
#define FT6236_TD_STATUS 0x02
#define FT6236_P1_XH 0x03
#define FT6236_P1_XL 0x04
#define FT6236_P1_YH 0x05
#define FT6236_P1_YL 0x06
#define FT6236_P1_WEIGHT 0x07
#define FT6236_P1_MISC 0x08
#define FT6236_P2_XH 0x09
#define FT6236_P2_XL 0x0A
#define FT6236_P2_YH 0x0B
#define FT6236_P2_YL 0x0C
#define FT6236_P2_WEIGHT 0x0D
#define FT6236_P2_MISC 0x0E
#define FT6236_TH_GROUP 0x80
#define FT6236_TH_DIFF 0x85
#define FT6236_CTRL 0x86
#define FT6236_TIMEENTERMONITOR 0x87
#define FT6236_PERIODACTIVE 0x88
#define FT6236_PERIODMONITOR 0x89
#define FT6236_RADIAN_VALUE 0x91
#define FT6236_OFFSET_LEFT_RIGHT 0x92
#define FT6236_UP_DOWN 0x93
#define FT6236_DISTANCE_LEFT_RIGHT 0x94
#define FT6236_DISTANCE_UP_DOWN 0x95
#define FT6236_DISTANCE_ZOOM 0x96
#define FT6236_LIB_VER_H 0xA1
#define FT6236_LIB_VER_L 0xA2
#define FT6236_CIPHER 0xA3
#define FT6236_G_MODE 0xA4
#define FT6236_PWR_MODE 0xA5
#define FT6236_FIRMID 0xA6
#define FT6236_FOCALTECH_ID 0xA8
#define FT6236_RELEASE_CODE_ID 0xAF
#define FT6236_STATE 0xBC
//===========================================================================================================
#define FT6236_EVENT_PRESS_DOWN 0x00
#define FT6236_EVENT_LIFT_UP 0x01
#define FT6236_EVENT_CONTACT 0x02
#define FT6236_EVENT_NO_EVENT 0x03
//===========================================================================================================
void FT6236_Pre_Init(void);
void FT6236_Init(void);
void FT6236_Read_Touch_Position(int16_t *Touch_X_Position, int16_t *Touch_Y_Position);
//===========================================================================================================
//===========================================================================================================
#endif /* __FT6236_H */
