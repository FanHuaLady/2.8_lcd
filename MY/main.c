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
    printf("%s\r\n", buf); // 直接打印LVGL已经格式化好的日志字符串
}

int main()
{
	Timer_Init();
	Usart_Init();
	
	lv_init();                             											// LVGL 初始化
	lv_port_disp_init();                   											// 注册LVGL的显示任务
	lv_port_indev_init();
	
	lv_log_register_print_cb(my_log_cb);
	
	lv_obj_t *spinner = lv_spinner_create(lv_scr_act(),1000,60);					//设置指示器
	lv_obj_set_style_arc_width(spinner,15,LV_PART_MAIN);							//设置主体圆弧宽度
	lv_obj_set_style_arc_width(spinner,15,LV_PART_INDICATOR);						//设置指示器圆弧宽度
	lv_obj_set_size(spinner, 80, 80);  												//设置加载器宽高
	lv_obj_center(spinner);															//设置加载器位置
	
	lv_obj_t *label1 = lv_label_create(lv_scr_act());								//创建标签(tab1)
	lv_label_set_text(label1,"hello");												//设置标签内容
	lv_obj_center(label1);
	
	lv_obj_t *btn = lv_btn_create(lv_scr_act());            						// 创建按钮
    lv_obj_set_size(btn, 120, 40);                          						// 大小：120×40
    lv_obj_set_pos(btn, 100, 180);                           						// 定位
	
	lv_obj_t *label_btn = lv_label_create(btn);             						// 按钮上的标签
    lv_label_set_text(label_btn, "click me");               						// 英文文本
    lv_obj_center(label_btn);                               						// 标签居中
	
	/*
	无法使用列表
	lv_obj_t *list = lv_list_create(lv_scr_act());
	lv_obj_set_size(list, 180, 80);                         						// 大小：180×80
    lv_obj_set_pos(list, 70, 180);                          						// 定位在屏幕下方
	*/
	
	/*
	无法使用滑块
	lv_obj_t *slider = lv_slider_create(lv_scr_act());
    lv_obj_set_size(slider, 200, 20);                       // 大小：200×20
    lv_obj_set_pos(slider, 60, 120);                        // 定位
    lv_slider_set_range(slider, 0, 100);                    // 取值范围：0-100
	*/
	
	while(1)
	{
		Delay_ms(1);
		lv_timer_handler();
	}
}

void TIM4_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)  // 检查更新中断标志
    {
        lv_tick_inc(10);  // 向LVGL报告：过去了10ms
        TIM_ClearITPendingBit(TIM4, TIM_IT_Update);     // 清除中断标志，避免重复触发
    }
}
