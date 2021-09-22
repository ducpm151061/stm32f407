/*
*****************************************************************************
  * @file    	: Motor_Controler.c
  * @author  	: Dao Thien Bao
  * @version 	: V1.0.0
  * @date    	: 26/12/2014
  * @brief   	: 
*****************************************************************************
*/

/* Includes */
#include "Motor_Controler.h"
#include "TPRT.h"

#define MT_Step_Time 3

Motor_t Motor;
MotorTest_t MotorTest;

/*
******************************************************************************
 * Function Name  	: Motor_Init
 * Return         	: None
 * Parameters 		: None
 * Description		: 
******************************************************************************
*/
void Motor_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	/* GPIOA, MotorPort and GPIOE Clocks enable */

	/* MotorPort Configuration*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;

	GPIO_InitStructure.GPIO_Pin = IN11;
	GPIO_Init(IN11_GPIO, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = IN12;
	GPIO_Init(IN12_GPIO, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = IN21;
	GPIO_Init(IN21_GPIO, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = IN22;
	GPIO_Init(IN22_GPIO, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = MOTOR_EN_PIN;
	GPIO_Init(MOTOR_EN_GPIO, &GPIO_InitStructure);

	Motor.ForwardTime = MT_Step_Time;
	Motor.StopTime = MT_Step_Time;

	//	Motor_InitTest();

	Motor_Stop();
}

/*
******************************************************************************
 * Function Name  	: Motor_Forward_Step
 * Return         	: None
 * Parameters 		: None
 * Description		: 
******************************************************************************
*/
void Motor_Forward_Step(uint16_t NbOfStep)
{
	switch (NbOfStep)
	{
	case 1:
	case 5:
	case 9:
	case 13:
		SMT_C1_ON;
		Motor.TimeStepMotor = MT_Step_Time;
		break;

	case 2:
	case 6:
	case 10:
	case 14:
		SMT_C2_ON;
		Motor.TimeStepMotor = MT_Step_Time;
		break;

	case 3:
	case 7:
	case 11:
	case 15:
		SMT_C1_OFF;
		Motor.TimeStepMotor = MT_Step_Time;
		break;
	case 4:
	case 8:
	case 12:
	case 16:
		SMT_C2_OFF;
		Motor.TimeStepMotor = MT_Step_Time;
		break;

	default:
		SMT_C1_OFF;
		SMT_C2_OFF;
		break;
	}
}

/*******************************************************************************
 * Function Name  	: Motor_Stop
 * Return         	: None
 * Parameters 		: None
 * Description		: 
*******************************************************************************/
void Motor_Stop(void)
{
	GPIO_SetBits(IN11_GPIO, IN11);
	GPIO_ResetBits(IN12_GPIO, IN12);
	GPIO_ResetBits(IN21_GPIO, IN21);
	GPIO_ResetBits(IN22_GPIO, IN22);
	SMT_DISABLE;
}

void TPRT_Motor_Tick(void)
{
	if ((Motor.NbOfStep_OneDotLine > 0) && (Motor.TimeStepMotor == 0))
	{
		Motor_Forward_Step(Motor.NbOfStep);
		Motor.NbOfStep_OneDotLine--;
		if (++Motor.NbOfStep > 16)
			Motor.NbOfStep = 1;
	}
}

uint8_t TPRT_Motor_CheckIsNewLine(void)
{
	return (Motor.NbOfStep_OneDotLine == 0);
}

/********************************* END OF FILE *******************************/
