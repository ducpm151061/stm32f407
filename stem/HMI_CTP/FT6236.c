#include <stdio.h>
#include <math.h>
//#include "i2c.h"
//#include "delay.h"
#include "FT6236.h"
#include "DataDefine.h"
#include "GUI.h"
//#include "Communication_Function.h"
//===========================================================================================================
//===========================================================================================================
uint32_t FT6236_X_Valid[50];
uint32_t FT6236_Y_Valid[50];

int32_t DeviationOf_FT6236_X_Valid[50];
int32_t DeviationOf_FT6236_Y_Valid[50];

float Z_Score_FT6236_X_Valid[50];
float Z_Score_FT6236_Y_Valid[50];
//===========================================================================================================
//===========================================================================================================

void Delay_ms(uint8_t nTime)
{
	uint32_t TimingDelay = nTime * 1000;
	while (--TimingDelay != 0)
		;
}

void I2Cx_WriteOneRegister(I2C_TypeDef *I2Cx, uint8_t Slave_Address, uint8_t Register_Address, uint8_t Data)
{
	//========================================================================================================
	//Checks: I2C is not busy
	while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY))
		;
	//========================================================================================================
	//Generates START Condition
	I2C_GenerateSTART(I2Cx, ENABLE);

	//Checks: Slave Has Acknowledged START Condition
	while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
		;
	//========================================================================================================
	//Sends Slave's Address, Transmitter
	I2C_Send7bitAddress(I2Cx, Slave_Address, I2C_Direction_Transmitter);

	//Checks: Slave Has Acknowledged Address, Transmitter
	while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
		;
	//========================================================================================================
	//Sends Register Address
	I2C_SendData(I2Cx, Register_Address);

	//Checks: One Byte Has Been Transmitted
	while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		;
	//========================================================================================================
	//Writes Data to Register
	I2C_SendData(I2Cx, Data);

	//Checks: One Byte Has Been Transmitted
	while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		;
	//========================================================================================================
	//Generates STOP Condition
	I2C_GenerateSTOP(I2Cx, ENABLE);
	//========================================================================================================
}

//===========================================================================================================
//===========================================================================================================
/**
	* @brief  	Initializes I2C
	* @param  	+ I2C's Name
					+ I2C's Mode
					+ I2C's DutyCycle
					+ I2C's OwnAddress1
					+ I2C's Ack
					+ I2C's ClockSpeed
					+ I2C's AcknowledgedAddress
	* @retval 	None
	*/
void I2Cx_Init(I2C_TypeDef *I2Cx, uint16_t Mode, uint16_t DutyCycle, uint16_t OwnAddress1, uint16_t Ack, uint32_t ClockSpeed, uint16_t AcknowledgedAddress)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	I2C_InitTypeDef I2C_InitStructure;
	//========================================================================================================
	//========================================================================================================
	//========================================================================================================
	if (I2Cx == I2C1)
	{
		//=====================================================================================================
		//=====================================================================================================
		//Enables I2C1's SDA Pin Clock
		RCC_AHB1PeriphClockCmd(FT6236_SDA_PORT_RCC, ENABLE);

		//Configures I2C1's SDA Pin is Alternate Function, 100MHz, Open Drain, No Pull
		GPIO_InitStructure.GPIO_Pin = FT6236_SDA;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(FT6236_SDA_PORT, &GPIO_InitStructure);
		//=====================================================================================================
		//=====================================================================================================
		//Enables I2C1's SCL Pin Clock
		RCC_AHB1PeriphClockCmd(FT6236_SCL_PORT_RCC, ENABLE);

		//Configures I2C1's SCL Pin is Alternate Function, 100MHz, Open Drain, No Pull
		GPIO_InitStructure.GPIO_Pin = FT6236_SCL;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(FT6236_SCL_PORT, &GPIO_InitStructure);
		//=====================================================================================================
		//=====================================================================================================
		//Connects SCL, SDA Pin to  I2C1
		GPIO_PinAFConfig(FT6236_SCL_PORT, GPIO_PinSource8, GPIO_AF_I2C1);
		GPIO_PinAFConfig(FT6236_SDA_PORT, GPIO_PinSource9, GPIO_AF_I2C1);
		//=====================================================================================================
		//=====================================================================================================
		//Enables I2C1 Clock
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

		//Resets I2C1
		I2C_DeInit(I2C1);

		//Configures I2C1's Mode, DutyCycle, OwnAddress1, Ack, ClockSpeed, AcknowledgedAddress
		I2C_InitStructure.I2C_Mode = Mode;
		I2C_InitStructure.I2C_DutyCycle = DutyCycle;
		I2C_InitStructure.I2C_OwnAddress1 = OwnAddress1;
		I2C_InitStructure.I2C_Ack = Ack;
		I2C_InitStructure.I2C_ClockSpeed = ClockSpeed;
		I2C_InitStructure.I2C_AcknowledgedAddress = AcknowledgedAddress;
		I2C_Init(I2C1, &I2C_InitStructure);

		//Enables I2C1
		I2C_Cmd(I2C1, ENABLE);
		//=====================================================================================================
		//=====================================================================================================
	}

	//========================================================================================================
	//========================================================================================================
	//========================================================================================================
}
//===========================================================================================================//===========================================================================================================
/**
	* @brief  	Initializes FT6236
	* @param  	None
	* @retval 	None
	*/
void FT6236_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//========================================================================================================
	//========================================================================================================
	//========================================================================================================
	//Initializes I2C Communicate With FT6236
	I2Cx_Init(I2C_COMMUNICATE_FT6236, I2C_Mode_I2C, I2C_DutyCycle_2, 0x01, I2C_Ack_Disable, 100000, I2C_AcknowledgedAddress_7bit);
	//========================================================================================================
	//========================================================================================================
	//========================================================================================================
	//Enables FT6236's IRQ Pin Clock
	RCC_AHB1PeriphClockCmd(FT6236_IRQ_PORT_RCC, ENABLE);

	//Configures FT6236's IRQ Pin is GPIO, Input Mode, Push-Pull, 100Mhz, Pull Up
	GPIO_InitStructure.GPIO_Pin = FT6236_IRQ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(FT6236_IRQ_PORT, &GPIO_InitStructure);
	//========================================================================================================
	//========================================================================================================
	//========================================================================================================
	//Enables FT6236's RST Pin Clock
	RCC_AHB1PeriphClockCmd(FT6236_RST_PORT_RCC, ENABLE);

	//Configures FT6236's RST Pin is GPIO, Output Mode, Push-Pull, 100Mhz, Pull Up
	GPIO_InitStructure.GPIO_Pin = FT6236_RST;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(FT6236_RST_PORT, &GPIO_InitStructure);

	//Resets FT6236
	GPIO_ResetBits(FT6236_RST_PORT, FT6236_RST);
	Delay_ms(10);

	GPIO_SetBits(FT6236_RST_PORT, FT6236_RST);
	Delay_ms(255);
	//========================================================================================================
	//========================================================================================================
	//Configures FT6236
	I2Cx_WriteOneRegister(I2C_COMMUNICATE_FT6236, FT6236_ADDRESS, FT6236_CTRL, 0x00);
	I2Cx_WriteOneRegister(I2C_COMMUNICATE_FT6236, FT6236_ADDRESS, FT6236_G_MODE, 0x00);

	I2Cx_WriteOneRegister(I2C_COMMUNICATE_FT6236, FT6236_ADDRESS, FT6236_TH_GROUP, 0x10);
	//========================================================================================================
	//========================================================================================================
	//========================================================================================================
}
//===========================================================================================================
//===========================================================================================================
/**
	* @brief  	Reads Data from One Register of Slave
	* @param  	+ I2C's Name
					+ Slave's Address
					+ Address of Source Register				
	* @retval 	+ Register's Data
	*/
uint8_t I2Cx_ReadOneRegister(I2C_TypeDef *I2Cx, uint8_t Slave_Address, uint8_t Register_Address)
{
	//========================================================================================================
	//Checks: I2C is not busy
	while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY))
		;
	//========================================================================================================
	//Generates START Condition
	I2C_GenerateSTART(I2Cx, ENABLE);

	//Checks: Slave Has Acknowledged START Condition
	while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
		;
	//========================================================================================================
	//Sends Slave's Address, Transmitter
	I2C_Send7bitAddress(I2Cx, Slave_Address, I2C_Direction_Transmitter);

	//Checks: Slave Has Acknowledged Address, Transmitter
	while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
		;
	//========================================================================================================
	//Checks: Buffer is Empty
	while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
		;

	//Sends Sub Address
	I2C_SendData(I2Cx, Register_Address);
	//========================================================================================================
	//Checks: One Byte Has Been Transmitted
	while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		;
	//========================================================================================================
	//Generates RESTART Condition
	I2C_GenerateSTART(I2Cx, ENABLE);

	//Checks: Slave Has Acknowledged RESTART Condition
	while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
		;
	//========================================================================================================
	//Sends Slave's Address, Receiver
	I2C_Send7bitAddress(I2Cx, Slave_Address, I2C_Direction_Receiver);

	//Checks: Slave Has Acknowledged Address, Receiver
	while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
		;
	//========================================================================================================
	//Disables Acknowledge of Received Data
	I2C_AcknowledgeConfig(I2Cx, DISABLE);
	//========================================================================================================
	//Generates STOP Condition
	I2C_GenerateSTOP(I2Cx, ENABLE);
	//========================================================================================================
	//Checks: One Byte Has Been Received
	while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED))
		;

	//Returns Received Data
	return (I2C_ReceiveData(I2Cx));
	//========================================================================================================
}
//===========================================================================================================
//===========================================================================================================
/**
	* @brief  	Pre-Initializes FT6236
	* @param  	None
	* @retval 	None
	*/
void FT6236_Pre_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//========================================================================================================
	//========================================================================================================
	//========================================================================================================
	//Enables FT6236's SCL Pin Clock
	RCC_AHB1PeriphClockCmd(FT6236_SCL_PORT_RCC, ENABLE);

	//Configures FT6236's SCL Pin is GPIO, Output Mode, Push-Pull, 100Mhz, Pull Down
	GPIO_InitStructure.GPIO_Pin = FT6236_SCL;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(FT6236_SCL_PORT, &GPIO_InitStructure);

	GPIO_ResetBits(FT6236_SCL_PORT, FT6236_SCL);
	//========================================================================================================
	//========================================================================================================
	//========================================================================================================
	//Enables FT6236's SDA Pin Clock
	RCC_AHB1PeriphClockCmd(FT6236_SDA_PORT_RCC, ENABLE);

	//Configures FT6236's SDA Pin is GPIO, Output Mode, Push-Pull, 100Mhz, Pull Down
	GPIO_InitStructure.GPIO_Pin = FT6236_SDA;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(FT6236_SDA_PORT, &GPIO_InitStructure);

	GPIO_ResetBits(FT6236_SDA_PORT, FT6236_SDA);
	//========================================================================================================
	//========================================================================================================
	//========================================================================================================
	//Enables FT6236's IRQ Pin Clock
	RCC_AHB1PeriphClockCmd(FT6236_IRQ_PORT_RCC, ENABLE);

	//Configures FT6236's IRQ Pin is GPIO, Output Mode, Push-Pull, 100Mhz, Pull Down
	GPIO_InitStructure.GPIO_Pin = FT6236_IRQ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(FT6236_IRQ_PORT, &GPIO_InitStructure);

	GPIO_ResetBits(FT6236_IRQ_PORT, FT6236_IRQ);
	//========================================================================================================
	//========================================================================================================
	//========================================================================================================
	//Enables FT6236's RST Pin Clock
	RCC_AHB1PeriphClockCmd(FT6236_RST_PORT_RCC, ENABLE);

	//Configures FT6236's RST Pin is GPIO, Output Mode, Push-Pull, 100Mhz, Pull Down
	GPIO_InitStructure.GPIO_Pin = FT6236_RST;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(FT6236_RST_PORT, &GPIO_InitStructure);

	GPIO_ResetBits(FT6236_RST_PORT, FT6236_RST);
	//========================================================================================================
	//========================================================================================================
	//========================================================================================================
}
//===========================================================================================================
//===========================================================================================================
//===========================================================================================================
/**
	* @brief  	Read Position of Touch
	* @param  	+ X-Axis Touch Position
					+ Y-Axis Touch Position
	* @retval 	None
	*/
void FT6236_Read_Touch_Position(int16_t *Touch_X_Position, int16_t *Touch_Y_Position)
{
	//========================================================================================================
	//========================================================================================================
	uint8_t FT6236_XH;
	uint8_t FT6236_XL;
	uint8_t FT6236_YH;
	uint8_t FT6236_YL;

	uint32_t FT6236_X;
	uint32_t FT6236_Y;

	uint32_t TempCounter = 0;
	static uint32_t ValidCounter = 0;
	//========================================================================================================
	uint32_t SumOf_FT6236_X_Valid = 0;
	uint32_t SumOf_FT6236_Y_Valid = 0;

	uint32_t MeanOf_FT6236_X_Valid = 0;
	uint32_t MeanOf_FT6236_Y_Valid = 0;

	float VarianceOf_FT6236_X_Valid = 0.0;
	float VarianceOf_FT6236_Y_Valid = 0.0;

	float StdOf_FT6236_X_Valid = 0.0;
	float StdOf_FT6236_Y_Valid = 0.0;

	uint32_t UsualCounter_FT6236_X_Valid = 0;
	uint32_t UsualCounter_FT6236_Y_Valid = 0;

	uint32_t MeanOf_FT6236_X_Usual = 0;
	uint32_t MeanOf_FT6236_Y_Usual = 0;

#ifdef FT6236_DEBUG
	char FT6236_TempString[25];
#endif
	//========================================================================================================
	//========================================================================================================
	//Reads Data
	for (TempCounter = 0; TempCounter < 15; TempCounter++)
	{
		FT6236_X = FT6236_Y = 0;
		if (GPIO_ReadInputDataBit(FT6236_IRQ_PORT, FT6236_IRQ) == 0)
		{
			//DebugPrint(" IRQ ");
			FT6236_XH = I2Cx_ReadOneRegister(I2C_COMMUNICATE_FT6236, FT6236_ADDRESS, FT6236_P1_XH);
			FT6236_XL = I2Cx_ReadOneRegister(I2C_COMMUNICATE_FT6236, FT6236_ADDRESS, FT6236_P1_XL);
			FT6236_YH = I2Cx_ReadOneRegister(I2C_COMMUNICATE_FT6236, FT6236_ADDRESS, FT6236_P1_YH);
			FT6236_YL = I2Cx_ReadOneRegister(I2C_COMMUNICATE_FT6236, FT6236_ADDRESS, FT6236_P1_YL);

			if ((FT6236_XH & 0xC0) == 0x80) //Checks Event Flag is Contact
			{
				//Calculates Contact Position
				FT6236_X = ((FT6236_XH & 0x0F) << 8) + FT6236_XL;
				FT6236_Y = ((FT6236_YH & 0x0F) << 8) + FT6236_YL;

				//Checks Valid Contact Position
				if ((FT6236_X > 0) && (FT6236_X < 320))
				{
					//DebugPrint(" X ");
					if ((FT6236_Y > 0) && (FT6236_Y < 480))
					{
						//	DebugPrint(" Y ");
						FT6236_X_Valid[ValidCounter] = FT6236_X;
						FT6236_Y_Valid[ValidCounter] = FT6236_Y;
						ValidCounter++;
						//DebugPrint("[%d]",ValidCounter);
					}
				}
			}
			Delay_ms(100);
		}
		//DebugPrint("\rRTP[%d-%d]:%d-%d.", ValidCounter,TempCounter,FT6236_X,FT6236_Y);
	}

	if (ValidCounter >= 3)
	{
//=====================================================================================================
//Debug
#ifdef FT6236_DEBUG
		USARTx_SendString(USART_COMMUNICATE_PC, "\n======================================\n");

		for (TempCounter = 0; TempCounter < ValidCounter; TempCounter++)
		{
			sprintf(FT6236_TempString, "Valid Data: %d,%d\n", FT6236_X_Valid[TempCounter], FT6236_Y_Valid[TempCounter]);
			USARTx_SendString(USART_COMMUNICATE_PC, FT6236_TempString);
		}

		sprintf(FT6236_TempString, "Valid Count: %d\n", ValidCounter);
		USARTx_SendString(USART_COMMUNICATE_PC, FT6236_TempString);
#endif
		//=====================================================================================================
		//Calculates Mean of Valid Data
		for (TempCounter = 0; TempCounter < ValidCounter; TempCounter++)
		{
			SumOf_FT6236_X_Valid = SumOf_FT6236_X_Valid + FT6236_X_Valid[TempCounter];
			SumOf_FT6236_Y_Valid = SumOf_FT6236_Y_Valid + FT6236_Y_Valid[TempCounter];
		}

		MeanOf_FT6236_X_Valid = SumOf_FT6236_X_Valid / ValidCounter;
		MeanOf_FT6236_Y_Valid = SumOf_FT6236_Y_Valid / ValidCounter;

		//		DebugPrint("\rRTPValid: %d,%d.", MeanOf_FT6236_X_Valid, MeanOf_FT6236_Y_Valid);

		//=====================================================================================================
		//Calculates Deviation from Mean of Valid Data
		for (TempCounter = 0; TempCounter < ValidCounter; TempCounter++)
		{
			DeviationOf_FT6236_X_Valid[TempCounter] = FT6236_X_Valid[TempCounter] - MeanOf_FT6236_X_Valid;
			DeviationOf_FT6236_Y_Valid[TempCounter] = FT6236_Y_Valid[TempCounter] - MeanOf_FT6236_Y_Valid;
		}
		//=====================================================================================================
		//Calculates Variance of Valid Data
		for (TempCounter = 0; TempCounter < ValidCounter; TempCounter++)
		{
			VarianceOf_FT6236_X_Valid = VarianceOf_FT6236_X_Valid + DeviationOf_FT6236_X_Valid[TempCounter] * DeviationOf_FT6236_X_Valid[TempCounter];
			VarianceOf_FT6236_Y_Valid = VarianceOf_FT6236_Y_Valid + DeviationOf_FT6236_Y_Valid[TempCounter] * DeviationOf_FT6236_Y_Valid[TempCounter];
		}

		VarianceOf_FT6236_X_Valid = VarianceOf_FT6236_X_Valid / ValidCounter;
		VarianceOf_FT6236_Y_Valid = VarianceOf_FT6236_Y_Valid / ValidCounter;
		//=====================================================================================================
		//Calculates Standard Deviation of Valid Data
		StdOf_FT6236_X_Valid = sqrt(VarianceOf_FT6236_X_Valid);
		StdOf_FT6236_Y_Valid = sqrt(VarianceOf_FT6236_Y_Valid);
		//=====================================================================================================
		if (StdOf_FT6236_X_Valid != 0)
		{
			//==================================================================================================
			//Calculates Z-Score of Valid Data
			for (TempCounter = 0; TempCounter < ValidCounter; TempCounter++)
			{
				Z_Score_FT6236_X_Valid[TempCounter] = DeviationOf_FT6236_X_Valid[TempCounter] / StdOf_FT6236_X_Valid;
			}
			//==================================================================================================
			//Determines Usual and Unusual Data. Caclculates Mean of Usual Data
			SumOf_FT6236_X_Valid = 0;

			for (TempCounter = 0; TempCounter < ValidCounter; TempCounter++)
			{
				if ((Z_Score_FT6236_X_Valid[TempCounter] >= (-1)) && (Z_Score_FT6236_X_Valid[TempCounter] <= (1)))
				{
					SumOf_FT6236_X_Valid = SumOf_FT6236_X_Valid + FT6236_X_Valid[TempCounter];

					UsualCounter_FT6236_X_Valid = UsualCounter_FT6236_X_Valid + 1;
				}
			}

			MeanOf_FT6236_X_Usual = SumOf_FT6236_X_Valid / UsualCounter_FT6236_X_Valid;
			//==================================================================================================
		}
		else
		{
			MeanOf_FT6236_X_Usual = MeanOf_FT6236_X_Valid;
		}
		//=====================================================================================================
		if (StdOf_FT6236_Y_Valid != 0)
		{
			//==================================================================================================
			//Calculates Z-Score of Valid Data
			for (TempCounter = 0; TempCounter < ValidCounter; TempCounter++)
			{
				Z_Score_FT6236_Y_Valid[TempCounter] = DeviationOf_FT6236_Y_Valid[TempCounter] / StdOf_FT6236_Y_Valid;
			}
			//==================================================================================================
			//Determines Usual and Unusual Data. Caclculates Mean of Usual Data
			SumOf_FT6236_Y_Valid = 0;

			for (TempCounter = 0; TempCounter < ValidCounter; TempCounter++)
			{
				if ((Z_Score_FT6236_Y_Valid[TempCounter] >= (-1)) && (Z_Score_FT6236_Y_Valid[TempCounter] <= (1)))
				{
					SumOf_FT6236_Y_Valid = SumOf_FT6236_Y_Valid + FT6236_Y_Valid[TempCounter];

					UsualCounter_FT6236_Y_Valid = UsualCounter_FT6236_Y_Valid + 1;
				}
			}

			MeanOf_FT6236_Y_Usual = SumOf_FT6236_Y_Valid / UsualCounter_FT6236_Y_Valid;
			//==================================================================================================
		}
		else
		{
			MeanOf_FT6236_Y_Usual = MeanOf_FT6236_Y_Valid;
		}
		//=====================================================================================================
		/*Rotates
		*Touch_X_Position = MeanOf_FT6236_Y_Usual;
		*Touch_Y_Position = 320 - MeanOf_FT6236_X_Usual;*/

		*Touch_X_Position = MeanOf_FT6236_X_Usual;
		*Touch_Y_Position = MeanOf_FT6236_Y_Usual;

//Debug
#ifdef FT6236_DEBUG
		sprintf(FT6236_TempString, "Mean Usual: %d,%d\n", MeanOf_FT6236_X_Usual, MeanOf_FT6236_Y_Usual);
		USARTx_SendString(USART_COMMUNICATE_PC, FT6236_TempString);
#endif
		//=====================================================================================================
		ValidCounter = 0;
	}
	else
	{
		*Touch_X_Position = -1;
		*Touch_Y_Position = -1;
	}
	//========================================================================================================
	//========================================================================================================
}
//===========================================================================================================
//===========================================================================================================

extern System_t xSystem;
/*Tick 10ms*/
void CTP_GetTouchPoint_Tick(void)
{
	int16_t X, Y;

	if (xSystem.CTP.CTP_Sens > 0)
		return;

	if (GPIO_ReadInputDataBit(FT6236_IRQ_PORT, FT6236_IRQ) == 0)
	{
		FT6236_Read_Touch_Position(&X, &Y);

		xSystem.GLStatus.TouchPoint.X = X;
		xSystem.GLStatus.TouchPoint.Y = Y;

		xSystem.TimeOut.Clk1s_CountToTurnOff = 0;
		
		GUI_TOUCH_StoreState(X, Y);
		GUI_Exec();
	}
	else
		//Touch Screen is not Press
		GUI_TOUCH_StoreState(-1, -1);
}
