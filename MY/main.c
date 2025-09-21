#include "stm32f4xx.h"
#include "delay.h"
#include "LCD.h"

int main()
{
	Delay_Init();
	LCD_Init();
	
	while(1)
	{
		LCD_Full(0xFFFF);
		LCD_Full(0x0000);
	}
}
