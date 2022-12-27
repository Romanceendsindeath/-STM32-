#include "ITR9606.h"
#include "oled.h"

long int seep_1 = 0,seep_2 = 0;
float seep=0;
extern int pwm;

void ITR9606_init(void){//左边PA10  右边PA11  TIM_1  遮挡产生高电平

	GPIO_InitTypeDef GPIO_InitITR9606;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟 TIM3 使能
	
	GPIO_InitITR9606.GPIO_Mode=GPIO_Mode_IN_FLOATING;   //浮空输入（无法确认状态则用这个）
	GPIO_InitITR9606.GPIO_Pin=GPIO_Pin_10|GPIO_Pin_11;  //PA10  PA11
	GPIO_InitITR9606.GPIO_Speed=GPIO_Speed_50MHz;     // 传输速度
	GPIO_Init(GPIOA,&GPIO_InitITR9606);
	
	//定时器 TIM3 初始化
	TIM_TimeBaseStructure.TIM_Period = 19999; //设置自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =719; //设置时钟频率除数的预分频值   72000000/20000/720=50HZ = 200ms
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM 向上计数
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //初始化 TIM3
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //允许更新中断
	
	//中断优先级 NVIC 设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; //TIM3 中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //先占优先级 0 级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; //从优先级 3 级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ 通道被使能
	NVIC_Init(&NVIC_InitStructure); //初始化 NVIC 寄存器
	
	TIM_Cmd(TIM3, ENABLE); //使能 TIM3

}

void EXTIX_Init(void)  // 外部中断配置
{
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	
	ITR9606_init();
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource10);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource11);
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line10|EXTI_Line11;  //中断线10  11
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;     //中断模式为中断模式
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;    //下降沿触发
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置中断优先级分组2
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;  //线10  11中断配置
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //主分组
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;    //子分组
	NVIC_Init(&NVIC_InitStructure);

}

void EXTI15_10_IRQHandler(void){
if(EXTI_GetITStatus(EXTI_Line10)!= RESET) //判断是否发生中断，发生则中断标志置1
	{
		seep_1++;
	}
	EXTI_ClearITPendingBit(EXTI_Line10); //清除 LINE 上的中断标志位
	
	if(EXTI_GetITStatus(EXTI_Line11)!=RESET)//判断线10上的中断是否发生 
	{
		seep_2++;
	}
	EXTI_ClearITPendingBit(EXTI_Line11); //清除 LINE 上的中断标志位
}

void TIM3_IRQHandler(void) //TIM3 中断
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //检查 TIM3 更新中断发生与否
	{
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update ); //清除 TIM3 更新中断标志
		// 速度计算  两个光电门取平均值 除以 20目 ×周长 除以 时间
		seep = (seep_1+seep_2)/40*0.075/0.2;
		show_seep();
		OLED_ShowNum(73,0,seep,4,16);
		OLED_ShowNum(73,5,1875-pwm,4,16);
		seep_1=0;
		seep_2=0;
	}
}


