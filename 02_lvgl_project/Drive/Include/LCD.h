#ifndef __LCD_H
#define __LCD_H
#include <stm32f4xx.h>

#define LCD_ROTATE      1                           // 屏幕旋转

#if (LCD_ROTATE == 0 || LCD_ROTATE == 1)
#define LCD_WIDTH   128
#define LCD_HEIGHT  64
#else
#define LCD_WIDTH   320
#define LCD_HEIGHT  240
#endif

#define LCD_BUFF_SIZE   LCD_WIDTH * LCD_HEIGHT     // 缓冲区大小

// 修正后：R 和 G 通道互换（基于实际显示效果）
#define BLACK           0x0000      // 黑色（所有通道0）
#define WHITE           0xFFFF      // 白色（所有通道满）

// 核心修正：交换 R 和 G 的值
#define RED             0x07E0      // 红色（原GREEN的值，对应中间6位全1）
#define GREEN           0x001F      // 绿色（原RED的值，对应低5位全1）
#define BLUE            0xF800      // 蓝色（高5位全1，不变，假设B通道正确）

// 复合色重新计算（红+绿、红+蓝、蓝+绿）
#define YELLOW          (RED | GREEN)  // 黄 = 红 + 绿 → 0x07E0 | 0x001F = 0x07FF
#define MAGENTA         (RED | BLUE)   // 洋红 = 红 + 蓝 → 0x07E0 | 0xF800 = 0xFFE0
#define CYAN            (GREEN | BLUE) // 青 = 绿 + 蓝 → 0x001F | 0xF800 = 0xF81F

// 灰色系列（灰度由三通道均衡组成，重新计算）
#define GRAY_10         0x1863  // 保持不变（若显示正常）
#define GRAY_20         0x3186  
#define GRAY_30         0x4AE9  
#define GRAY_40         0x63CC  
#define GRAY_50         0x7BEF  
#define GRAY_60         0x94F3  
#define GRAY_70         0xAD16  
#define GRAY_80         0xC639  
#define GRAY_90         0xDF5C  

// 红色系列（基于新的RED值调整）
#define LIGHT_RED       0x07FF  // 浅红 = 红 + 少量绿（原YELLOW的值）
#define DARK_RED        0x0400  // 深红（中间6位取部分值，如010000000000）

// 绿色系列（基于新的GREEN值调整）
#define LIGHT_GREEN     0x07FF  // 浅绿 = 绿 + 少量红（与浅红相同，因红绿互换）
#define DARK_GREEN      0x0008  // 深绿（低5位取部分值，如00001000）

// 其他颜色按新的R、G、B重新推导（示例）
#define BROWN           (RED | (GREEN >> 2))  // 棕色 = 红 + 少量绿
#define ORANGE          (RED | (GREEN >> 1))  // 橙色 = 红 + 较多绿

void LCD_Init(void);								

void LCD_Full(uint16_t color);
void LCD_DrawPoint(uint16_t x, uint16_t y, uint16_t color);
void LCD_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);

void LCD_Full_Refresh(void);

#endif
