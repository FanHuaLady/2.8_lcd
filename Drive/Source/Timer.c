#include "stm32f4xx.h"

void Timer_Init(void)
{
    // ʹ��TIM4ʱ�� (TIM4������APB1������)
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    
    // ѡ���ڲ�ʱ��Դ
    TIM_InternalClockConfig(TIM4);
    
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;  // ʱ�ӷ�Ƶ����
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;  // ���ϼ���ģʽ
    
    // ����Ϊ10ms�жϣ�(419+1)*(999+1) / 42MHz = 10ms
    TIM_TimeBaseInitStructure.TIM_Period = 999;            // ARR�Զ���װ��ֵ��������1000�����
    TIM_TimeBaseInitStructure.TIM_Prescaler = 419;          // PSCԤ��Ƶ��ֵ��42MHz / 420 = 100kHz��
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;    // �ظ����������߼���ʱ��ר�ã��˴���0��
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);
    
    // ʹ�ܸ����ж�
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
    
    // ����NVIC�жϿ�����
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  // ���ȼ�����2��2λ��ռ��2λ�����ȼ���
    
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  // TIM4�ж�ͨ��
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  // ʹ���ж�ͨ��
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  // ��ռ���ȼ����ɸ������������
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         // �����ȼ����ɸ������������
    NVIC_Init(&NVIC_InitStructure);
    
    // ������ʱ��
    TIM_Cmd(TIM4, ENABLE);
}
