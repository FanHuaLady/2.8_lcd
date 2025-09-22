#include "stm32f4xx.h"
#include "delay.h"
#include "LCD.h"

int main()
{
	Delay_Init();
	LCD_Init();
	LCD_Full(0xFFFF);
	LCD_DrawLine(10, 10, 100, 100, BLUE);
	LCD_Full_Refresh();
	
	while(1)
	{
		
	}
}
