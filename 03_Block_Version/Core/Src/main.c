#include <stm32f4xx.h>
#include "Delay.h"
#include "OLED.h"
#include "LCD.h"
#include "Timer.h"

int main(void)
{
    // 1. 初始化外设
    OLED_Init();        // 初始化OLED（显示帧率）
    LCD_Init();         // 初始化LCD（分区刷新模式）
    Timer_FPS_Init();   // 初始化定时器（1秒中断计算帧率）

    LCD_FullScreen_SetColor(0xFFFF);
    
    while(1)
    {
        LCD_FullScreen_SetColor(0xFFFF);
        LCD_FullScreen_SetColor(0x0000);
    }
}
