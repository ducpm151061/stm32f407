
#include "string.h"
#include "TPRT.h"
#include "TPRT_Driver.h"
#include "Motor_Controler.h"
#include "Fonts.h"

/*******************************************************************************
 * Function Name  	: Fonts_GetInfo
 * Return         	: None
 * Parameters 		: 
 * Description		: Lay thong tin ve Font chu thuong
*******************************************************************************/
Fonts_t Fonts_GetInfo(uint8_t Kytu)
{

	//#if 0
	if (Kytu < 4)
	{
		Kytu = 32;
		//return Fonts_Info[Kytu];
	}
	else
	{
		if (Kytu < 32 && Kytu > 4)
			Kytu = 32;
		if (Kytu < 127)
			Kytu -= 32;
		else
			Kytu -= 32;
		// if((Kytu >= 127) && (Kytu < 160))
		// 	Kytu = 0;
		// if (Kytu >= 160)
		// 	Kytu -= 66;

		return Fonts_Info[Kytu];
	}
	//#endif

	//return Fonts_Info[Kytu-1];
	//return (Kytu-1)*60;
	return Fonts_Info[Kytu];
}

/*******************************************************************************
 * Function Name  	: TPRT_GetInfo_BigFonts
 * Return         	: None
 * Parameters 		: 
 * Description		: Lay thong tin ve Font chu to
*******************************************************************************/
Fonts_t Fonts_GetInfo_BigFonts(uint8_t Kytu)
{

	if (Kytu <= 32)
		Kytu = 0;
	if (Kytu > 32)
		Kytu -= 32;
	return Fonts_Info_Big[Kytu];
}

/*******************************************************************************
 * Function Name  	: TPRT_WriteBytes
 * Return         	: None
 * Parameters 		: Dia chi can ghi, bo dem can ghi, chieu dai mang can ghi
 * Description		: Ghi mang du lieu vao trong FLASH
*******************************************************************************/
void TPRT_ClearPRTBuffer(void)
{
	uint8_t i;
	for (i = 0; i < 48; i++)
	{
		TPRT_SPI_TransferByte(0);
	}
}

/*******************************************************************************
 * Function Name  	: Enable_Strobe
 * Return         	: None
 * Parameters 		: 
 * Description		: 
*******************************************************************************/
void Enable_Strobe(uint8_t NbOfStrobe, uint8_t HeatingTime)
{
	switch (NbOfStrobe)
	{
	case 0:
		TPRT_Disable2Strobe3();
		TPRT_Enable2Strobe1();
		break;

	case 1:
		TPRT_Disable2Strobe1();
		TPRT_Enable2Strobe2();
		break;

	case 2:
		TPRT_Disable2Strobe2();
		TPRT_Enable2Strobe3();
		break;

	default:
		TPRT_Disable();
		TPRT_DisableAllStrobe();
		break;
	}
}

/*******************************************************************************
 * Function Name  	: TPRT_PrintLine
 * Return         	: None
 * Parameters 		: 
 * Description		: Gui du lieu cua dot line can in sang dau in(chu thuong)
*******************************************************************************/
void TPRT_PrintLine(char *Buffer, uint8_t column)
{
	//uint8_t BufferAnhNen[48];
	//uint8_t i = 0;
	uint8_t Buff[48];
	uint8_t ThuTu, Length, ThuTu_End, Length_End;
	uint32_t Font_HienTai_Start, Font_TiepTheo_Start;
	Length = strlen(Buffer);

	for (ThuTu = 0; ThuTu < 48; ThuTu++)
		Buff[ThuTu] = 0x00;

	for (ThuTu = 0; ThuTu < Length; ThuTu++)
	{
		if ((ThuTu % 2) == 0)
		{
			if ((ThuTu + 1) < Length)
			{
				Font_HienTai_Start = Fonts_GetInfo(Buffer[ThuTu]).Start;
				Font_TiepTheo_Start = Fonts_GetInfo(Buffer[ThuTu + 1]).Start;
				Buff[3 * ThuTu / 2] = Fonts_Arial[Font_HienTai_Start + 2 * column];
				Buff[3 * ThuTu / 2 + 1] = (Fonts_Arial[Font_HienTai_Start + 2 * column + 1] & 0xF0) | ((Fonts_Arial[Font_TiepTheo_Start + 2 * column] >> 4) & 0x0F);
				Buff[3 * ThuTu / 2 + 2] = ((Fonts_Arial[Font_TiepTheo_Start + 2 * column] << 4) & 0xF0) | ((Fonts_Arial[Font_TiepTheo_Start + 2 * column + 1] >> 4) & 0x0F);
			}
			else if ((ThuTu + 1) == Length)
			{
				Font_HienTai_Start = Fonts_GetInfo(Buffer[ThuTu]).Start;
				Buff[3 * ThuTu / 2] = Fonts_Arial[Font_HienTai_Start + 2 * column];
				Buff[3 * ThuTu / 2 + 1] = Fonts_Arial[Font_HienTai_Start + 2 * column + 1] & 0xF0;
				Buff[3 * ThuTu / 2 + 2] = 0x00;
			}
		}
	}
	if (Length % 2 == 0)
		Length_End = Length * 3 / 2;
	else
		Length_End = (Length + 1) * 3 / 2;

	for (ThuTu_End = Length_End; ThuTu_End < 48; ThuTu_End++)
		Buff[ThuTu_End] = 0x00;
	TPRT_WriteLine(Buff);
}

/*******************************************************************************
 * @FunctionName  	: TPRT_PrintLine_BigFont
 * @author			: 
 * @Return         	: None
 * @Parameters 		: 
 * @Description		: Gui du lieu cua dot line can in sang dau in(chu to)
*******************************************************************************/
void TPRT_PrintLine_BigFont(char *Buffer, uint8_t column)
{
	uint8_t BufferAnhNen[48];
	//uint8_t i = 0;
	uint8_t Buff[48];
	static uint8_t ThuTu, Length;
	static uint32_t Start;

	Length = strlen(Buffer);
	for (ThuTu = Length; ThuTu < 48; ThuTu++)
		Buff[ThuTu] = 0x00;

	for (ThuTu = 0; ThuTu < Length; ThuTu++)
	{
		Start = Fonts_GetInfo_BigFonts(Buffer[ThuTu]).Start;
		Buff[3 * ThuTu] = Fonts_Arial_Big[Start + 3 * column];
		Buff[3 * ThuTu + 1] = Fonts_Arial_Big[Start + 3 * column + 1];
		Buff[3 * ThuTu + 2] = Fonts_Arial_Big[Start + 3 * column + 2];
	}
	for (ThuTu = Length * 3; ThuTu < 48; ThuTu++)
	{
		Buff[ThuTu] = 0x00;
	}

	TPRT_WriteLine(Buff);
}

/*******************************************************************************
 * Function Name  	: TPRT_PrintString
 * Return         	: None
 * Parameters 		: Du lieu can in, Kieu Font
 * Description		: In du lieu cua mot dong in
*******************************************************************************/
void TPRT_PrintString(char *Buffer, uint8_t Font_Size, uint8_t HeatingTime)
{
	static uint8_t DotLine = 0;

	if (TPRT_Motor_CheckIsNewLine())
	{
		if (Font_Size == FONT_NORMAL)
		{
			if ((DotLine <= FONT_NORMAL_COLUMN) && (Printer.Flag_ReqNewLine == 0))
			{
				if (DotLine == FONT_NORMAL_COLUMN)
				{
					TPRT_ClearPRTBuffer();
				}
				else
				{
					TPRT_PrintLine(Buffer, DotLine);
				}
				DotLine++;
			}
			if (DotLine > FONT_NORMAL_COLUMN)
			{
				DotLine = 0;
				Printer.Flag_ReqNewLine = 1;
				TPRT_DisableAllStrobe();
			}
		}
		Printer.NbOfStrobe = 0;
		Printer.HeatingTimeClk = HeatingTime;
		Motor.NbOfStep_OneDotLine = 2; //8 NbOfStepPerDot
	}

	if ((Printer.HeatingTimeClk > 0) && (Printer.Flag_ReqNewLine == 0))
	{
		Enable_Strobe(Printer.NbOfStrobe, HeatingTime);
	}
	if (Printer.HeatingTimeClk == 0)
	{
		Printer.HeatingTimeClk = HeatingTime;
		Printer.NbOfStrobe++;
	}
	if (Printer.NbOfStrobe > 2)
	{
		TPRT_DisableAllStrobe();
		TPRT_ClearPRTBuffer();
	}
}

/*******************************************************************************
 * Function Name  	: TPRT_PrintText_Tick
 * Return         	: None
 * Parameters 		: 
 * Description		: In du lieu nhan duoc tu cong debug o che do textmode
 * Note				: Call every in while loop
*******************************************************************************/

extern void UART_Printf(USART_TypeDef *USARTx, const char *str, ...);

void TPRT_PrintText_Tick(void)
{
	if (Printer.LinePrinted < Printer.LineNeedPrint)
	{
		/* Quay dong co 2 step de in mot hang diem */
		TPRT_PrintString((char *)&DuLieuIn[Printer.LinePrinted].Text, DuLieuIn[Printer.LinePrinted].Font_Size, Printer.HeatingTimeMax);
	}
	if (Printer.Flag_ReqNewLine == 1)
	{
		Printer.LinePrinted++;
		Printer.Flag_ReqNewLine = 0;
	}
	if (Printer.LinePrinted >= Printer.LineNeedPrint)
	{
		//UART_Printf(USART3,"\r==IN XONG==");
		Printer.TimeOut = 0;
		TPRT_Disable();
		TPRT_DisableAllStrobe();
		Motor_Stop();
		Printer.Mode = MODE_FREE;
		Printer.LinePrinted = 0;
		Printer.LineNeedPrint = 0;
		Printer.IsPrinting = 0;
		TPRT_Timer_Disable();
	}
}
