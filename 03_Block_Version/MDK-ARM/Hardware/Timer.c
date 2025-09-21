#include "Timer.h"
#include "stm32f4xx.h"
#include "OLED.h"  // ����������OLEDͷ�ļ���������ʾ֡��

// ȫ�ּ������������ֲ��䣩
volatile uint32_t g_timer_cnt = 0;

// ������֡�ʱ������洢�������FPSֵ�����ⲿ��ȡ����ѡ��
volatile uint32_t g_fps_value = 0;

// ��ʱ����ʼ�������ֲ��䣬�����޸ģ�
void Timer_FPS_Init(void)
{
    // ԭ�д��루����֤��ȷ��ֱ�ӱ�����
    RCC_APB1PeriphClockCmd(TIM_FPS_CLK, ENABLE);
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
    TIM_TimeBaseStruct.TIM_Prescaler = 9400 - 1;
    TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStruct.TIM_Period = 10000 - 1;
    TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM_FPS_TIMER, &TIM_TimeBaseStruct);
    TIM_ITConfig(TIM_FPS_TIMER, TIM_IT_Update, ENABLE);
    
    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = TIM_FPS_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
    
    TIM_Cmd(TIM_FPS_TIMER, ENABLE);
}

// ���������ӿڣ����ֲ��䣩
void Timer_FPS_Inc_Cnt(void)
{
    g_timer_cnt++;
}

// ��ȡ�����ӿڣ����ֲ��䣩
uint32_t Timer_FPS_Get_Cnt(void)
{
    return g_timer_cnt;
}

// ��������ȡ֡��ֵ�ӿڣ����ⲿ��ȡ����ѡ��
uint32_t Timer_FPS_Get_FPS(void)
{
    return g_fps_value;
}

// ��ʱ���жϷ��������޸ĺ����߼�����
void TIM3_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM_FPS_TIMER, TIM_IT_Update) == SET)
    {
        // 1. ����֡�ʣ�1���ڵ�LCDˢ�´��� = g_timer_cnt
        g_fps_value = g_timer_cnt;

        // 2. ��OLED����ʾ֡�ʣ��̶���ʽ��"FPS: XX"��
        OLED_ShowString(0, 0, "FPS: ", OLED_8X16);  // ��ʾǰ׺��X=0,Y=0��8x16���壩
        OLED_ShowNum(40, 0, g_fps_value, 2, OLED_8X16);  // ��ʾ֡����ֵ��2λ��0~99��
        // 3. ������OLED��֡����ʾ���򣨼�����˸������Ч�ʣ�
        // ����X=0~63��Y=0~15������"FPS: XX"�ķ�Χ��
        OLED_Update();

        // 4. ���ü�����Ϊ��һ���֡��ͳ����׼��
        g_timer_cnt = 0;

        // 5. ����жϱ�־λ�����뱣�����������޴����жϣ�
        TIM_ClearITPendingBit(TIM_FPS_TIMER, TIM_IT_Update);
    }
}
