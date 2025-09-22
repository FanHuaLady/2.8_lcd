#include "LCD.h"
#include "delay.h"
#include <stdlib.h>

// --------------------------------------------------------------------// 驱动层
// 背光
void LCD_W_BLK(uint8_t BitValue)
{
    GPIO_WriteBit(GPIOA, GPIO_Pin_1, (BitAction)BitValue);
}

// 指令/数据
void LCD_W_DC(uint8_t BitValue)
{
    GPIO_WriteBit(GPIOA, GPIO_Pin_2, (BitAction)BitValue);
}

// 复位
void LCD_W_RES(uint8_t BitValue)
{
    GPIO_WriteBit(GPIOA, GPIO_Pin_3, (BitAction)BitValue);
}

// 片选
void LCD_W_CS(uint8_t BitValue)
{
    GPIO_WriteBit(GPIOA, GPIO_Pin_4, (BitAction)BitValue);
}

void LCD_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		                // 普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		                // 推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	                // 100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		                // 上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);				                // 初始化
	GPIO_SetBits(GPIOA,GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4);
}

void LCD_SPI_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_7;				// 5:SPI_SCK 7:SPI_MOSI
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;						// 复用输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;						// 推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;					// 100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);								// 初始化
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);				// 引脚复用配置
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);				// 引脚复用配置
	
	SPI_InitTypeDef SPI_InitStructure;									// 定义结构体变量
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;						// 主模式
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;	// 全双工
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;							// 极性低
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;						// 第一边沿采样（模式0）
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;					// 8位数据宽度
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;					// 高位先行
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;	// 2分频
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;							// 软件控制NSS
	SPI_InitStructure.SPI_CRCPolynomial = 7;							// CRC默认值
	SPI_Init(SPI1, &SPI_InitStructure);									// 初始化SPI1
	SPI_Cmd(SPI1, ENABLE);
}

void LCD_DMA_Init(uint32_t addr)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	
    DMA_InitTypeDef DMA_InitStruct;
    
    // 先禁用DMA
    DMA_Cmd(DMA2_Stream3, DISABLE);
    
    DMA_InitStruct.DMA_Channel = DMA_Channel_3;
    DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&SPI1->DR;
    DMA_InitStruct.DMA_Memory0BaseAddr = addr;
    DMA_InitStruct.DMA_DIR = DMA_DIR_MemoryToPeripheral;
    DMA_InitStruct.DMA_BufferSize = 0; // 初始为0，每次传输前设置
    DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStruct.DMA_Priority = DMA_Priority_High;
    DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Disable;
    DMA_InitStruct.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
    DMA_InitStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    
    DMA_Init(DMA2_Stream3, &DMA_InitStruct);
    
    // 禁用中断（因为我们使用轮询方式）
    DMA_ITConfig(DMA2_Stream3, DMA_IT_TC, DISABLE);
    
    // 使能SPI1的DMA发送
    SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE);
}

// 交换1字节（SPI轮询发送接收，核心发送函数）
uint8_t LCD_SPI_SwapByte(uint8_t data)
{
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);  // 等待发送缓冲区空
	SPI_I2S_SendData(SPI1, data);                                   // 发送数据
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET); // 等待接收缓冲区满
	return SPI_I2S_ReceiveData(SPI1);                               // 读取接收数据（全双工必须读）
}

// 单独发送1个字节（带片选控制）
void LCD_Send_Byte(uint8_t data)
{
    LCD_W_CS(0);                                                        // 片选使能
	LCD_SPI_SwapByte(data);                                             // 发送字节
	LCD_W_CS(1);	                                                    // 片选禁用
}

// 发送8位数据
void LCD_Send_Data8(uint8_t data)
{
    LCD_W_DC(1);                                                        // 写数据
    LCD_Send_Byte(data);                                                // 发送字节
}

// 发送16位数据（拆分为高低8位）
void LCD_Send_Data16(uint16_t data)
{
    LCD_W_DC(1);
    LCD_Send_Byte((data >> 8) & 0xFF);  // 发送高字节
    LCD_Send_Byte(data & 0xFF);         // 发送低字节
}

// 发送指令
void LCD_Send_Cmd(uint8_t cmd)
{
    LCD_W_DC(0);
    LCD_Send_Byte(cmd);
}

// 地址设置函数（核心：指定显示区域，用于后续发送像素）
void LCD_Address_Set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	if(LCD_ROTATE==0)
	{
		LCD_Send_Cmd(0x2a);  // 列地址设置
		LCD_Send_Data16(x1);
		LCD_Send_Data16(x2);
		LCD_Send_Cmd(0x2b);  // 行地址设置
		LCD_Send_Data16(y1);
		LCD_Send_Data16(y2);
		LCD_Send_Cmd(0x2c);  // 开始写入像素数据
	}
	else if(LCD_ROTATE==1)
	{
		LCD_Send_Cmd(0x2a);
		LCD_Send_Data16(x1);
		LCD_Send_Data16(x2);
		LCD_Send_Cmd(0x2b);
		LCD_Send_Data16(y1);
		LCD_Send_Data16(y2);
		LCD_Send_Cmd(0x2c);
	}
	else if(LCD_ROTATE==2)
	{
		LCD_Send_Cmd(0x2a);
		LCD_Send_Data16(x1);
		LCD_Send_Data16(x2);
		LCD_Send_Cmd(0x2b);
		LCD_Send_Data16(y1);
		LCD_Send_Data16(y2);
		LCD_Send_Cmd(0x2c);
	}
	else
	{
		LCD_Send_Cmd(0x2a);
		LCD_Send_Data16(x1);
		LCD_Send_Data16(x2);
		LCD_Send_Cmd(0x2b);
		LCD_Send_Data16(y1);
		LCD_Send_Data16(y2);
		LCD_Send_Cmd(0x2c);
	}
}

void LCD_Init(void)
{
    LCD_GPIO_Init();
    LCD_SPI_Init();

    LCD_W_RES(0);                                                       // 复位LCD
    Delay_ms(100);
    LCD_W_RES(1);                                                       // 结束复位
    Delay_ms(100);

    LCD_W_BLK(1);                                                       // 打开背光
    Delay_ms(100);

    // LCD初始化序列（原逻辑保留）
	LCD_Send_Cmd(0x11); 												// 退出睡眠 
	Delay_ms(120); 

	LCD_Send_Cmd(0x36);  // 内存访问方向（旋转配置）
	if (LCD_ROTATE == 0)
	{
		LCD_Send_Data8(0x00);
	}
	else if (LCD_ROTATE == 1)
	{
		LCD_Send_Data8(0xC0);
	}
	else if (LCD_ROTATE == 2)
	{
		LCD_Send_Data8(0x70);
	}
	else if (LCD_ROTATE == 3)
	{
		LCD_Send_Data8(0xA0);
	}

	LCD_Send_Cmd(0x3A);			// 像素格式设置
	LCD_Send_Data8(0x05);      // 16位色（RGB565）

	LCD_Send_Cmd(0xB2);			//  porch设置
	LCD_Send_Data8(0x0C);
	LCD_Send_Data8(0x0C); 
	LCD_Send_Data8(0x00); 
	LCD_Send_Data8(0x33); 
	LCD_Send_Data8(0x33); 			

	LCD_Send_Cmd(0xB7);			// 网关控制
	LCD_Send_Data8(0x35);

	LCD_Send_Cmd(0xBB);			// Vcom设置
	LCD_Send_Data8(0x32); 		// Vcom=1.35V
					
	LCD_Send_Cmd(0xC2);			// 电源控制2
	LCD_Send_Data8(0x01);

	LCD_Send_Cmd(0xC3);			// 电源控制3
	LCD_Send_Data8(0x15); 		// GVDD=4.8V
				
	LCD_Send_Cmd(0xC4);			// 电源控制4
	LCD_Send_Data8(0x20); 		// VDV=0v

	LCD_Send_Cmd(0xC6);			// 帧速率控制
	LCD_Send_Data8(0x0F); 		// 60Hz       	

	LCD_Send_Cmd(0xD0);			// 电源控制1
	LCD_Send_Data8(0xA4);
	LCD_Send_Data8(0xA1); 

	LCD_Send_Cmd(0xE0);			// 正伽马校正
	LCD_Send_Data8(0xD0);   
	LCD_Send_Data8(0x08);   
	LCD_Send_Data8(0x0E);   
	LCD_Send_Data8(0x09);   
	LCD_Send_Data8(0x09);   
	LCD_Send_Data8(0x05);   
	LCD_Send_Data8(0x31);   
	LCD_Send_Data8(0x33);   
	LCD_Send_Data8(0x48);   
	LCD_Send_Data8(0x17);   
	LCD_Send_Data8(0x14);   
	LCD_Send_Data8(0x15);   
	LCD_Send_Data8(0x31);   
	LCD_Send_Data8(0x34);   

	LCD_Send_Cmd(0xE1);     	// 负伽马校正
	LCD_Send_Data8(0xD0);   
	LCD_Send_Data8(0x08);   
	LCD_Send_Data8(0x0E);   
	LCD_Send_Data8(0x09);   
	LCD_Send_Data8(0x09);   
	LCD_Send_Data8(0x15);   
	LCD_Send_Data8(0x31);   
	LCD_Send_Data8(0x33);   
	LCD_Send_Data8(0x48);   
	LCD_Send_Data8(0x17);   
	LCD_Send_Data8(0x14);   
	LCD_Send_Data8(0x15);   
	LCD_Send_Data8(0x31);   
	LCD_Send_Data8(0x34);

	LCD_Send_Cmd(0x29); 		// 开启显示
}

// 全屏填充（无缓冲区：直接设置全屏区域，循环发送颜色数据）
void LCD_Full(uint16_t color)
{
    uint32_t total_pixels = (uint32_t)LCD_WIDTH * LCD_HEIGHT;  // 总像素数
    uint8_t color_high = (color >> 8) & 0xFF;                  // 颜色高字节
    uint8_t color_low = color & 0xFF;                           // 颜色低字节

    // 1. 设置显示区域为全屏
    LCD_Address_Set(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);

    // 2. 连续发送像素（减少片选切换，提升速度）
    LCD_W_DC(1);  // 固定为数据模式
    LCD_W_CS(0);  // 片选持续使能（一次选中，发送所有像素）

    // 3. 循环发送所有像素的高低字节
    for (uint32_t i = 0; i < total_pixels; i++)
    {
        LCD_SPI_SwapByte(color_high);
        LCD_SPI_SwapByte(color_low);
    }

    LCD_W_CS(1);  // 结束通信
}

// 画点（无缓冲区：直接设置单个点的区域，发送颜色）
void LCD_DrawPoint(uint16_t x, uint16_t y, uint16_t color)
{
    // 1. 坐标合法性判断（超出屏幕范围则不绘制）
    if (x >= LCD_WIDTH || y >= LCD_HEIGHT)
        return;

    // 2. 设置显示区域为单个点（x1=x2=x，y1=y2=y）
    LCD_Address_Set(x, y, x, y);

    // 3. 发送该点的颜色数据
    LCD_W_DC(1);
    LCD_W_CS(0);
    LCD_SPI_SwapByte((color >> 8) & 0xFF);  // 高字节
    LCD_SPI_SwapByte(color & 0xFF);         // 低字节
    LCD_W_CS(1);
}
