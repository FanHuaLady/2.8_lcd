#include <stm32f4xx.h>
#include "Delay.h"
#include "OLED.h"
#include "LCD.h"
#include "Timer.h"

int main(void)
{
    // 1. ��ʼ��˳��OLED �� LCD �� ��ʱ����˳�����ϸ�Ҫ��ȷ������ʼ�����ɣ�
    OLED_Init();        // ��ʼ��OLED��������ʾ֡�ʣ�
    LCD_Init();         // ��ʼ��LCD���л�����������֤��
    Timer_FPS_Init();   // ��ʼ����ʱ����1���жϣ�����֡�ʼ��㣩

    // 2. ��ʼ����
    OLED_Clear();       // OLED����������������룩
    LCD_FullScreen_Fill(0xFFFF);  // LCD����ɫ���״�ˢ�£���������+1��

    // 3. ��ѭ��������ˢ��LCD������֡�ʼ�����ģ��ʵ��ʹ�ó�����
    while(1)
    {
        // ʾ��1������ˢ��LCDΪ��ɫ��0xFFFF���ͻ�ɫ��0xAAAA����ģ�⶯̬����
        LCD_FullScreen_Fill(0xFFFF);  // ˢ�°�ɫ �� ����+1
        // Delay_ms(10);  // �ɼ��ӳٽ���ˢ���ٶȣ�����FPS�仯�����10ms��~50FPS��

        LCD_FullScreen_Fill(0xAAAA);  // ˢ�»�ɫ �� ����+1
        // Delay_ms(10);
        // ʾ��2������������FPS��ȥ���ӳ٣�ֱ��ѭ��ˢ��
        // LCD_FullScreen_Fill(0xFFFF);
        // LCD_FullScreen_Fill(0xAAAA);

        // ���ؼ�����������ѭ������֡�ʣ����м������ʾ���ڶ�ʱ���ж����Զ����
    }
}
