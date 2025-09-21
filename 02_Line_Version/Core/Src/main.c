#include <stm32f4xx.h>
#include "Delay.h"
#include "OLED.h"
#include "LCD.h"
#include "Timer.h"

int main(void)
{
    // 1. 初始化顺序：OLED → LCD → 定时器（顺序无严格要求，确保都初始化即可）
    OLED_Init();        // 初始化OLED（用于显示帧率）
    LCD_Init();         // 初始化LCD（行缓冲驱动已验证）
    Timer_FPS_Init();   // 初始化定时器（1秒中断，用于帧率计算）

    // 2. 初始清屏
    OLED_Clear();       // OLED清屏（避免残留乱码）
    LCD_FullScreen_Fill(0xFFFF);  // LCD填充白色（首次刷新，触发计数+1）

    // 3. 主循环：持续刷新LCD，触发帧率计数（模拟实际使用场景）
    while(1)
    {
        // 示例1：交替刷新LCD为白色（0xFFFF）和灰色（0xAAAA），模拟动态画面
        LCD_FullScreen_Fill(0xFFFF);  // 刷新白色 → 计数+1
        // Delay_ms(10);  // 可加延迟降低刷新速度（测试FPS变化，如加10ms→~50FPS）

        LCD_FullScreen_Fill(0xAAAA);  // 刷新灰色 → 计数+1
        // Delay_ms(10);
        // 示例2：若想测试最大FPS，去掉延迟，直接循环刷新
        // LCD_FullScreen_Fill(0xFFFF);
        // LCD_FullScreen_Fill(0xAAAA);

        // 【关键】无需在主循环处理帧率！所有计算和显示都在定时器中断中自动完成
    }
}
