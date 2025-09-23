/**
 * @file lv_port_disp_templ.c
 *
 */

/*Copy this file as "lv_port_disp.c" and set this value to "1" to enable content*/
#if 1

/*********************
 *      INCLUDES
 *********************/
#include "lv_port_disp.h"
#include <stdbool.h>
#include "LCD.h"
/*********************
 *      DEFINES
 *********************/
#ifndef MY_DISP_HOR_RES
    #warning Please define or replace the macro MY_DISP_HOR_RES with the actual screen width, default value 320 is used for now.
    #define MY_DISP_HOR_RES    320
#endif

#ifndef MY_DISP_VER_RES
    #warning Please define or replace the macro MY_DISP_HOR_RES with the actual screen height, default value 240 is used for now.
    #define MY_DISP_VER_RES    240
#endif

static void disp_init(void);
static void disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p);

static lv_disp_draw_buf_t draw_buf_dsc_1;														// 连接 “用户定义的内存缓冲区” 和 “显示驱动” 的桥梁
static lv_color_t mybuff[MY_DISP_HOR_RES * 40];													// 小型缓冲区域
 
/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_port_disp_init(void)
{
    disp_init();																				// 初始化显示设备
	LCD_DMA_Init((uint32_t)mybuff);
	
	lv_disp_draw_buf_init(&draw_buf_dsc_1, mybuff, NULL, MY_DISP_HOR_RES * 40);					// 缓冲区初始化

    static lv_disp_drv_t disp_drv;                         										/*Descriptor of a display driver*/
    lv_disp_drv_init(&disp_drv);                    											/*Basic initialization*/

    disp_drv.hor_res = MY_DISP_HOR_RES;															// 设置屏幕宽
    disp_drv.ver_res = MY_DISP_VER_RES;

    disp_drv.flush_cb = disp_flush;																// 用于将缓冲区的内容复制到显示屏上
    disp_drv.draw_buf = &draw_buf_dsc_1;
	
    lv_disp_drv_register(&disp_drv);
}

// LCD初始化
static void disp_init(void)
{
    LCD_Init();
}

volatile bool disp_flush_enabled = true;

/* Enable updating the screen (the flushing process) when disp_flush() is called by LVGL
 */
void disp_enable_update(void)
{
    disp_flush_enabled = true;
}

/* Disable updating the screen (the flushing process) when disp_flush() is called by LVGL
 */
void disp_disable_update(void)
{
    disp_flush_enabled = false;
}

/*将内部缓冲区的内容刷新到显示屏的特定区域上。
 *您可以在后台使用直接内存访问（DMA）或任何硬件加速技术来执行此操作，但是
 *完成操作后必须调用“lv_disp_flush_ready()”函数。.*/
static void disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p)
{
    if(disp_flush_enabled) 
    {
        uint32_t width = area->x2 - area->x1 + 1;
        uint32_t height = area->y2 - area->y1 + 1;
        uint32_t data_size = width * height * sizeof(lv_color_t);
        
        LCD_Address_Set(area->x1, area->y1, area->x2, area->y2);
		
        LCD_W_DC(1);  																// 数据模式
        LCD_W_CS(0);  																// 片选使能
		SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE);
		
        // 配置DMA传输
        DMA_Cmd(DMA2_Stream3, DISABLE);  											// 先关闭DMA
        while(DMA_GetCmdStatus(DMA2_Stream3) != DISABLE);  							// 等待关闭完成
        DMA_SetCurrDataCounter(DMA2_Stream3, data_size); 							// 传输字节数
        DMA_Cmd(DMA2_Stream3, ENABLE);												// 使能DMA
		
        while(DMA_GetFlagStatus(DMA2_Stream3, DMA_FLAG_TCIF3) == RESET);			// 等待DMA传输完成
        DMA_ClearFlag(DMA2_Stream3,DMA_FLAG_TCIF3);
        
        LCD_W_CS(1);  																// 片选禁用
    }

    lv_disp_flush_ready(disp_drv);
}

#else /*Enable this file at the top*/

/*This dummy typedef exists purely to silence -Wpedantic.*/
typedef int keep_pedantic_happy;
#endif
