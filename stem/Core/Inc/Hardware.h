#ifndef __HARDWARE_H__
#define __HARDWARE_H__

#include "stm32f4xx_conf.h"

#define BUZZER_PIN GPIO_Pin_9
#define BUZZER_PORT GPIOC

#define VIN_PORT GPIOC
#define VIN_PIN GPIO_Pin_1
#define VIN_CHANNEL ADC_Channel_11

#define BATTERY_PORT GPIOC
#define BATTERY_PIN GPIO_Pin_2
#define BATTERY_CHANNEL ADC_Channel_12

#define V8_2_PORT GPIOC
#define V8_2_PIN GPIO_Pin_3
#define V8_2_CHANNEL ADC_Channel_13

#define TPRT_TEMP_PORT GPIOA
#define TPRT_TEMP_PIN GPIO_Pin_3
#define TPRT_TEMP_CHANNEL ADC_Channel_3

#define PRESSURE_SENSOR_PORT GPIOB
#define PRESSURE_SENSOR_PIN GPIO_Pin_0
#define PRESSURE_SENSOR_CHANNEL ADC_Channel_8

#define TEMPERATURE_SENSOR_PORT GPIOB
#define TEMPERATURE_SENSOR_PIN GPIO_Pin_1
#define TEMPERATURE_SENSOR_CHANNEL ADC_Channel_9

/************************* DEBUG ***************************************/

#define DEBUG_UART USART3
#define DEBUG_UART_IRQ USART3_IRQn
#define DEBUG_IRQHandler USART3_IRQHandler

#define DEBUG_TX_PIN_SOURCE GPIO_PinSource10
#define DEBUG_RX_PIN_SOURCE GPIO_PinSource11
#define DEBUG_TX_PIN GPIO_Pin_10
#define DEBUG_RX_PIN GPIO_Pin_11
#define DEBUG_UART_IO_AF GPIO_AF_USART3
#define DEBUG_UART_GPIO GPIOB

/************************* GPS MODULE ***************************************/

#define GPS_UART USART2
#define GPS_UART_IRQ USART2_IRQn
#define GPS_IRQHandler USART2_IRQHandler

#define GPS_TX_PIN_SOURCE GPIO_PinSource5
#define GPS_RX_PIN_SOURCE GPIO_PinSource6
#define GPS_TX_PIN GPIO_Pin_5
#define GPS_RX_PIN GPIO_Pin_6
#define GPS_UART_IO_AF GPIO_AF_USART2
#define GPS_UART_GPIO GPIOD

#define GPS_RESET_PORT GPIOA
#define GPS_RESET_PIN GPIO_Pin_10

#define GPS_PWR_PORT GPIOA
#define GPS_PWR_PIN GPIO_Pin_9

/************************* AIR PUMP CONTROL ************************************/
#define AIR_PUMP_A_CTRL_PORT GPIOC
#define AIR_PUMP_A_CTRL_PIN GPIO_Pin_11

#define AIR_PUMP_B_CTRL_PORT GPIOC
#define AIR_PUMP_B_CTRL_PIN GPIO_Pin_12

#define AIR_PUMP_VCC_MEASURE_PORT GPIOC
#define AIR_PUMP_VCC_MEASURE_PIN GPIO_Pin_0
#define AIR_PUMP_VCC_MEASURE_CHANNEL ADC_Channel_10

/************************ ADS7822 ******************************************/
#define ADC_SPI SPI3
#define ADC_SPI_AF GPIO_AF_SPI3

#define ADC_CS_PIN GPIO_Pin_8
#define ADC_CS_PORT GPIOA

#define ADC_MISO_PIN_SOURCE GPIO_PinSource4
#define ADC_MISO_PIN GPIO_Pin_4
#define ADC_MISO_PORT GPIOB

#define ADC_SCK_PIN_SOURCE GPIO_PinSource3
#define ADC_SCK_PIN GPIO_Pin_3
#define ADC_SCK_PORT GPIOB

#define ADC_MOSI_PIN_SOURCE GPIO_PinSource5
#define ADC_MOSI_PIN GPIO_Pin_5
#define ADC_MOSI_PORT GPIOB

/************************* FRAM / FLASH / PRINTER ***************************************/
/************************* SPANSION ***************************/
#define FLASH_SPI SPI2
#define FLASH_GPIO_CLK RCC_AHB1Periph_GPIOB
#define FLASH_SPI_CLK RCC_APB1Periph_SPI2
#define FLASH_SPI_AF GPIO_AF_SPI2

#define FLASH_IO_ClkCmd RCC_AHB1PeriphClockCmd
#define FLASH_SPI_ClkCmd RCC_APB1PeriphClockCmd

#define FLASH_CS1_PIN GPIO_Pin_7
#define FLASH_CS1_PORT GPIOC

#define FLASH_CS2_PIN GPIO_Pin_8
#define FLASH_CS2_PORT GPIOC

#define FLASH_MOSI_PIN_SOURCE GPIO_PinSource15
#define FLASH_MOSI_PIN GPIO_Pin_15
#define FLASH_MOSI_PORT GPIOB

#define FLASH_MISO_PIN_SOURCE GPIO_PinSource14
#define FLASH_MISO_PIN GPIO_Pin_14
#define FLASH_MISO_PORT GPIOB

#define FLASH_SCK_PIN_SOURCE GPIO_PinSource13
#define FLASH_SCK_PIN GPIO_Pin_13
#define FLASH_SCK_PORT GPIOB

/*******************************************************************************/
/* LCD TFT */
#define LCD_DB_Port GPIOE

#define LCD_DB0_Pin GPIO_Pin_0
#define LCD_DB1_Pin GPIO_Pin_1
#define LCD_DB2_Pin GPIO_Pin_2
#define LCD_DB3_Pin GPIO_Pin_3
#define LCD_DB4_Pin GPIO_Pin_4
#define LCD_DB5_Pin GPIO_Pin_5
#define LCD_DB6_Pin GPIO_Pin_6
#define LCD_DB7_Pin GPIO_Pin_7
#define LCD_DB8_Pin GPIO_Pin_8
#define LCD_DB9_Pin GPIO_Pin_9
#define LCD_DB10_Pin GPIO_Pin_10
#define LCD_DB11_Pin GPIO_Pin_11
#define LCD_DB12_Pin GPIO_Pin_12
#define LCD_DB13_Pin GPIO_Pin_13
#define LCD_DB14_Pin GPIO_Pin_14
#define LCD_DB15_Pin GPIO_Pin_15

#define LCD_XCS_Pin GPIO_Pin_7
#define LCD_XCS_Port GPIOD

#define LCD_DCX_Pin GPIO_Pin_5
#define LCD_DCX_Port GPIOB

#define LCD_WRX_Pin GPIO_Pin_6
#define LCD_WRX_Port GPIOB

#define LCD_RDX_Pin GPIO_Pin_7
#define LCD_RDX_Port GPIOB

#define LCD_RST_Pin GPIO_Pin_1
#define LCD_RST_Port GPIOD

#define LCD_BACKLIGHT_Pin GPIO_Pin_0
#define LCD_BACKLIGHT_Port GPIOD

/*******************************************************************************/
/* TOUCH TFT */

//#define RTP_SPI				SPI1
//#define RTP_SPI_AF			GPIO_AF_SPI1

//#define	RTP_CS_Pin			GPIO_Pin_2
//#define	RTP_CS_Port			GPIOD

//#define RTP_SCK_PIN_SOURCE        GPIO_PinSource5
//#define	RTP_CLK_Pin			GPIO_Pin_5
//#define	RTP_CLK_Port		GPIOA

//#define RTP_MISO_PIN_SOURCE        GPIO_PinSource7
//#define	RTP_DIN_Pin			GPIO_Pin_7
//#define	RTP_DIN_Port		GPIOA

//#define RTP_MOSI_PIN_SOURCE        GPIO_PinSource6
//#define	RTP_DOUT_Pin		GPIO_Pin_6
//#define	RTP_DOUT_Port		GPIOA

//#define	RTP_BUSY_Pin		GPIO_Pin_4
//#define	RTP_BUSY_Port		GPIOA

//#define	RTP_IRQ_Pin			GPIO_Pin_4
//#define	RTP_IRQ_Port		GPIOC

/*******************************************************************************/
/* BUTTON */
#define BUTTON1_Pin GPIO_Pin_5
#define BUTTON1_Port GPIOC

#define BUTTONPWR_Pin GPIO_Pin_12
#define BUTTONPWR_Port GPIOB

#define VSYS_PWR_CTRL_Port GPIOC
#define VSYS_PWR_CTRL_Pin GPIO_Pin_13

#define LED_STATUS_Pin GPIO_Pin_15
#define LED_STATUS_Port GPIOA

/*****************************************************************/
#define ILI9488_DATA_PORT GPIOE
#define ILI9488_DATA_PORT_RCC RCC_AHB1Periph_GPIOE
//===========================================================================================================
//Defines ILI9488 Control Pin
//Power Control
#define ILI9488_PWR_CTRL GPIO_Pin_10
#define ILI9488_PWR_CTRL_PORT GPIOC
#define ILI9488_PWR_CTRL_PORT_RCC RCC_AHB1Periph_GPIOC

//Reset Control
#define ILI9488_RST GPIO_Pin_1
#define ILI9488_RST_PORT GPIOD
#define ILI9488_RST_PORT_RCC RCC_AHB1Periph_GPIOD

//Backlight Control
#define ILI9488_BL_CTRL GPIO_Pin_0
#define ILI9488_BL_CTRL_PORT GPIOD
#define ILI9488_BL_CTRL_PORT_RCC RCC_AHB1Periph_GPIOD

//Chip Select
#define ILI9488_CS GPIO_Pin_7
#define ILI9488_CS_PORT GPIOD
#define ILI9488_CS_PORT_RCC RCC_AHB1Periph_GPIOD

//Data Or Command
#define ILI9488_DC GPIO_Pin_5
#define ILI9488_DC_PORT GPIOB
#define ILI9488_DC_PORT_RCC RCC_AHB1Periph_GPIOB

//Write Signal Control
#define ILI9488_WRX GPIO_Pin_6
#define ILI9488_WRX_PORT GPIOB
#define ILI9488_WRX_PORT_RCC RCC_AHB1Periph_GPIOB

//Read Signal Control
#define ILI9488_RD GPIO_Pin_7
#define ILI9488_RD_PORT GPIOB
#define ILI9488_RD_PORT_RCC RCC_AHB1Periph_GPIOB

#endif // __HARDWARE_H__
