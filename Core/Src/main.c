#include <stm32f4xx.h>
#include "Delay.h"
#include "OLED.h"
#include "LCD.h"

int main()
{
	LCD_Init();
	LCD_Color_Fill(0,0,100,100,0xFFFF);
}
