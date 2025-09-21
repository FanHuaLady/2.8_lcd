#include <stm32f4xx.h>
#include "Delay.h"
#include "OLED.h"
#include "LCD.h"
#include "Timer.h"

int main(void)
{
    // 1. ��ʼ������
    OLED_Init();        // ��ʼ��OLED����ʾ֡�ʣ�
    LCD_Init();         // ��ʼ��LCD������ˢ��ģʽ��
    Timer_FPS_Init();   // ��ʼ����ʱ����1���жϼ���֡�ʣ�

    LCD_FullScreen_SetColor(0xFFFF);
    
    while(1)
    {
        LCD_FullScreen_SetColor(0xFFFF);
        LCD_FullScreen_SetColor(0x0000);
    }
}
