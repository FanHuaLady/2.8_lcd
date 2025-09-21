#ifndef __TIMER_H
#define __TIMER_H

#include "stm32f4xx.h"

// 原有宏定义（保持不变）
#define TIM_FPS_TIMER    TIM3
#define TIM_FPS_IRQn     TIM3_IRQn
#define TIM_FPS_CLK      RCC_APB1Periph_TIM3
#define TIM_FPS_PRESCALER  49999
#define TIM_FPS_ARR        999  // F411用999，F407用839

// 原有计数变量声明（保持不变）
extern volatile uint32_t g_timer_cnt;

// 新增：帧率变量声明（供外部读取）
extern volatile uint32_t g_fps_value;

// 原有接口声明（保持不变）
void Timer_FPS_Init(void);
void Timer_FPS_Inc_Cnt(void);
uint32_t Timer_FPS_Get_Cnt(void);

// 新增：获取帧率值接口（供外部读取，可选）
uint32_t Timer_FPS_Get_FPS(void);

#endif
