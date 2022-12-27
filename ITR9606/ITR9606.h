#ifndef __ITR9606_H
#define __ITR9606_H

#include "sys.h"

#define tcrt_1 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_10)
#define tcrt_2 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11)

void ITR9606_init(void);
void EXTIX_Init(void);
void EXTI15_10_IRQHandler(void);

#endif

