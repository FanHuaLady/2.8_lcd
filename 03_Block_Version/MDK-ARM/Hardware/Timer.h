#ifndef __TIMER_H
#define __TIMER_H

#include "stm32f4xx.h"

// ԭ�к궨�壨���ֲ��䣩
#define TIM_FPS_TIMER    TIM3
#define TIM_FPS_IRQn     TIM3_IRQn
#define TIM_FPS_CLK      RCC_APB1Periph_TIM3
#define TIM_FPS_PRESCALER  49999
#define TIM_FPS_ARR        999  // F411��999��F407��839

// ԭ�м����������������ֲ��䣩
extern volatile uint32_t g_timer_cnt;

// ������֡�ʱ������������ⲿ��ȡ��
extern volatile uint32_t g_fps_value;

// ԭ�нӿ����������ֲ��䣩
void Timer_FPS_Init(void);
void Timer_FPS_Inc_Cnt(void);
uint32_t Timer_FPS_Get_Cnt(void);

// ��������ȡ֡��ֵ�ӿڣ����ⲿ��ȡ����ѡ��
uint32_t Timer_FPS_Get_FPS(void);

#endif
