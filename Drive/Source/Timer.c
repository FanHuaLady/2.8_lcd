#include "stm32f4xx.h"

void Timer_Init(void)
{
    // 使能TIM4时钟 (TIM4挂载在APB1总线上)
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    
    // 选择内部时钟源
    TIM_InternalClockConfig(TIM4);
    
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;  // 时钟分频因子
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;  // 向上计数模式
    
    // 配置为10ms中断：(419+1)*(999+1) / 42MHz = 10ms
    TIM_TimeBaseInitStructure.TIM_Period = 999;            // ARR自动重装载值（计数到1000溢出）
    TIM_TimeBaseInitStructure.TIM_Prescaler = 419;          // PSC预分频器值（42MHz / 420 = 100kHz）
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;    // 重复计数器（高级定时器专用，此处置0）
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);
    
    // 使能更新中断
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
    
    // 配置NVIC中断控制器
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  // 优先级分组2（2位抢占，2位子优先级）
    
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  // TIM4中断通道
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  // 使能中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  // 抢占优先级（可根据需求调整）
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         // 子优先级（可根据需求调整）
    NVIC_Init(&NVIC_InitStructure);
    
    // 启动定时器
    TIM_Cmd(TIM4, ENABLE);
}
