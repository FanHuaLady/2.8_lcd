#include "Timer.h"
#include "stm32f4xx.h"
#include "OLED.h"  // 新增：包含OLED头文件，用于显示帧率

// 全局计数变量（保持不变）
volatile uint32_t g_timer_cnt = 0;

// 新增：帧率变量（存储计算出的FPS值，供外部读取，可选）
volatile uint32_t g_fps_value = 0;

// 定时器初始化（保持不变，无需修改）
void Timer_FPS_Init(void)
{
    // 原有代码（已验证正确，直接保留）
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

// 计数自增接口（保持不变）
void Timer_FPS_Inc_Cnt(void)
{
    g_timer_cnt++;
}

// 获取计数接口（保持不变）
uint32_t Timer_FPS_Get_Cnt(void)
{
    return g_timer_cnt;
}

// 新增：获取帧率值接口（供外部读取，可选）
uint32_t Timer_FPS_Get_FPS(void)
{
    return g_fps_value;
}

// 定时器中断服务函数（修改核心逻辑！）
void TIM3_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM_FPS_TIMER, TIM_IT_Update) == SET)
    {
        // 1. 计算帧率：1秒内的LCD刷新次数 = g_timer_cnt
        g_fps_value = g_timer_cnt;

        // 2. 在OLED上显示帧率（固定格式："FPS: XX"）
        OLED_ShowString(0, 0, "FPS: ", OLED_8X16);  // 显示前缀（X=0,Y=0，8x16字体）
        OLED_ShowNum(40, 0, g_fps_value, 2, OLED_8X16);  // 显示帧率数值（2位，0~99）
        // 3. 仅更新OLED的帧率显示区域（减少闪烁，提升效率）
        // 区域：X=0~63，Y=0~15（覆盖"FPS: XX"的范围）
        OLED_Update();

        // 4. 重置计数：为下一秒的帧率统计做准备
        g_timer_cnt = 0;

        // 5. 清除中断标志位（必须保留，否则无限触发中断）
        TIM_ClearITPendingBit(TIM_FPS_TIMER, TIM_IT_Update);
    }
}
