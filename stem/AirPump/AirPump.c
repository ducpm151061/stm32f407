#include <stdint.h>
#include <stm32f4xx.h>
#include "AirPump.h"

uint16_t AP_Timeout = 0;

void AP_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(AP_Ctrl_GPIOClk, ENABLE);

    GPIO_InitStructure.GPIO_Pin = AP_CtrlA_PIN | AP_CtrlB_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_Init(AP_Ctrl_GPIO, &GPIO_InitStructure);

    /* Enable Motor */
    GPIO_ResetBits(AP_Ctrl_GPIO, AP_CtrlA_PIN | AP_CtrlB_PIN);
    //  AP_Ctrl_Push(200);
}

void AP_Ctrl_Push(uint16_t Timeout)
{
    GPIO_ResetBits(AP_Ctrl_GPIO, AP_CtrlA_PIN | AP_CtrlB_PIN);
    GPIO_ResetBits(AP_Ctrl_GPIO, AP_CtrlA_PIN);
    GPIO_SetBits(AP_Ctrl_GPIO, AP_CtrlB_PIN);
    AP_Timeout = Timeout;
}

void AP_Ctrl_Pull(uint16_t Timeout)
{
    GPIO_ResetBits(AP_Ctrl_GPIO, AP_CtrlA_PIN | AP_CtrlB_PIN);
    GPIO_ResetBits(AP_Ctrl_GPIO, AP_CtrlB_PIN);
    GPIO_SetBits(AP_Ctrl_GPIO, AP_CtrlA_PIN);
    AP_Timeout = Timeout;
}

void AP_Ctrl_Off(void)
{
    GPIO_ResetBits(AP_Ctrl_GPIO, AP_CtrlA_PIN | AP_CtrlB_PIN);
}

void AP_Tick(void)
{
    if (AP_Timeout > 0)
        if (--AP_Timeout == 0)
            AP_Ctrl_Off();
}
