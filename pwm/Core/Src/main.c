#include "delay.h"
#include "sys.h"
#include "led.h"
#include "pwm.h"

int main(void)
{
	u8 dir=1;
	u16 PWM_Value=0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);     //��ʼ����ʱ����
	LED_Init();          //��ʼ��LED��GPIO
	//tout=((499+1)*(83+1))/84(us)=500us,PWMƵ��Ϊ2KHz
	TIM5_PWM_Init(499,83);	//��ʱ��5 PWM��ʼ����ʱ��84M����Ƶϵ��84������84M/84=1Mhz�ļ���Ƶ�ʣ�����500��Ϊ500us
	LED1=0;              //����LED1
	while(1)
	{
		delay_ms(10);	 
		if(dir)
		{
			PWM_Value++;   //dir==1 led0pwmval����
		}
		else 
		{
			PWM_Value--;	//dir==0 led0pwmval�ݼ� 
 		}
		if(PWM_Value>300)
		{
			dir=0;        //led0pwmval����300�󣬷���Ϊ�ݼ�
		}
		if(PWM_Value==0)
		{
			dir=1;	      //led0pwmval�ݼ���0�󣬷����Ϊ����
		}
		TIM_SetCompare2(TIM5,PWM_Value);	//�޸ıȽ�ֵ���޸�ռ�ձ�
	}
}

