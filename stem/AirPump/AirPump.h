#ifndef __AIRPUMP_H
#define __AIRPUMP_H

#include <stdint.h>

#define AP_CtrlA_PIN GPIO_Pin_11
#define AP_CtrlB_PIN GPIO_Pin_12
#define AP_Ctrl_GPIO GPIOC
#define AP_Ctrl_GPIOClk RCC_AHB1Periph_GPIOC

void AP_Ctrl_Off(void);
void AP_Ctrl_Pull(uint16_t Timeout);
void AP_Ctrl_Push(uint16_t Timeout);
void AP_Init(void);
void AP_Tick(void);

#endif
