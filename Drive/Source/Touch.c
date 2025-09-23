#include "Touch.h"
#include "Delay.h"

Touch_StateTypeDef touch_state;

uint8_t touch_rotation = 3;

// 硬件I2C初始化
void Touch_I2C_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    I2C_InitTypeDef I2C_InitStructure;
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);							// 使能时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;						// 配置I2C引脚 PB10-SCL, PB11-SDA
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;           							// 复用功能
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;         							// 开漏输出
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;           							// 上拉
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_I2C2);						// 引脚复用映射
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_I2C2);
    
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;										// 配置I2C
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = 0x00;              							// 主机地址，可任意设置
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed = 400000;             							// 400kHz标准模式
    
    I2C_Init(I2C2, &I2C_InitStructure);
    I2C_Cmd(I2C2, ENABLE);
}

// RESET引脚初始化
void Touch_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;										// RESET引脚 PB12
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

// 控制复位引脚
void Touch_W_RES(uint8_t BitValue)
{
    GPIO_WriteBit(GPIOB, GPIO_Pin_12, (BitAction)BitValue);
}

// 检查I2C事件
uint8_t Touch_I2C_CheckEvent(uint32_t event)
{
    uint32_t timeout = 100000;
    while (!I2C_CheckEvent(I2C2, event))
    {
        if (timeout-- == 0)
            return 1; 																// 超时
    }
    return 0; 																		// 成功
}

// 读寄存器
uint8_t Touch_ReadReg(uint8_t reg, uint8_t *buff, uint8_t len) 
{
    uint32_t timeout;
    
    I2C_GenerateSTART(I2C2, ENABLE);												// 发送起始信号
    if (Touch_I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT)) 
        return 1;
    
    I2C_Send7bitAddress(I2C2, TOUCH_ADDR_WR, I2C_Direction_Transmitter);			// 发送设备地址（写模式）
    if (Touch_I2C_CheckEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) 
        return 2;
    
    I2C_SendData(I2C2, reg);														// 发送寄存器地址
    if (Touch_I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED)) 
        return 3;
    
    I2C_GenerateSTART(I2C2, ENABLE);												// 发送起始信号
    if (Touch_I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT)) 
        return 4;
    
    I2C_Send7bitAddress(I2C2, TOUCH_ADDR_RD, I2C_Direction_Receiver);				// 发送设备地址（读模式）
    if (Touch_I2C_CheckEvent(I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)) 
        return 5;
    
    for (uint8_t i = 0; i < len; i++)												// 读取数据
    {
        if (i == len - 1) 															// 最后一个字节
        {
            I2C_AcknowledgeConfig(I2C2, DISABLE);									// 发送非应答，然后停止
            I2C_GenerateSTOP(I2C2, ENABLE);
        }
        
        timeout = 100000;															// 等待数据接收完成
        while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED))
        {
            if (timeout-- == 0)
            {
                I2C_AcknowledgeConfig(I2C2, ENABLE);
                return 6;
            }
        }
        
        buff[i] = I2C_ReceiveData(I2C2);
    }
    
    I2C_AcknowledgeConfig(I2C2, ENABLE);											// 重新使能应答
    
    return 0;
}

void Touch_Init(void)
{
    Touch_I2C_Init();
    Touch_GPIO_Init();
    
    Touch_W_RES(0);
    Delay_ms(50);
    Touch_W_RES(1);
    Delay_ms(300);
    
    touch_state.is_touch = 0;
    touch_state.point_num = 0;
    touch_state.x = 0;
    touch_state.y = 0;
}

void Touch_TransformCoordinates(uint16_t *x, uint16_t *y)
{
    uint16_t temp;
    
    switch(touch_rotation)
    {
        case 0:
            // 0度旋转，坐标不变
            break;
            
        case 1:
            // 90度旋转：x变为原y，y变为(LCD_WIDTH - 原x - 1)
            temp = *x;
            *x = *y;
            *y = LCD_WIDTH - temp - 1;
            break;
            
        case 2:
            // 180度旋转：x变为(LCD_WIDTH - 原x - 1)，y变为(LCD_HEIGHT - 原y - 1)
            *x = LCD_WIDTH - *x - 1;
            *y = LCD_HEIGHT - *y - 1;
            break;
            
        case 3:
            // 270度旋转：x变为(LCD_HEIGHT - 原y - 1)，y变为原x
            temp = *x;
            *x = LCD_HEIGHT - *y - 1;
            *y = temp;
            break;
    }
}

// 应用层函数保持不变
void Touch_Query(void) 
{
    uint8_t buf[4];
    uint8_t point_num;
    uint16_t x, y;
    
    if (Touch_ReadReg(REG_NUM_FINGER, &point_num, 1) != 0) 
    {
        touch_state.is_touch = 0;
        return;
    }
    
    point_num &= 0x0F;
    if (point_num == 0) 
    {
        touch_state.is_touch = 0;
        touch_state.point_num = 0;
        return;
    }
    
    if (Touch_ReadReg(REG_TP1_XH, buf, 4) != 0) 
    {
        touch_state.is_touch = 0;
        return;
    }
    
    // 先读取原始坐标
    x = ((buf[0] & 0x0F) << 8) | buf[1];
    y = ((buf[2] & 0x0F) << 8) | buf[3];
    
    // 根据旋转方向转换坐标
    Touch_TransformCoordinates(&x, &y);
    
	x += 60;
	y -= 20;
	
    // 根据旋转后的屏幕分辨率进行边界检查
    if (touch_rotation == 1 || touch_rotation == 3)
    {
        // 旋转90或270度后，宽高互换
        if (x >= LCD_HEIGHT) x = LCD_HEIGHT - 1;
        if (y >= LCD_WIDTH) y = LCD_WIDTH - 1;
    }
    else
    {
        // 0或180度旋转，宽高不变
        if (x >= LCD_WIDTH) x = LCD_WIDTH - 1;
        if (y >= LCD_HEIGHT) y = LCD_HEIGHT - 1;
    }
    
    // 更新触摸状态
    touch_state.x = x;
    touch_state.y = y;
    touch_state.is_touch = 1;
    touch_state.point_num = point_num;
}
