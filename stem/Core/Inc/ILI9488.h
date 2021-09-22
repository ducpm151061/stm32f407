#ifndef __ILI9488_H__
#define __ILI9488_H__

#include "DataDefine.h"

//Defines LCD Specification
#define LCD_WITDH_SIZE 320
#define LCD_HIGH_SIZE 480
#define LCD_PIXEL_SIZE (LCD_WITDH_SIZE * LCD_HIGH_SIZE)
/*Command List*/

#define CMD_RESET 0x01
#define CMD_READ_ID 0x04
#define CMD_READ_STATUS 0x09
#define CMD_READ_PWRMODE 0x0A
#define CMD_READ_PIXELFORMAT 0x0C
#define CMD_READ_IMGMODE 0x0D
#define CMD_READ_SIGMODE 0x0E
#define CMD_NORMALMODE 0x13
#define CMD_INVERSION_OFF 0x20
#define CMD_INVERSION_ON 0x21
#define CMD_ALL_OFF 0x22
#define CMD_ALL_ON 0x23
#define CMD_DISPLAY_OFF 0x28
#define CMD_DISPLAY_ON 0x29
#define CMD_COLUMNSET 0x2A
#define CMD_PAGESET 0x2B
#define CMD_MEMORY_WRITE 0x2C /*0x2C D1 D2 ... Dn*/
#define CMD_MEMORY_READ 0x2E
#define CMD_ACCESS_MEMORY 0x36   /*0x36 0x00*/
#define CMD_SET_PIXELFORMAT 0x3A /*0x3A 0xXX ([DPI]x[DBI]) 24bit 0x07; 18 bit 0x06; 16 bit 0x05*/
#define CMD_MEMORY_WRITE_CONTINUE 0x3C
#define CMD_MEMORY_READ_CONTINUE 0x3E
#define CMD_SET_BRIGHTNESS 0x51 /*0x51 0x00-0xFF*/
#define CMD_READ_BRIGHTNESS 0x52
#define CMD_DISLPLAY_CTRL 0x53 /*0x53 [0 0 BR 0 DIM BL 0 0]*/
#define CMD_READ_DISLPLAY_CTRL 0x54

#define ILI9488_Software_Reset 0x01
#define ILI9488_Read_Display_Identification 0x04
#define ILI9488_Read_Number_Of_The_Errors_On_DSI 0x05
#define ILI9488_Read_Display_Status 0x09
#define ILI9488_Read_Display_Power_Mode 0x0A
#define ILI9488_Read_Display_MADCTL 0x0B
#define ILI9488_Read_Display_Pixel_Format 0x0C
#define ILI9488_Read_Display_Image_Mode 0x0D
#define ILI9488_Read_Display_Signal_Mode 0x0E
#define ILI9488_Read_Display_Self_Diagnostic_Result 0x0F
#define ILI9488_Sleep_IN 0x10
#define ILI9488_Sleep_OUT 0x11
#define ILI9488_Partial_Mode_ON 0x12
#define ILI9488_Normal_Display_Mode_ON 0x13
#define ILI9488_Display_Inversion_OFF 0x20
#define ILI9488_Display_Inversion_ON 0x21
#define ILI9488_All_Pixel_OFF 0x22
#define ILI9488_All_Pixel_ON 0x23
#define ILI9488_Display_OFF 0x28
#define ILI9488_Display_ON 0x29
#define ILI9488_Column_Address_Set 0x2A
#define ILI9488_Page_Address_Set 0x2B
#define ILI9488_Memory_Write 0x2C
#define ILI9488_Memory_Read 0x2E
#define ILI9488_Partial_Area 0x30
#define ILI9488_Vertical_Scrolling_Definition 0x33
#define ILI9488_Tearing_Effect_Line_OFF 0x34
#define ILI9488_Tearing_Effect_Line_ON 0x35
#define ILI9488_Memory_Access_Control 0x36
#define ILI9488_Vertical_Scrolling_Start_Address 0x37
#define ILI9488_Idle_Mode_OFF 0x38
#define ILI9488_Idle_Mode_ON 0x39
#define ILI9488_Interface_Pixel_Format 0x3A
#define ILI9488_Memory_Write_Continue 0x3C
#define ILI9488_Memory_Read_Continue 0x3E
#define ILI9488_Set_Tear_Scan_Line 0x44
#define ILI9488_Get_Scan_Line 0x45
#define ILI9488_Write_Display_Brightness 0x51
#define ILI9488_Read_Display_Brightness 0x52
#define ILI9488_Write_CTRL_Display 0x53
#define ILI9488_Read_CTRL_Display 0x54
#define ILI9488_Write_Content_Adaptive_Brightness 0x55
#define ILI9488_Read_Content_Adaptive_Brightness 0x56
#define ILI9488_Write_CABC_Minimum_Brightness 0x5E
#define ILI9488_Read_CABC_Minimum_Brightness 0x5F
#define ILI9488_Read_Automatic_Brightness_Control 0x68
#define ILI9488_Read_ID1 0xDA
#define ILI9488_Read_ID2 0xDB
#define ILI9488_Read_ID3 0xDC

#define ILI9488_Interface_Mode_Control 0xB0
#define ILI9488_Frame_Control_Normal_Mode 0xB1
#define ILI9488_Frame_Control_Idle_Mode 0xB2
#define ILI9488_Frame_Control_Partial_Mode 0xB3
#define ILI9488_Display_Inversion_Control 0xB4
#define ILI9488_Blanking_Porch_Control 0xB5
#define ILI9488_Display_Function_Control 0xB6
#define ILI9488_Entry_Mode_Set 0xB7
#define ILI9488_Color_Enhancement_Control_1 0xB9
#define ILI9488_Color_Enhancement_Control_2 0xBA
#define ILI9488_HS_Lanes_Control 0xBE
#define ILI9488_Power_Control_1 0xC0
#define ILI9488_Power_Control_2 0xC1
#define ILI9488_Power_Control_3 0xC2
#define ILI9488_Power_Control_4 0xC3
#define ILI9488_Power_Control_5 0xC4
#define ILI9488_VCOM_Control 0xC5
#define ILI9488_CABC_Control_1 0xC6
#define ILI9488_CABC_Control_2 0xC8
#define ILI9488_CABC_Control_3 0xC9
#define ILI9488_CABC_Control_4 0xCA
#define ILI9488_CABC_Control_5 0xCB
#define ILI9488_CABC_Control_6 0xCC
#define ILI9488_CABC_Control_7 0xCD
#define ILI9488_CABC_Control_8 0xCE
#define ILI9488_CABC_Control_9 0xCF
#define ILI9488_NV_Memory_Write 0xD0
#define ILI9488_NV_Memory_Protection_Key 0xD1
#define ILI9488_NV_Memory_Status_Read 0xD2
#define ILI9488_Read_ID4 0xD3
#define ILI9488_Adjust_Control_1 0xD7

#define ILI9488_Positive_Gamma_Control 0xE0
#define ILI9488_Negative_Gamma_Control 0xE1
#define ILI9488_Digital_Gamma_Control_1 0xE2
#define ILI9488_Digital_Gamma_Control_2 0xE3
#define ILI9488_Set_Image_Function 0xE9
#define ILI9488_Adjust_Control_2 0xF2
#define ILI9488_Adjust_Control_3 0xF7
#define ILI9488_Adjust_Control_4 0xF8
#define ILI9488_Adjust_Control_5 0xF9
#define ILI9488_SPI_Read_Command_Setting 0xFB
#define ILI9488_Adjust_Control_6 0xFC

//===========================================================================================================
//===========================================================================================================
#define BIT0 (1 << 0)
#define BIT1 (1 << 1)
#define BIT2 (1 << 2)
#define BIT3 (1 << 3)
#define BIT4 (1 << 4)
#define BIT5 (1 << 5)
#define BIT6 (1 << 6)
#define BIT7 (1 << 7)
#define BIT8 (1 << 8)
#define BIT9 (1 << 9)
#define BIT10 (1 << 10)
#define BIT11 (1 << 11)
#define BIT12 (1 << 12)
#define BIT13 (1 << 13)
#define BIT14 (1 << 14)
#define BIT15 (1 << 15)
#define BIT16 (1 << 16)
#define BIT17 (1 << 17)
#define BIT18 (1 << 18)
#define BIT19 (1 << 19)
#define BIT20 (1 << 20)
#define BIT21 (1 << 21)
#define BIT22 (1 << 22)
#define BIT23 (1 << 23)
#define BIT24 (1 << 24)
#define BIT25 (1 << 25)
#define BIT26 (1 << 26)
#define BIT27 (1 << 27)
#define BIT28 (1 << 28)
#define BIT29 (1 << 29)
#define BIT30 (1 << 30)
#define BIT31 (1 << 31)

//void ILI9488_WriteCommand(uint16_t Cmd);
//void ILI9488_WriteRAMByte(uint32_t RGB_Code);
//void ILI9488_WriteRAMMultiByte(uint32_t* Buffer, uint32_t Length);
//void ILI9488_ReadMultiRAM(uint32_t* Buffer, uint16_t Length);

//uint32_t ILI9488_ReadPixel(uint16_t x, uint16_t y);
//void ILI9488_WritePixel(uint16_t x, uint16_t y, uint32_t RGB_Code);
//void ILI9488_WritePixelArray(uint16_t Start_x, uint16_t Start_y, uint16_t End_x, uint16_t End_y, uint32_t RGB_Buffer);
//void ILI9488_Config(void);
//void ILI9488_SetCommand(uint16_t Cmd, uint16_t* Parameter, uint32_t Length);

//void ILI9488_Init(void);

void ILI9488_Init(void);
void ILI9488_PowerOn(void);
void ILI9488_PowerOff(void);
void ILI9488_Write_Command(uint16_t Command);
void ILI9488_Write_Command_Data(uint16_t Command_Data);
void ILI9488_Write_GRAM(uint16_t Color);

uint16_t Convert_BGR2RGB(uint16_t Color);
uint16_t RGB565_converter(uint8_t R, uint8_t G, uint8_t B);
void Swap(uint16_t *a, uint16_t *b);

void ILI9488_LCD_Set_Orientation(uint8_t Orientation_Mode);
void ILI9488_Set_Address_Pointer(uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2);
void ILI9488_LCD_Fill(uint16_t Color);
void ILI9488_LCD_Fill_Area(uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2, uint16_t Color);
void ILI9488_LCD_Draw_Pixel(uint16_t X, uint16_t Y, uint16_t Color);
void ILI9488_LCD_Draw_Font_Pixel(uint16_t X, uint16_t Y, uint16_t Color, uint8_t Pixel_Size);
void ILI9488_LCD_Draw_BMP(uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2, const uint16_t *Bitmap);

void ILI9488_DisplayOn(void);
void ILI9488_DisplayOff(void);

#endif
