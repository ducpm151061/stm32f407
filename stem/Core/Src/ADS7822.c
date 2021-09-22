#include "ADS7822.h"

extern System_t xSystem;

void ADS7822_SelectChip(uint8_t enable);

void ADS7822_Init(void)
{

    xSystem.SPI->Init(ADC_SPI);
    DebugPrint("\rInit ADS7822\r\n");
}

uint16_t ADS7822_GetOneSample(void)
{
    uint16_t result = 0;
    uint16_t rawData[2];
    uint16_t timeout = 0x1FF;

    GPIO_ResetBits(ADC_CS_PORT, ADC_CS_PIN);

    rawData[0] = xSystem.SPI->SendByte(ADC_SPI, 0xFF);

    /*rawData[0] bo 3 bit dau*/

    rawData[0] &= 0x1FFF;
    rawData[0] >>= 0x1;

    //  while((SPI_I2S_GetFlagStatus(ADC_SPI, SPI_I2S_FLAG_RXNE) == SET) && timeout--);
    while (timeout--)
        ;

    GPIO_SetBits(ADC_CS_PORT, ADC_CS_PIN);
    //DebugPrint("\rADS7822 value: %d\r\n", rawData[0]);
    result = rawData[0];

    return result;
}

void ADS7822_SelectChip(uint8_t enable)
{

    //  if(enable){
    //      GPIO_ResetBits(ADC_CS_PORT, ADC_CS_PIN);
    //  }
    //  else{
    //      GPIO_SetBits(ADC_CS_PORT, ADC_CS_PIN);
    //  }
}
