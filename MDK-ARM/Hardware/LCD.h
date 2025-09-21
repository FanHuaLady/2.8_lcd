#ifndef __LCD_H
#define __LCD_H
#include <stm32f4xx.h>

#define LCD_ROTATE      1

void LCD_Init(void);
void LCD_Color_Fill(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, uint16_t color);

#endif
