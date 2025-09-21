#ifndef __LCD_H
#define __LCD_H
#include <stm32f4xx.h>

// 屏幕旋转
#define LCD_ROTATE      2

#if (LCD_ROTATE == 0 || LCD_ROTATE == 1)
#define LCD_WIDTH   240
#define LCD_HEIGHT  320
#else
#define LCD_WIDTH   320
#define LCD_HEIGHT  240
#endif

void LCD_Init(void);
void LCD_Color_Fill(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, uint16_t color);

#endif
