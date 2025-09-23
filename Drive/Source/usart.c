#include "Usart.h"

#include <stdio.h>

// GPIO初始化
void  USART_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd ( USART1_TX_CLK|USART1_RX_CLK, ENABLE); 					// IO口时钟配置

	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;   	 							// 复用模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;   								// 推挽
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;		 							// 上拉
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 								// 速度等级
	GPIO_InitStructure.GPIO_Pin = USART1_TX_PIN;	 
	GPIO_Init(USART1_TX_PORT, &GPIO_InitStructure);													   
	GPIO_InitStructure.GPIO_Pin = USART1_RX_PIN;	
	GPIO_Init(USART1_RX_PORT, &GPIO_InitStructure);		
	GPIO_PinAFConfig(USART1_TX_PORT,USART1_TX_PinSource,GPIO_AF_USART1); 
	GPIO_PinAFConfig(USART1_RX_PORT,USART1_RX_PinSource,GPIO_AF_USART1);	
}

// USART初始化
void Usart_Init(void)
{		
	USART_InitTypeDef USART_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	USART_GPIO_Init();
	USART_InitStructure.USART_BaudRate 	 = USART1_BaudRate; 						// 波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; 					// 数据位8位
	USART_InitStructure.USART_StopBits   = USART_StopBits_1; 						// 停止位1位
	USART_InitStructure.USART_Parity     = USART_Parity_No ; 						// 无校验
	USART_InitStructure.USART_Mode 	    = USART_Mode_Rx | USART_Mode_Tx; 			// 发送和接收模式
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 不使用硬件流控制
	
	USART_Init(USART1,&USART_InitStructure); 										// 初始化串口1
	USART_Cmd(USART1,ENABLE);														// 使能串口1
}

// 重定义fputc函数
int fputc(int c, FILE *fp)
{

	USART_SendData( USART1,(u8)c);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	return (c);
}


