/*
******************************************************************************
  * @file    	: main.h
  * @version 	: V1.0.0
  * @date    	: 04/12/2014
  * @brief   	: Khai bao cac macro va cac ham dung trong main
******************************************************************************
*/

#include "stm32f4xx.h"

#ifndef _MOTOR_CONTROLER_H
#define _MOTOR_CONTROLER_H

#define IN11 GPIO_Pin_4
#define IN11_GPIO GPIOC

#define IN12 GPIO_Pin_2
#define IN12_GPIO GPIOA

#define IN21 GPIO_Pin_7 //0
#define IN21_GPIO GPIOA

#define IN22 GPIO_Pin_14
#define IN22_GPIO GPIOD

#define MOTOR_EN_PIN GPIO_Pin_15
#define MOTOR_EN_GPIO GPIOD

//#define	SMT_C1_OFF	IN21_GPIO->BSRRL = IN21; IN12_GPIO->BSRRH = IN12;
//#define	SMT_C1_ON	IN21_GPIO->BSRRH = IN21; IN12_GPIO->BSRRL = IN12;
//#define	SMT_C2_OFF	IN11_GPIO->BSRRL = IN11; IN22_GPIO->BSRRH = IN22;
//#define	SMT_C2_ON	IN11_GPIO->BSRRH = IN11; IN22_GPIO->BSRRL = IN22;

#define SMT_C2_ON          \
  IN11_GPIO->BSRRH = IN11; \
  IN12_GPIO->BSRRL = IN12;
#define SMT_C1_ON          \
  IN21_GPIO->BSRRH = IN21; \
  IN22_GPIO->BSRRL = IN22;
#define SMT_C2_OFF         \
  IN11_GPIO->BSRRL = IN11; \
  IN12_GPIO->BSRRL = IN12;
#define SMT_C1_OFF         \
  IN21_GPIO->BSRRL = IN21; \
  IN22_GPIO->BSRRL = IN22;

#define SMT_ENABLE MOTOR_EN_GPIO->BSRRH = MOTOR_EN_PIN;
#define SMT_DISABLE MOTOR_EN_GPIO->BSRRL = MOTOR_EN_PIN;

typedef struct
{
  uint8_t NbOfStep;
  uint8_t TimeStepMotor;
  uint8_t NbOfStep_OneDotLine; //quay dong co 2 buoc de in 1 dotline
  uint8_t ForwardTime;
  uint8_t StopTime;
} Motor_t;

typedef struct
{
  uint8_t TimeStepMotor;
  uint8_t NbOfStep;
  uint16_t TestTime;
} MotorTest_t;

extern Motor_t Motor;
extern MotorTest_t MotorTest;

/* Khai bao cac ham dung trong module */
void Motor_Init(void);
void Motor_InitTest(void);
uint8_t Motor_ForwardTest(uint8_t Step);
void Motor_Forward_Step(uint16_t NbOfStep);
void Motor_Stop(void);
void Motor_Tick(void);
void TPRT_Motor_Tick(void);
uint8_t TPRT_Motor_CheckIsNewLine(void);

#endif /* _MOTOR_CONTROLER_H */

/********************************* END OF FILE *******************************/
