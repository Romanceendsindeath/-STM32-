#include "sys.h"
#include "oled.h"
#include "delay.h"
#include "usart.h"
#include "string.h"	
#include "hc05.h"
#include "pwm.h"
#include "ITR9606.h"

extern long int seep_1,seep_2;
int pwm = 1874;     //����Ϊ1867  ����Ϊ1875��ֹͣ��

int main(void){
	
	uart3_init(9600);
	delay_init(); 						//��ʱ������ʼ��
	NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	ITR9606_init();
	EXTIX_Init();	
	pwm_init(1999,719);   				//PWM ��� 	72000000/2000/720=50HZ = 20ms
    TIM_SetCompare1(TIM2,1750);		// ���ռ�ձ�Ϊ  12.5%=2.5ms
	delay_ms(1000);delay_ms(1000);delay_ms(1000);
	TIM_SetCompare1(TIM2,1900);		// PWMB ��� 	 	//���ռ�ձ�Ϊ  5%=1ms
	delay_ms(1000);delay_ms(1000);delay_ms(1000);delay_ms(1000);delay_ms(1000);
	OLED_Init();
	delay_ms(10);
	OLED_Clear();
	while(1){
		show_seep();
	}
}

