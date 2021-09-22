/*******************************************************************************
  * @file    	: TPRT_Driver.c
  * @author  	: 
  * @version 	: V1.0.0
  * @date    	: 
  * @brief   	: 
*******************************************************************************/

/* Includes */
#include "TPRT_Driver.h"
void TPRT_POW_ON(void)
{
	GPIO_SetBits(TPRT_POWER_GPIO, TPRT_POWER_PIN);
}
void TPRT_POW_OFF(void)
{
	GPIO_ResetBits(TPRT_POWER_GPIO, TPRT_POWER_PIN);
}

/*******************************************************************************
 * Function Name  	: TPRT_PowerControl
 * Return         	: None
 * Parameters 		: None
 * Description		: Khoi tao Flash HeadPrinter
*******************************************************************************/
void TPRT_PowerControlInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(TPRT_POWER_GPIO_CLK, ENABLE);

	/* MotorPort Configuration*/
	GPIO_InitStructure.GPIO_Pin = TPRT_POWER_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(TPRT_POWER_GPIO, &GPIO_InitStructure);
	TPRT_POW_ON();
}

/*******************************************************************************
 * Function Name  	: TPRT_InitHardware
 * Return         	: None
 * Parameters 		: None
 * Description		: Khoi tao ngoai vi giao tiep voi Flash HeadPrinter
*******************************************************************************/
void TPRT_InitHardware(void)
{
	//SPI_InitTypeDef SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	/*Enable SPI2 and GPIO clocks*/
	RCC_APB2PeriphClockCmd(TPRT_SPI_CLK, ENABLE);
	RCC_AHB1PeriphClockCmd(TPRT_SPI_GPIO_CLK, ENABLE);

	RCC_AHB1PeriphClockCmd(TPRT_STB_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Pin = TPRT_STB1_PIN | TPRT_STB23_PIN | TPRT_STB4_PIN | TPRT_STB56_PIN;
	GPIO_Init(TPRT_STB_GPIO, &GPIO_InitStructure);
	TPRT_DisableAllStrobe();

	RCC_AHB1PeriphClockCmd(TPRT_PAPER_DETECT_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = TPRT_PAPER_DETECT_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(TPRT_PAPER_DETECT_GPIO, &GPIO_InitStructure);

	/* Configure SPI */
	RCC_AHB1PeriphClockCmd(TPRT_CS_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = TPRT_CS_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(TPRT_CS_GPIO, &GPIO_InitStructure);
	TPRT_Disable();

	/* DeInit SPI2 
	SPI_I2S_DeInit(TPRT_SPI);
	SPI_Cmd(TPRT_SPI, DISABLE);
	
	GPIO_InitStructure.GPIO_Pin = TPRT_CLK_PIN | TPRT_MOSI_PIN | TPRT_MISO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(TPRT_SPI_GPIO, &GPIO_InitStructure);
	
	// Connect SPI3 pins to SPI alternate function
	GPIO_PinAFConfig(TPRT_SPI_GPIO, TPRT_CLK_PINSOURCE, GPIO_AF_SPI2);
	GPIO_PinAFConfig(TPRT_SPI_GPIO, TPRT_MOSI_PINSOURCE, GPIO_AF_SPI2);
	GPIO_PinAFConfig(TPRT_SPI_GPIO, TPRT_MISO_PINSOURCE, GPIO_AF_SPI2);
	
//	SPI_RxFIFOThresholdConfig (TPRT_SPI, SPI_RxFIFOThreshold_QF);
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;			// Set to full duplex mode, seperate MOSI and MISO lines
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;     							// Transmit in master mode, NSS pin has to be always high
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;							// One packet of data is 8 bits wide
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;									// Clock is low when idle
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;								// Data sampled at first edge
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;									// Set the NSS management to internal and pull internal NSS high
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;			// SPI frequency is APB2 frequency / 4
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;							// Data is transmitted MSB first
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(TPRT_SPI, &SPI_InitStructure); 
	SPI_CalculateCRC(TPRT_SPI, DISABLE);
	SPI_Cmd(TPRT_SPI, ENABLE);				// Enable SPI2
	
	*/
}

/*******************************************************************************
 * Function Name  	: TPRT_SPI_TransferByte
 * Return         	: None
 * Parameters 		: None
 * Description		: Gui du lieu sang HeadPrinter bang giao tiep SPI2
*******************************************************************************/
void TPRT_SPI_TransferByte(uint8_t Data)
{
	TPRT_SPI_I2S_SendByte(Data);
}

/*******************************************************************************
 * Function Name  	: TPRT_WriteBytes
 * Return         	: None
 * Parameters 		: Dia chi can ghi, bo dem can ghi, chieu dai mang can ghi
 * Description		: Ghi mang du lieu vao trong FLASH
*******************************************************************************/
void TPRT_WriteLine(uint8_t *Buffer)
{
	uint8_t i;
	for (i = 0; i < 48; i++)
	{
		TPRT_SPI_TransferByte(Buffer[i]);
	}
}

/*******************************************************************************
 * Function Name  	: Disable_AllStrobe
 * Return         	: None
 * Parameters 		: Dia chi can ghi, bo dem can ghi, chieu dai mang can ghi
 * Description		: Ghi mang du lieu vao trong FLASH
*******************************************************************************/
void TPRT_DisableAllStrobe(void)
{
	Strobe1_Disable();
	Strobe2_Disable();
	Strobe3_Disable();
	Strobe4_Disable();
	Strobe5_Disable();
	Strobe6_Disable();
}

/*******************************************************************************
 * Function Name  	: Enable_AllStrobe
 * Return         	: None
 * Parameters 		: Dia chi can ghi, bo dem can ghi, chieu dai mang can ghi
 * Description		: Ghi mang du lieu vao trong FLASH
*******************************************************************************/
void TPRT_EnableAllStrobe(void)
{
	Strobe1_Enable();
	Strobe2_Enable();
	Strobe3_Enable();
	Strobe4_Enable();
	Strobe5_Enable();
	Strobe6_Enable();
}

/*******************************************************************************
 * Function Name  	: PaPer_Detect
 * Return         	: None
 * Parameters 		: Dia chi can ghi, bo dem can ghi, chieu dai mang can ghi
 * Description		: Ghi mang du lieu vao trong FLASH
*******************************************************************************/
uint8_t TPRT_PaPerDetect(void)
{
	uint8_t TrangThai = 0;

	TrangThai = GPIO_ReadInputDataBit(TPRT_PAPER_DETECT_GPIO, TPRT_PAPER_DETECT_PIN);

	if (TrangThai == 1)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/*******************************************************************************
 * Function Name  	: SPI_FLASH_SendByte
 * Return         	: None
 * Parameters 		: Byte can gui sang FLASH
 * Description		: Ham giao tiep voi FLASH
*******************************************************************************/
extern uint16_t SPI_SendByte(SPI_TypeDef *SPIx, uint8_t Data);
void TPRT_SPI_I2S_SendByte(uint8_t Data)
{
	SPI_SendByte(TPRT_SPI, Data);
	/* Loop while DR register in not emplty */
	//	while (SPI_I2S_GetFlagStatus(TPRT_SPI, SPI_I2S_FLAG_TXE) == RESET)
	//    {}

	/* Send byte through the SPI2 peripheral */
	//	SPI_I2S_SendData(TPRT_SPI, Data);
}

/*******************************************************************************
 * Function Name  	: TPRT_Disable3Strobe1
 * Return         	: None
 * Parameters 		: 
 * Description		: 
*******************************************************************************/
void TPRT_Disable3Strobe1(void)
{
	Strobe1_Disable();
	Strobe2_Disable();
	Strobe3_Disable();
}
/*******************************************************************************
 * Function Name  	: TPRT_Disable3Strobe2
 * Return         	: None
 * Parameters 		: 
 * Description		: 
*******************************************************************************/
void TPRT_Disable3Strobe2(void)
{
	Strobe4_Disable();
	Strobe5_Disable();
	Strobe6_Disable();
}
/*******************************************************************************
 * Function Name  	: TPRT_Enable3Strobe2
 * Return         	: None
 * Parameters 		: 
 * Description		: 
*******************************************************************************/
void TPRT_Enable3Strobe2(void)
{
	Strobe4_Enable();
	Strobe5_Enable();
	Strobe6_Enable();
}
/*******************************************************************************
 * Function Name  	: TPRT_Enable3Strobe1
 * Return         	: None
 * Parameters 		: 
 * Description		: 
*******************************************************************************/
void TPRT_Enable3Strobe1(void)
{
	Strobe1_Enable();
	Strobe2_Enable();
	Strobe3_Enable();
}

/*******************************************************************************
 * Function Name  	: TPRT_Enable2Strobe1
 * Return         	: None
 * Parameters 		: 
 * Description		: 
*******************************************************************************/
void TPRT_Enable2Strobe1(void)
{
	Strobe1_Enable();
	Strobe2_Enable();
}

/*******************************************************************************
 * Function Name  	: TPRT_Enable2Strobe2
 * Return         	: None
 * Parameters 		: 
 * Description		: 
*******************************************************************************/
void TPRT_Enable2Strobe2(void)
{
	Strobe3_Enable();
	Strobe4_Enable();
}

/*******************************************************************************
 * Function Name  	: TPRT_Enable2Strobe3
 * Return         	: None
 * Parameters 		: 
 * Description		: 
*******************************************************************************/
void TPRT_Enable2Strobe3(void)
{
	Strobe5_Enable();
	Strobe6_Enable();
}

/*******************************************************************************
 * Function Name  	: TPRT_Disable2Strobe1
 * Return         	: None
 * Parameters 		: 
 * Description		: 
*******************************************************************************/
void TPRT_Disable2Strobe1(void)
{
	Strobe1_Disable();
	Strobe2_Disable();
}

/*******************************************************************************
 * Function Name  	: TPRT_Disable2Strobe2
 * Return         	: None
 * Parameters 		: 
 * Description		: 
*******************************************************************************/
void TPRT_Disable2Strobe2(void)
{
	Strobe3_Disable();
	Strobe4_Disable();
}

/*******************************************************************************
 * Function Name  	: TPRT_Disable2Strobe3
 * Return         	: None
 * Parameters 		: 
 * Description		: 
*******************************************************************************/
void TPRT_Disable2Strobe3(void)
{
	Strobe5_Disable();
	Strobe6_Disable();
}

/********************************* END OF FILE *******************************/
