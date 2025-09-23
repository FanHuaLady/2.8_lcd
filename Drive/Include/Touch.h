#ifndef __TOUCH_H
#define __TOUCH_H
#include <stm32f4xx.h>

#define LCD_WIDTH			320
#define LCD_HEIGHT			240

#define TOUCH_ADDR_WR     	0x70        	// I2C写地址
#define TOUCH_ADDR_RD     	0x71  			// I2C读地址
#define REG_NUM_FINGER     	0x02        	// 触摸点数量寄存器
#define REG_TP1_XH         	0x03        	// 第一个触摸点X坐标高4位
#define REG_TP1_XL         	0x04        	// 第一个触摸点X坐标低8位
#define REG_TP1_YH         	0x05        	// 第一个触摸点Y坐标高4位
#define REG_TP1_YL         	0x06        	// 第一个触摸点Y坐标低8位
#define REG_CHIP_ID        	0xA8        	// 芯片ID寄存器

typedef struct {
    uint8_t is_touch;                  		// 0=无触摸，1=有触摸
    uint8_t point_num;                 		// 触摸点数量（1~5）
    uint16_t x;                        		// 触摸点X坐标（单指触摸简化）
    uint16_t y;                        		// 触摸点Y坐标（单指触摸简化）
} Touch_StateTypeDef;

void Touch_Init(void);
void Touch_Query(void);
uint8_t Touch_ReadReg(uint8_t reg, uint8_t *buff, uint8_t len);

extern Touch_StateTypeDef touch_state;

#endif
