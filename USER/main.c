#include "sys.h"
#include "oled.h"
#include "delay.h"
#include "usart.h"
#include "string.h"	
#include "hc05.h"
#include "pwm.h"
#include "ITR9606.h"

extern long int seep_1,seep_2;
int pwm = 1874;     //最大快为1867  最慢为1875（停止）

int main(void){
	
	uart3_init(9600);
	delay_init(); 						//延时函数初始化
	NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	ITR9606_init();
	EXTIX_Init();	
	pwm_init(1999,719);   				//PWM 输出 	72000000/2000/720=50HZ = 20ms
    TIM_SetCompare1(TIM2,1750);		// 输出占空比为  12.5%=2.5ms
	delay_ms(1000);delay_ms(1000);delay_ms(1000);
	TIM_SetCompare1(TIM2,1900);		// PWMB 输出 	 	//输出占空比为  5%=1ms
	delay_ms(1000);delay_ms(1000);delay_ms(1000);delay_ms(1000);delay_ms(1000);
	OLED_Init();
	delay_ms(10);
	OLED_Clear();
	while(1){
		show_seep();
	}
}

