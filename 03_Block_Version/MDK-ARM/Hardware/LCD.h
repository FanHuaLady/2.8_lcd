#ifndef __LCD_H
#define __LCD_H
#include <stm32f4xx.h>

#define LCD_ROTATE      0                           // 屏幕旋转

#if (LCD_ROTATE == 0 || LCD_ROTATE == 1)
#define LCD_WIDTH   240
#define LCD_HEIGHT  320
#else
#define LCD_WIDTH   320
#define LCD_HEIGHT  240
#endif

#define AREA_COUNT      4                           // 总区域数
#define AREA_HEIGHT     80                          // 每个区域高度（320/4=80）
#define AREA_BUF_SIZE   LCD_WIDTH * AREA_HEIGHT     // 区域缓冲区大小（240×80）

void LCD_Init(void);
void LCD_AreaBuf_Fill(uint16_t color);
void LCD_Area_Refresh(uint8_t area);
void LCD_FullScreen_SetColor(uint16_t color);

#endif
