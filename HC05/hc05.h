#ifndef __HC05_H
#define __HC05_H

#include "sys.h" 

void USART3_Send_String(char *String);
void uart3_init(u32 bound);					//����3��ʼ������
void USART3_IRQHandler(void);     	//����3�жϷ������

#endif

