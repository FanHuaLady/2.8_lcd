#ifndef __LCD_H
#define __LCD_H
#include <stm32f4xx.h>

#define LCD_ROTATE      1                           // 屏幕旋转

#if (LCD_ROTATE == 0 || LCD_ROTATE == 1)
#define LCD_WIDTH   240
#define LCD_HEIGHT  320
#else
#define LCD_WIDTH   320
#define LCD_HEIGHT  240
#endif

#define BLOCK_COUNT      4                           // 总区域数
#define BLOCK_HEIGHT     80                          // 每个区域高度（320/4=80）
#define BLOCK_BUF_SIZE   LCD_WIDTH * BLOCK_HEIGHT     // 区域缓冲区大小（240×80）

void LCD_Init(void);								
void LCD_Block_Fill(uint16_t color);
void LCD_Block_Refresh(uint8_t block);

void LCD_Full(uint16_t color);
void LCD_DrawPoint(uint16_t x, uint16_t y, uint16_t color);
void LCD_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);

#endif
