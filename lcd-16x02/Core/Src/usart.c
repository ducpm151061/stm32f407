
#include "sys.h"
#include "usart.h"
#include "stdarg.h"

static char *itoa(int value, char *string, int radix);

//////////////////////////////////////////////////////////////////
//�������´���,֧��printf�����������,������Ҫѡ��use MicroLIB
// #if !USE_MICROLIB
// #pragma import(__use_no_semihosting) //����__use_no_semihosting���ţ�ȷ��û�д�C��ʹ�ð������ĺ���
//��׼����Ҫ��֧�ֺ���
struct __FILE
{
	int handle;
};

FILE __stdout;
//����_sys_exit()�Ա���ʹ�ð�����ģʽ
void _sys_exit(int x)
{
	x = x;
}

// #endif

//�ض���fputc����
int fputc(int ch, FILE *f)
{
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
		; //ѭ������,ֱ���������
	USART_SendData(USART1, (u8)ch);
	return ch;
}

u8 USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.

//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA = 0; //����״̬���

/*****************************************************************************
 * @name       :void USART1_Init(u32 bound)
 * @date       :2020-05-08 
 * @function   :Initialize USART1
 * @parameters :bound:baudrate
 * @retvalue   :None
******************************************************************************/
void USART1_Init(u32 bound)
{
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);  //ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); //ʹ��USART1ʱ��

	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);  //GPIOA9����ΪUSART1
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1); //GPIOA10����ΪUSART1

	//USART1�˿�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9��GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;			//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;			//���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;			//����
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//��ʼ��PA9��PA10

	//USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;										//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;								//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//�շ�ģʽ
	USART_Init(USART1, &USART_InitStructure);										//��ʼ������1

	USART_Cmd(USART1, ENABLE);					   //ʹ�ܴ���1
	USART_ClearFlag(USART1, USART_FLAG_TC);		   //�����־λ
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); //��������ж�

	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;		  //����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		  //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  //IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);							  //����ָ���Ĳ�����ʼ��VIC�Ĵ�����
}
/* Method 1 using usart send */
void USART1_Send(const char *str)
{
	while (*str)
	{
		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
			;
		USART_SendData(USART1, *str++);
	}
}
/* Method 2 using itoa */
void USART1_printf(USART_TypeDef *USARTx, char *Data, ...)
{
	const char *s;
	int d;
	char buf[16];

	va_list ap;
	va_start(ap, Data);

	while (*Data != 0) // Determine whether the end of string is reached
	{
		if (*Data == 0x5c) //'\'
		{
			switch (*++Data)
			{
			case 'r': //carriage return
				USART_SendData(USARTx, 0x0d);
				Data++;
				break;

			case 'n': //line break
				USART_SendData(USARTx, 0x0a);
				Data++;
				break;

			default:
				Data++;
				break;
			}
		}

		else if (*Data == '%')
		{ //
			switch (*++Data)
			{
			case 's': //string
				s = va_arg(ap, const char *);

				for (; *s; s++)
				{
					USART_SendData(USARTx, *s);
					while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET)
						;
				}

				Data++;
				break;
			case 'd':
				//Decimal
				d = va_arg(ap, int);
				itoa(d, buf, 10);
				for (s = buf; *s; s++)
				{
					USART_SendData(USARTx, *s);
					while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET)
						;
				}
				Data++;
				break;
			default:
				Data++;
				break;
			}
		}
		else
			USART_SendData(USARTx, *Data++);
		while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET)
			;
	}
}

/*
   * Function name: itoa
   * Description: Convert integer data to character string
   * Input: -radix =10 means decimal, other results are 0
   * -value Integer to be converted
   * -buf converted string
 *         -radix = 10
   * Output: None
   * Return: None
   * Call: Called by USART1_printf()
 */
static char *itoa(int value, char *string, int radix)
{
	int i, d;
	int flag = 0;
	char *ptr = string;

	/* This implementation only works for decimal numbers. */
	if (radix != 10)
	{
		*ptr = 0;
		return string;
	}

	if (!value)
	{
		*ptr++ = 0x30;
		*ptr = 0;
		return string;
	}

	/* if this is a negative value insert the minus sign. */
	if (value < 0)
	{
		*ptr++ = '-';

		/* Make the value positive. */
		value *= -1;
	}

	for (i = 10000; i > 0; i /= 10)
	{
		d = value / i;

		if (d || flag)
		{
			*ptr++ = (char)(d + 0x30);
			value -= (d * i);
			flag = 1;
		}
	}

	/* Null terminate the string. */
	*ptr = 0;

	return string;
}

/*****************************************************************************
 * @name       :void USART1_Init(u32 bound)
 * @date       :2020-05-08 
 * @function   :USART1 interrupt service program
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void USART1_IRQHandler(void)
{
	u8 Res;
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		Res = USART_ReceiveData(USART1);  //��ȡ���յ�������
		if ((USART_RX_STA & 0x8000) == 0) //����δ���
		{
			if (USART_RX_STA & 0x4000) //���յ���0x0d
			{
				if (Res != 0x0a)
				{
					USART_RX_STA = 0; //���մ���,���¿�ʼ
				}
				else
				{
					USART_RX_STA |= 0x8000; //���������
				}
			}
			else //��û�յ�0X0D
			{
				if (Res == 0x0d)
				{
					USART_RX_STA |= 0x4000;
				}
				else
				{
					USART_RX_BUF[USART_RX_STA & 0X3FFF] = Res;
					USART_RX_STA++;
					if (USART_RX_STA > (USART_REC_LEN - 1))
					{
						USART_RX_STA = 0; //�������ݴ���,���¿�ʼ����
					}
				}
			}
		}
	}
}
