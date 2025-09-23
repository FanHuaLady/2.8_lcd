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

static lv_disp_draw_buf_t draw_buf_dsc_1;														// ���� ���û�������ڴ滺������ �� ����ʾ������ ������
static lv_color_t mybuff[MY_DISP_HOR_RES * 40];													// С�ͻ�������
 
/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_port_disp_init(void)
{
    disp_init();																				// ��ʼ����ʾ�豸
	LCD_DMA_Init((uint32_t)mybuff);
	
	lv_disp_draw_buf_init(&draw_buf_dsc_1, mybuff, NULL, MY_DISP_HOR_RES * 40);					// ��������ʼ��

    static lv_disp_drv_t disp_drv;                         										/*Descriptor of a display driver*/
    lv_disp_drv_init(&disp_drv);                    											/*Basic initialization*/

    disp_drv.hor_res = MY_DISP_HOR_RES;															// ������Ļ��
    disp_drv.ver_res = MY_DISP_VER_RES;

    disp_drv.flush_cb = disp_flush;																// ���ڽ������������ݸ��Ƶ���ʾ����
    disp_drv.draw_buf = &draw_buf_dsc_1;
	
    lv_disp_drv_register(&disp_drv);
}

// LCD��ʼ��
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

/*���ڲ�������������ˢ�µ���ʾ�����ض������ϡ�
 *�������ں�̨ʹ��ֱ���ڴ���ʣ�DMA�����κ�Ӳ�����ټ�����ִ�д˲���������
 *��ɲ����������á�lv_disp_flush_ready()��������.*/
static void disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p)
{
    if(disp_flush_enabled) 
    {
        uint32_t width = area->x2 - area->x1 + 1;
        uint32_t height = area->y2 - area->y1 + 1;
        uint32_t data_size = width * height * sizeof(lv_color_t);
        
        LCD_Address_Set(area->x1, area->y1, area->x2, area->y2);
		
        LCD_W_DC(1);  																// ����ģʽ
        LCD_W_CS(0);  																// Ƭѡʹ��
		SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE);
		
        // ����DMA����
        DMA_Cmd(DMA2_Stream3, DISABLE);  											// �ȹر�DMA
        while(DMA_GetCmdStatus(DMA2_Stream3) != DISABLE);  							// �ȴ��ر����
        DMA_SetCurrDataCounter(DMA2_Stream3, data_size); 							// �����ֽ���
        DMA_Cmd(DMA2_Stream3, ENABLE);												// ʹ��DMA
		
        while(DMA_GetFlagStatus(DMA2_Stream3, DMA_FLAG_TCIF3) == RESET);			// �ȴ�DMA�������
        DMA_ClearFlag(DMA2_Stream3,DMA_FLAG_TCIF3);
        
        LCD_W_CS(1);  																// Ƭѡ����
    }

    lv_disp_flush_ready(disp_drv);
}

#else /*Enable this file at the top*/

/*This dummy typedef exists purely to silence -Wpedantic.*/
typedef int keep_pedantic_happy;
#endif
