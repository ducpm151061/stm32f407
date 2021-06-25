
#ifndef __LED_H
#define __LED_H
#include "sys.h"	

//LED�˿ڶ���
#define LED0 PAout(1)	// LED0
#define LED1 PCout(5)	// LED1

void LED_Init(void);  //��ʼ��GPIO		 				    
#endif
