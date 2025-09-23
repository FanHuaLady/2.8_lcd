#ifndef __TOUCH_H
#define __TOUCH_H
#include <stm32f4xx.h>

#define LCD_WIDTH			320
#define LCD_HEIGHT			240

#define TOUCH_ADDR_WR     	0x70        	// I2Cд��ַ
#define TOUCH_ADDR_RD     	0x71  			// I2C����ַ
#define REG_NUM_FINGER     	0x02        	// �����������Ĵ���
#define REG_TP1_XH         	0x03        	// ��һ��������X�����4λ
#define REG_TP1_XL         	0x04        	// ��һ��������X�����8λ
#define REG_TP1_YH         	0x05        	// ��һ��������Y�����4λ
#define REG_TP1_YL         	0x06        	// ��һ��������Y�����8λ
#define REG_CHIP_ID        	0xA8        	// оƬID�Ĵ���

typedef struct {
    uint8_t is_touch;                  		// 0=�޴�����1=�д���
    uint8_t point_num;                 		// ������������1~5��
    uint16_t x;                        		// ������X���꣨��ָ�����򻯣�
    uint16_t y;                        		// ������Y���꣨��ָ�����򻯣�
} Touch_StateTypeDef;

void Touch_Init(void);
void Touch_Query(void);
uint8_t Touch_ReadReg(uint8_t reg, uint8_t *buff, uint8_t len);

extern Touch_StateTypeDef touch_state;

#endif
