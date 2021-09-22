/******************************************************************************
 * @file        DriverSPI.c
 * @author
 * @version     V1.0.0
 * @date
 * @brief
 ******************************************************************************/

/******************************************************************************
                                   INCLUDES
 ******************************************************************************/
#include "DataDefine.h"

/******************************************************************************
                                   GLOBAL VARIABLES
 ******************************************************************************/

/******************************************************************************
                                   DATA TYPE DEFINE
 ******************************************************************************/

/******************************************************************************
                                   PRIVATE VARIABLES
 ******************************************************************************/

/******************************************************************************
                                   LOCAL FUNCTIONS
 ******************************************************************************/

/*****************************************************************************/
/**
 * @brief   :
 * @param   :
 * @retval  :
 * @author  :
 * @created :
 * @comment :
 */
void InitSPI(SPI_TypeDef *SPIx)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef SPI_InitStructure;

    if (ADC_SPI == SPIx)
    {
        GPIO_PinAFConfig(ADC_MISO_PORT, ADC_MISO_PIN_SOURCE, ADC_SPI_AF);
        GPIO_PinAFConfig(ADC_SCK_PORT, ADC_SCK_PIN_SOURCE, ADC_SPI_AF);

        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;

        /* SPI SCK pin configuration */
        GPIO_InitStructure.GPIO_Pin = ADC_SCK_PIN;
        GPIO_Init(ADC_SCK_PORT, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
        GPIO_InitStructure.GPIO_Pin = ADC_MISO_PIN;
        GPIO_Init(ADC_MISO_PORT, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;

        GPIO_InitStructure.GPIO_Pin = ADC_CS_PIN;
        GPIO_Init(ADC_CS_PORT, &GPIO_InitStructure);

        SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32; //  42 / 16 = 1.875 MHz
        SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
        SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //SPI_Direction_1Line_Rx;
        SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
        SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;
    }
    //  if (RTP_SPI == SPIx)
    //  {
    //      GPIO_PinAFConfig(RTP_CLK_Port, RTP_SCK_PIN_SOURCE, RTP_SPI_AF);
    //      GPIO_PinAFConfig(RTP_DOUT_Port, RTP_MOSI_PIN_SOURCE, RTP_SPI_AF);
    //      GPIO_PinAFConfig(RTP_DIN_Port, RTP_MISO_PIN_SOURCE, RTP_SPI_AF);

    //      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    //      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    //      GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    //      GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;

    //      /* SPI SCK pin configuration */
    //      GPIO_InitStructure.GPIO_Pin = RTP_CLK_Pin;
    //      GPIO_Init(RTP_CLK_Port, &GPIO_InitStructure);

    //      /* SPI  MOSI pin configuration */
    //      GPIO_InitStructure.GPIO_Pin =  RTP_DOUT_Pin;
    //      GPIO_Init(RTP_DOUT_Port, &GPIO_InitStructure);
    //
    //      GPIO_InitStructure.GPIO_Pin =  RTP_DIN_Pin;
    //      GPIO_Init(RTP_DIN_Port, &GPIO_InitStructure);
    //
    //      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    //      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    //      GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    //      GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    //
    //      GPIO_InitStructure.GPIO_Pin =  RTP_CS_Pin;
    //      GPIO_Init(RTP_CS_Port, &GPIO_InitStructure);
    //
    //      SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;     //  84 / 16
    //      SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    //      SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    //      SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    //      SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    //  }
    if (FLASH_SPI == SPIx)
    {
        GPIO_PinAFConfig(FLASH_SCK_PORT, FLASH_SCK_PIN_SOURCE, FLASH_SPI_AF);
        GPIO_PinAFConfig(FLASH_MOSI_PORT, FLASH_MOSI_PIN_SOURCE, FLASH_SPI_AF);
        GPIO_PinAFConfig(FLASH_MISO_PORT, FLASH_MISO_PIN_SOURCE, FLASH_SPI_AF);

        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;

        /* SPI SCK pin configuration */
        GPIO_InitStructure.GPIO_Pin = FLASH_SCK_PIN;
        GPIO_Init(FLASH_SCK_PORT, &GPIO_InitStructure);

        /* SPI  MOSI pin configuration */
        GPIO_InitStructure.GPIO_Pin = FLASH_MOSI_PIN;
        GPIO_Init(FLASH_MOSI_PORT, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin = FLASH_MISO_PIN;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
        GPIO_Init(FLASH_MISO_PORT, &GPIO_InitStructure);

        // Connect SPI3 pins to SPI alternate function
        GPIO_PinAFConfig(FLASH_MISO_PORT, FLASH_MISO_PIN_SOURCE, GPIO_AF_SPI2);

        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

        GPIO_InitStructure.GPIO_Pin = FLASH_CS1_PIN;
        GPIO_Init(FLASH_CS1_PORT, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin = FLASH_CS2_PIN;
        GPIO_Init(FLASH_CS2_PORT, &GPIO_InitStructure);

        SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
        SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64; //  84 / 16
        SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
        SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
        SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
        //SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;           // Cho may in nhiet
    }

    /* SPI configuration -------------------------------------------------------*/
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;

    //  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    //  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;

    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPIx, &SPI_InitStructure);

    SPI_CalculateCRC(SPIx, DISABLE);
    SPI_Cmd(SPIx, ENABLE);
}

/*****************************************************************************/
/**
 * @brief   :
 * @param   :
 * @retval  :
 * @author  :
 * @created :
 * @comment :
 */
uint16_t SPI_SendByte(SPI_TypeDef *SPIx, uint8_t Data)
{
    uint32_t Timeout;

    if (ADC_SPI == SPIx)
        Timeout = 0;
    else
        Timeout = 0x1FFFFF;
    while ((SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET) && (Timeout > 1))
        Timeout--;

    SPI_I2S_SendData(SPIx, Data);

    if (ADC_SPI == SPIx)
        Timeout = 0;

    else
        Timeout = 0x1FFFFF;
    while ((SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET) && (Timeout > 1))
        Timeout--;

    return SPI_I2S_ReceiveData(SPIx);
}

SPI_Driver_t SPI_Driver =
    {
        InitSPI,
        SPI_SendByte};

/********************************* END OF FILE *******************************/
