#include <stm32f4xx.h>
#include "Delay.h"
#include "OLED.h"
#include "LCD.h"

int main()
{
	LCD_Init();
	LCD_Color_Fill(0,0,320,240,0xFFFF);
	
	while(1)
	{

	}
}
