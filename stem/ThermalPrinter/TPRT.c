
#include "TPRT.h"

Printer_t Printer;
DuLieuIn_t DuLieuIn[PRINTER_NBOF_LINE];
extern System_t xSystem;

uint16_t CountTimerPRT = 0;
void TPRT_Timer_Enable(void)
{
	TIM_Cmd(TIM4, ENABLE);
}

void TPRT_Timer_Disable(void)
{
	TIM_Cmd(TIM4, DISABLE);
}

void TPRT_Timer_Tick(void)
{
	if (Printer.HeatingTimeClk > 0)
		Printer.HeatingTimeClk--;
	Printer.Type = 0;
	CountTimerPRT++;

	if (Printer.IsPrinting == 1)
	{
		if (Printer.Mode == MODE_TEXT)
		{
			TPRT_PrintText_Tick();
		}
		else if (Printer.Mode == MODE_FREE)
			Printer.Mode = MODE_TEXT;
	}

	if (Motor.TimeStepMotor > 0)
		Motor.TimeStepMotor--;
	else
		TPRT_Motor_Tick();
}

/*******************************************************************************
 * Function Name  	: TPRT_InitSoftware
 * Return         	: None
 * Parameters 		: None
 * Description		: Khoi tao thong so khi khoi dong
*******************************************************************************/
void TPRT_InitSoftware(void)
{
	Printer.Mode = MODE_FREE;
	Printer.Fonts_Size_Old = FONT_NORMAL;
	Printer.SoKyTu_MotDongIn = PRINTER_NBOF_CHARINLINE;
	Motor.NbOfStep_OneDotLine = 0;
	TPRT_DisableAllStrobe();
	MotorTest.NbOfStep = 0;
	Printer.HeatingTimeMax = 4;

	Printer.LinePrinted = 0;
	Printer.LineNeedPrint = 0;
}

/*******************************************************************************
 * Function Name  	: NapCuoc_ReceiveData
 * Return         	: None
 * Parameters 		: Du lieu nhan duoc tu cong nap cuoc
 * Description		: Dong goi du lieu nhan duoc tu cong nap cuoc
*******************************************************************************/
void Printer_ReceiveData(uint8_t Data)
{
	/* Luu Du lieu vao mang nho RxBuffer */
	if (Printer.RxCounter < PRINTER_BUFFER - 2)
		Printer.RxBuffer[Printer.RxCounter++] = Data;
	Printer.TimeOutReceive = 20;
}

/*******************************************************************************
 * Function Name        : TPRT_Timer_Initialize
 * Return               : None
 * Parameters           : None
 * Description          : Khoi tao Timer 3 dung de check TimeOut
*******************************************************************************/
void TPRT_Timer_Initialize(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* TIM4 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	TIM_Cmd(TIM4, DISABLE);

	/* Time base configuration */
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Period = 300 - 1;
	TIM_TimeBaseStructure.TIM_Prescaler = 100; //168
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

	//TIM_ARRPreloadConfig(TIM3, DISABLE);

	/* TIM IT enable */
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);

	/* Enable the TIM3 gloabal Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&NVIC_InitStructure);

	/* TIM4 enable counter */
	TIM_Cmd(TIM4, DISABLE);
}

/*******************************************************************************
* Function Name  	: TIM4_IRQHandler
* Return         	: None
* Parameters 		: None
* Description		: Ham nay xu ly ngat 5ms de xu ly cac ngoai vi
*******************************************************************************/
void TIM4_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
		TPRT_Timer_Tick();
	}
}

/*******************************************************************************
 * Function Name  	: TPRT_Initialize
 * Return         	: None
 * Parameters 		: None
 * Description		: Khoi tao Flash HeadPrinter
*******************************************************************************/
void TPRT_Initialize(void)
{
	/* Khoi tao phan cung ngoai vi */
	TPRT_PowerControlInit();
	TPRT_InitHardware();
	TPRT_InitSoftware();
	TPRT_Timer_Initialize();
	Motor_Init();

	TPRT_DisableAllStrobe();
	TPRT_AlM_StopPrint();
}

void TPRT_AlM_AddLine(uint8_t *Data)
{
	memset((uint8_t *)&DuLieuIn[Printer.LineNeedPrint].Text, 0, PRINTER_NBOF_CHARINLINE);
	memcpy((char *)&DuLieuIn[Printer.LineNeedPrint].Text, Data, PRINTER_NBOF_CHARINLINE);
	DuLieuIn[Printer.LineNeedPrint].Font_Size = FONT_NORMAL;
	Printer.LineNeedPrint++;
}

void TPRT_AlM_StartPrint(void)
{
	TPRT_POW_ON();
	Printer.IsPrinting = 1;
	TPRT_Enable();
	TPRT_Timer_Enable();
	SMT_ENABLE;
}

void TPRT_AlM_StopPrint(void)
{
	Printer.IsPrinting = 0;
	TPRT_Disable();
	TPRT_Timer_Disable();
	Printer.LinePrinted = 0;
	Printer.LineNeedPrint = 0;
	TPRT_POW_OFF();
	SMT_DISABLE;
}

void TPRT_AlM_ClearData(void)
{
	TPRT_InitSoftware();
}

uint8_t TPRT_IsPrintg(void)
{
	return Printer.IsPrinting;
}
