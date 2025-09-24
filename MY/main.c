#include "stm32f4xx.h"
#include "delay.h"
#include "LCD.h"
#include "Timer.h"
#include "Touch.h"
#include "usart.h"

#include "lvgl.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"

#include <stdio.h>

void my_log_cb(const char * buf)
{
    printf("%s\r\n", buf); // ֱ�Ӵ�ӡLVGL�Ѿ���ʽ���õ���־�ַ���
}

int main()
{
	Timer_Init();
	Usart_Init();
	
	lv_init();                             											// LVGL ��ʼ��
	lv_port_disp_init();                   											// ע��LVGL����ʾ����
	lv_port_indev_init();
	
	lv_log_register_print_cb(my_log_cb);
	
	lv_obj_t *spinner = lv_spinner_create(lv_scr_act(),1000,60);					//����ָʾ��
	lv_obj_set_style_arc_width(spinner,15,LV_PART_MAIN);							//��������Բ�����
	lv_obj_set_style_arc_width(spinner,15,LV_PART_INDICATOR);						//����ָʾ��Բ�����
	lv_obj_set_size(spinner, 80, 80);  												//���ü��������
	lv_obj_center(spinner);															//���ü�����λ��
	
	lv_obj_t *label1 = lv_label_create(lv_scr_act());								//������ǩ(tab1)
	lv_label_set_text(label1,"hello");												//���ñ�ǩ����
	lv_obj_center(label1);
	
	lv_obj_t *btn = lv_btn_create(lv_scr_act());            						// ������ť
    lv_obj_set_size(btn, 120, 40);                          						// ��С��120��40
    lv_obj_set_pos(btn, 100, 180);                           						// ��λ
	
	lv_obj_t *label_btn = lv_label_create(btn);             						// ��ť�ϵı�ǩ
    lv_label_set_text(label_btn, "click me");               						// Ӣ���ı�
    lv_obj_center(label_btn);                               						// ��ǩ����
	
	/*
	�޷�ʹ���б�
	lv_obj_t *list = lv_list_create(lv_scr_act());
	lv_obj_set_size(list, 180, 80);                         						// ��С��180��80
    lv_obj_set_pos(list, 70, 180);                          						// ��λ����Ļ�·�
	*/
	
	/*
	�޷�ʹ�û���
	lv_obj_t *slider = lv_slider_create(lv_scr_act());
    lv_obj_set_size(slider, 200, 20);                       // ��С��200��20
    lv_obj_set_pos(slider, 60, 120);                        // ��λ
    lv_slider_set_range(slider, 0, 100);                    // ȡֵ��Χ��0-100
	*/
	
	while(1)
	{
		Delay_ms(1);
		lv_timer_handler();
	}
}

void TIM4_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)  // �������жϱ�־
    {
        lv_tick_inc(10);  // ��LVGL���棺��ȥ��10ms
        TIM_ClearITPendingBit(TIM4, TIM_IT_Update);     // ����жϱ�־�������ظ�����
    }
}
