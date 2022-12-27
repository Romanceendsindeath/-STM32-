#include "pwm.h"

void pwm_init(u16 arr,u16 psd){

	GPIO_InitTypeDef GPIO_Initpwm;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitpwm;
	TIM_OCInitTypeDef TIM_OCInitpwm;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);  
	
	GPIO_Initpwm.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_Initpwm.GPIO_Pin=GPIO_Pin_0;
	GPIO_Initpwm.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_Initpwm);
	
	TIM_TimeBaseInitpwm.TIM_Period=arr;
	TIM_TimeBaseInitpwm.TIM_Prescaler=psd;
	TIM_TimeBaseInitpwm.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitpwm.TIM_ClockDivision=0;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitpwm);
	
	TIM_OCInitpwm.TIM_OCMode=TIM_OCMode_PWM2;
	TIM_OCInitpwm.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitpwm.TIM_OCPolarity=TIM_OCPolarity_High;
	TIM_OC1Init(TIM2,&TIM_OCInitpwm);
	
	TIM_OC1PreloadConfig(TIM2,TIM_OCPreload_Enable);
	
	TIM_Cmd(TIM2,ENABLE);
}


