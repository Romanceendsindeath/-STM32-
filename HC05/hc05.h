#ifndef __HC05_H
#define __HC05_H

#include "sys.h" 

void USART3_Send_String(char *String);
void uart3_init(u32 bound);					//串口3初始化函数
void USART3_IRQHandler(void);     	//串口3中断服务程序

#endif

