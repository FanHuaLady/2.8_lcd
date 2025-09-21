#include "LCD.h"
#include "Delay.h"
#include "OLED.h"

uint16_t LCD_LINE_BUFF[LCD_WIDTH];

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
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);				// 这个引脚不是规定的SPI脚，但可以复用
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);				// 复用
	
	SPI_InitTypeDef SPI_InitStructure;									// 定义结构体变量
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;						// 模式，选择为SPI主模式
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;	// 方向，全双工
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;							// SPI极性，选择低极性
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;						// SPI相位，选择第一个时钟边沿采样，极性和相位决定选择SPI模式0
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;					// 数据宽度，选择为8位
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;					// 先行位，选择高位先行
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;	// 波特率分频，选择2分频
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;							// NSS，选择由软件控制
	SPI_InitStructure.SPI_CRCPolynomial = 7;							// CRC多项式，暂时用不到，给默认值7
	SPI_Init(SPI1, &SPI_InitStructure);									// 将结构体变量交给SPI_Init，配置SPI1
	SPI_Cmd(SPI1,ENABLE);
}

// 交换1字节
uint8_t LCD_SPI_SwapByte(uint8_t data)
{
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI1, data);//发送数据
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	return SPI_I2S_ReceiveData(SPI1);
}

void LCD_Send_Byte(uint8_t data)
{
    LCD_W_CS(0);                                                        // 开始通信
	LCD_SPI_SwapByte(data);                                             // 发送字节
	LCD_W_CS(1);	                                                    // 停止通信
}

void LCD_Send_Data8(uint8_t data)
{
    LCD_W_DC(1);                                                        // 写数据
    LCD_Send_Byte(data);                                                // 发送字节
}

void LCD_Send_Data16(uint16_t data)
{
    LCD_W_DC(1);
    LCD_Send_Byte(data >> 8);
    LCD_Send_Byte(data);
}

void LCD_Send_Cmd(uint8_t cmd)
{
    LCD_W_DC(0);
    LCD_Send_Byte(cmd);
}

void LCD_Init(void)
{
    LCD_GPIO_Init();
    LCD_SPI_Init();

    LCD_W_RES(0);                                                       // 复位LCD
    Delay_ms(100);
    LCD_W_RES(1);                                                       // 重启
    Delay_ms(100);

    LCD_W_BLK(1);                                                       // 打开背光
    Delay_ms(100);

    //初始序列
	LCD_Send_Cmd(0x11); 												//退出睡眠 
	Delay_ms(120); 

	LCD_Send_Cmd(0x36);
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

	LCD_Send_Cmd(0x3A);			
	LCD_Send_Data8(0x05);

	LCD_Send_Cmd(0xB2);			
	LCD_Send_Data8(0x0C);
	LCD_Send_Data8(0x0C); 
	LCD_Send_Data8(0x00); 
	LCD_Send_Data8(0x33); 
	LCD_Send_Data8(0x33); 			

	LCD_Send_Cmd(0xB7);			
	LCD_Send_Data8(0x35);

	LCD_Send_Cmd(0xBB);			
	LCD_Send_Data8(0x32); 												//Vcom=1.35V
					
	LCD_Send_Cmd(0xC2);
	LCD_Send_Data8(0x01);

	LCD_Send_Cmd(0xC3);			
	LCD_Send_Data8(0x15); 												//GVDD=4.8V  颜色深度
				
	LCD_Send_Cmd(0xC4);			
	LCD_Send_Data8(0x20); 												//VDV, 0x20:0v

	LCD_Send_Cmd(0xC6);			
	LCD_Send_Data8(0x0F); 												//0x0F:60Hz        	

	LCD_Send_Cmd(0xD0);			
	LCD_Send_Data8(0xA4);
	LCD_Send_Data8(0xA1); 

	LCD_Send_Cmd(0xE0);
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

	LCD_Send_Cmd(0xE1);     
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
	// LCD_Send_Cmd(0x21); 

	LCD_Send_Cmd(0x29);
}

void LCD_Address_Set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	if(LCD_ROTATE==0)
	{
		LCD_Send_Cmd(0x2a);
		LCD_Send_Data16(x1);
		LCD_Send_Data16(x2);
		LCD_Send_Cmd(0x2b);
		LCD_Send_Data16(y1);
		LCD_Send_Data16(y2);
		LCD_Send_Cmd(0x2c);
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

// ---------------------------------------------------------------------// 应用层

// 发送指定行的缓冲区数据到LCD
void LCD_LineBuf_Send(uint16_t y)
{
    LCD_Address_Set(0, y, LCD_WIDTH - 1, y);
    
    LCD_W_DC(1);
    LCD_W_CS(0);
    for (uint16_t x = 0; x < LCD_WIDTH; x++)
    {
        LCD_SPI_SwapByte(LCD_LINE_BUFF[x] >> 8);  						// 高8位
        LCD_SPI_SwapByte(LCD_LINE_BUFF[x] & 0xFF); 						// 低8位
    }
    LCD_W_CS(1);
}

// 将一行设置颜色
void LCD_LineBuf_Fill(uint16_t y, uint16_t color)
{
    for (uint16_t x = 0; x < LCD_WIDTH; x++)
    {
        LCD_LINE_BUFF[x] = color;
    }
}

void LCD_DrawPoint_LineBuf(uint16_t x, uint16_t y, uint16_t color)
{
    LCD_LINE_BUFF[x] = color;
}

void LCD_FullScreen_Fill(uint16_t color)
{
    for (uint16_t y = 0; y < LCD_HEIGHT; y++)
    {
        LCD_LineBuf_Fill(y, color);  // 填充当前行的缓冲区
        LCD_LineBuf_Send(y);         // 发送该行到LCD
    }
}
