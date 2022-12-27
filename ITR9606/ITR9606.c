#include "ITR9606.h"
#include "oled.h"

long int seep_1 = 0,seep_2 = 0;
float seep=0;
extern int pwm;

void ITR9606_init(void){//���PA10  �ұ�PA11  TIM_1  �ڵ������ߵ�ƽ

	GPIO_InitTypeDef GPIO_InitITR9606;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ�� TIM3 ʹ��
	
	GPIO_InitITR9606.GPIO_Mode=GPIO_Mode_IN_FLOATING;   //�������루�޷�ȷ��״̬���������
	GPIO_InitITR9606.GPIO_Pin=GPIO_Pin_10|GPIO_Pin_11;  //PA10  PA11
	GPIO_InitITR9606.GPIO_Speed=GPIO_Speed_50MHz;     // �����ٶ�
	GPIO_Init(GPIOA,&GPIO_InitITR9606);
	
	//��ʱ�� TIM3 ��ʼ��
	TIM_TimeBaseStructure.TIM_Period = 19999; //�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =719; //����ʱ��Ƶ�ʳ�����Ԥ��Ƶֵ   72000000/20000/720=50HZ = 200ms
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM ���ϼ���
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //��ʼ�� TIM3
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //��������ж�
	
	//�ж����ȼ� NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; //TIM3 �ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //��ռ���ȼ� 0 ��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; //�����ȼ� 3 ��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ ͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure); //��ʼ�� NVIC �Ĵ���
	
	TIM_Cmd(TIM3, ENABLE); //ʹ�� TIM3

}

void EXTIX_Init(void)  // �ⲿ�ж�����
{
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	
	ITR9606_init();
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource10);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource11);
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line10|EXTI_Line11;  //�ж���10  11
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;     //�ж�ģʽΪ�ж�ģʽ
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;    //�½��ش���
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//�����ж����ȼ�����2
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;  //��10  11�ж�����
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //������
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;    //�ӷ���
	NVIC_Init(&NVIC_InitStructure);

}

void EXTI15_10_IRQHandler(void){
if(EXTI_GetITStatus(EXTI_Line10)!= RESET) //�ж��Ƿ����жϣ��������жϱ�־��1
	{
		seep_1++;
	}
	EXTI_ClearITPendingBit(EXTI_Line10); //��� LINE �ϵ��жϱ�־λ
	
	if(EXTI_GetITStatus(EXTI_Line11)!=RESET)//�ж���10�ϵ��ж��Ƿ��� 
	{
		seep_2++;
	}
	EXTI_ClearITPendingBit(EXTI_Line11); //��� LINE �ϵ��жϱ�־λ
}

void TIM3_IRQHandler(void) //TIM3 �ж�
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //��� TIM3 �����жϷ������
	{
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update ); //��� TIM3 �����жϱ�־
		// �ٶȼ���  ���������ȡƽ��ֵ ���� 20Ŀ ���ܳ� ���� ʱ��
		seep = (seep_1+seep_2)/40*0.075/0.2;
		show_seep();
		OLED_ShowNum(73,0,seep,4,16);
		OLED_ShowNum(73,5,1875-pwm,4,16);
		seep_1=0;
		seep_2=0;
	}
}


