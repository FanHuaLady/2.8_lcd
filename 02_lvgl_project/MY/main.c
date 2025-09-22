#include "stm32f4xx.h"
#include "delay.h"
#include "LCD.h"

#include "lvgl.h" 
#include "lv_port_disp_template.h"

void lv_ex_label(void)
{
	lv_obj_t *label = lv_label_create(lv_scr_act());
    lv_label_set_text(label, "Hello LVGL");
    lv_obj_center(label);
}

int main()
{
	Delay_Init();
	LCD_Init();
	
	lv_init();
	lv_port_disp_init();
	
	lv_ex_label();
	
	while(1)
	{
		lv_task_handler(); // ����LVGL���к�̨���񣨶�����ˢ�¡��¼��ȣ�
        Delay_ms(10);     // ��ʱ10ms�������������Ƶ�� + ����CPUռ��
	}
}
