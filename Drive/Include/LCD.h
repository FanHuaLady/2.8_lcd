#ifndef __LCD_H
#define __LCD_H
#include <stm32f4xx.h>

#define LCD_ROTATE      3                           // 屏幕旋转

#if (LCD_ROTATE == 0 || LCD_ROTATE == 1)
#define LCD_WIDTH   240
#define LCD_HEIGHT  320
#else
#define LCD_WIDTH   320
#define LCD_HEIGHT  240
#endif

void LCD_W_DC(uint8_t BitValue);
void LCD_W_CS(uint8_t BitValue);

void LCD_Init(void);								
void LCD_DMA_Init(uint32_t addr);
void LCD_Address_Set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);


void LCD_Full(uint16_t color);                     // 全屏填充（直接发送，无缓冲区）
void LCD_DrawPoint(uint16_t x, uint16_t y, uint16_t color); // 画点（直接发送，无缓冲区）

#endif
