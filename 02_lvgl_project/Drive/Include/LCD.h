#ifndef __LCD_H
#define __LCD_H
#include <stm32f4xx.h>

#define LCD_ROTATE      1                           // 屏幕旋转

#if (LCD_ROTATE == 0 || LCD_ROTATE == 1)
#define LCD_WIDTH   240
#define LCD_HEIGHT  240
#else
#define LCD_WIDTH   320
#define LCD_HEIGHT  240
#endif

#define LCD_BUFF_SIZE   LCD_WIDTH * LCD_HEIGHT     // 缓冲区大小

// 基本颜色（BGR565格式，适配通道错位问题）
#define BLACK           0x0000      // 黑色
#define WHITE           0xFFFF      // 白色
#define RED             0x001F      // 红色（原BLUE的值，因R和B通道互换）
#define GREEN           0x07E0      // 绿色（G通道正常，无需修改）
#define BLUE            0xF800      // 蓝色（原RED的值，因R和B通道互换）
#define CYAN            0xF81F      // 青色 (蓝+绿) = 原MAGENTA
#define MAGENTA         0x07FF      // 洋红色 (红+蓝) = 原CYAN
#define YELLOW          0xFFE0      // 黄色 (红+绿)（无需修改）

// 灰色系列（灰度值无RGB偏差，无需修改）
#define GRAY_10         0x1863      
#define GRAY_20         0x3186      
#define GRAY_30         0x4AE9      
#define GRAY_40         0x63CC      
#define GRAY_50         0x7BEF      
#define GRAY_60         0x94F3      
#define GRAY_70         0xAD16      
#define GRAY_80         0xC639      
#define GRAY_90         0xDF5C     

// 红色系列
#define LIGHT_RED       0x05FF      // 浅红色（原LIGHT_BLUE）
#define DARK_RED        0x0008      // 深红色（原DARK_BLUE）

// 蓝色系列
#define LIGHT_BLUE      0xFD20      // 浅蓝色（原LIGHT_RED）
#define DARK_BLUE       0x8000      // 深蓝色（原DARK_RED）

// 其他常用颜色（按BGR顺序调整）
#define BROWN           0x45A1      // 棕色（原A145反转高低字节）
#define ORANGE          0x05FF      // 橙色（原FD20对应BGR的浅红）
#define PINK            0x18FC      // 粉红色（原FC18反转高低字节）
#define PURPLE          0x1080      // 紫色（原8010反转高低字节）
#define TEAL            0x1004      // 蓝绿色（原0410反转高低字节）
#define NAVY            0x0F00      // 藏青色（原000F反转高低字节）
#define MAROON          0x0080      // 褐红色（原8000对应BGR的深红）
#define OLIVE           0x0084      // 橄榄绿（原8400反转高低字节）
#define LIME            0x07E0      // 酸橙绿（无需修改）
#define AQUA            0xF81F      // 水绿色（原07FF对应BGR的青色）
#define FUCHSIA         0x07FF      // 紫红色（原F81F对应BGR的洋红）
#define SILVER          0x18C6      // 银色（原C618反转高低字节）

void LCD_Init(void);								

void LCD_Full(uint16_t color);
void LCD_DrawPoint(uint16_t x, uint16_t y, uint16_t color);
void LCD_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);

void LCD_Full_Refresh(void);

#endif
