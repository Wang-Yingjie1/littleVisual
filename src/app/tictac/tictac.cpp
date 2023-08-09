/*
 * @Author: WYJ
 * @Date: 2022-03-28 17:03:20
 * @LastEditors: WYJ
 * @LastEditTime: 2022-04-03 12:56:39
 * @FilePath: \littleVisual\src\app\tictac\tictac.cpp
 * @Description: 
 */
#include "lvgl.h"
#include "tictac.h"
#include "app/apps.h"

extern lv_indev_t * indev_encoder;
LV_FONT_DECLARE(lv_font_montserrat_22);
LV_FONT_DECLARE(lv_font_montserrat_32);
LV_FONT_DECLARE(lv_font_ibmplex_64);
LV_FONT_DECLARE(lv_font_ibmplex_115);

static lv_obj_t *scr_tictac = NULL;
static lv_group_t * tictac_group;

static lv_obj_t * title;
static lv_obj_t * titleLabel;
static lv_obj_t * timer1;
static lv_obj_t * timer2;
static lv_obj_t * btn_start_stop;
static lv_obj_t * btn_start_stop_label;
static lv_obj_t * btn_flag;
static lv_obj_t * btn_flag_label;
static lv_obj_t * btn_back;
static lv_obj_t * btn_back_label;
static lv_obj_t * time_recoder_label[5];

static lv_style_t scr_tictac_style;
static lv_style_t title_style;
static lv_style_t title_label_style;
static lv_style_t time_recoder_style;
static lv_style_t timer_runner_style1;
static lv_style_t timer_runner_style2;
static lv_style_t btn_style;
//static lv_style_t btn_start_stop_label_style;

static bool running = false;
hw_timer_t *timer_tictac;
static unsigned long DRAM_ATTR start_time = 0;
static unsigned long DRAM_ATTR now_time = 0; 
static volatile int hour = 0;
static volatile int minute = 0;
static volatile int sec = 0;
static volatile int dsec = 0;    // 1/10 s
static uint32_t recode_counter = 0;

static void IRAM_ATTR decode_time(unsigned long start, unsigned long end);
static void IRAM_ATTR update_time(void);
static void IRAM_ATTR timer_tictac_cb(void);
static void btn_start_stop_cb(lv_event_t * e);
static void btn_flag_cb(lv_event_t * e);
static void btn_back_cb(lv_event_t * e);
static void tictac_style_init(void);
static void tictac_style_reset(void);
static void tictac_clean(void);
static void tictac_init(void);

static void IRAM_ATTR decode_time(unsigned long start, unsigned long end)
{
    unsigned long interval = (end - start)/100;     // 变成 dsec, 1/10 s
    dsec = interval % 10;
    sec = (interval / 10) % 60;
    minute = (interval / 600) % 60;
    hour = (interval / 36000) % 60;

}
static void IRAM_ATTR update_time(void)
{
    lv_label_set_text_fmt(timer1, "#ff0000 %01d#%02d#ffa500 %02d#",hour, minute, sec);
    lv_label_set_text_fmt(timer2, "%1d", dsec);
}

static void IRAM_ATTR timer_tictac_cb(void)
{
    now_time = millis();
    decode_time(start_time, now_time);
    update_time();
}

static void btn_start_stop_cb(lv_event_t * e)
{
    if(!running)
    {
        start_time = millis();
        timer_tictac = timerBegin(1,80,true);
        timerAttachInterrupt(timer_tictac, timer_tictac_cb, true);
        timerAlarmWrite(timer_tictac, 10*1000, true); // 10k us = 10 ms
        timerAlarmEnable(timer_tictac);
        lv_label_set_text(btn_start_stop_label, "Stop");
        running = true;
        for(int i = 0; i < 5; i++)
        {    
            lv_label_set_text_fmt(time_recoder_label[i], "%d: %d:%02d:%02d:%d", i+1, 0, 0, 0, 0);
        }
    }
    else
    {
        lv_label_set_text(btn_start_stop_label, "Start");
        timerAlarmDisable(timer_tictac);
        timerEnd(timer_tictac);
        running = false;
        recode_counter = 0;
    }
        
}

static void btn_flag_cb(lv_event_t * e)
{
    if(running)
    {
        now_time = millis();
        decode_time(start_time, now_time);
        uint32_t temp = recode_counter % 5;
        lv_label_set_text_fmt(time_recoder_label[temp], "%d: %d:%02d:%02d:%d", temp+1, hour, minute, sec, dsec);
        recode_counter++;
    }
}

static void btn_back_cb(lv_event_t * e)
{
    tictac_clean();
    if(running)
    {
        timerAlarmDisable(timer_tictac);
        timerEnd(timer_tictac);
    }
    APP_selector();
}

static void tictac_clean(void)
{
    lv_obj_clean(scr_tictac);
    tictac_style_reset();
    lv_group_del(tictac_group);
}

static void tictac_style_reset(void)
{
    lv_style_reset(&scr_tictac_style);
    lv_style_reset(&title_style);
    lv_style_reset(&title_label_style);
    lv_style_reset(&timer_runner_style1);
    lv_style_reset(&timer_runner_style2);
    lv_style_reset(&time_recoder_style);
}
static void tictac_style_init(void)
{
    lv_style_init(&scr_tictac_style);
    lv_style_set_bg_color(&scr_tictac_style,lv_color_black());

    lv_style_init(&title_style);
    lv_style_set_bg_color(&title_style, lv_palette_main(LV_PALETTE_ORANGE));
    lv_style_set_text_color(&title_style, lv_color_white());
    lv_style_set_opa(&title_style, LV_OPA_100);

    lv_style_init(&title_label_style);
    lv_style_set_text_opa(&title_label_style, LV_OPA_100);
    lv_style_set_text_font(&title_label_style, &lv_font_montserrat_32);

    lv_style_init(&timer_runner_style1);
    lv_style_set_text_opa(&timer_runner_style1, LV_OPA_100);
    lv_style_set_text_color(&timer_runner_style1, lv_color_white());
    lv_style_set_text_font(&timer_runner_style1, &lv_font_ibmplex_115);

    lv_style_init(&timer_runner_style2);
    lv_style_set_text_opa(&timer_runner_style2, LV_OPA_100);
    lv_style_set_text_color(&timer_runner_style2, lv_color_white());
    lv_style_set_text_font(&timer_runner_style2, &lv_font_ibmplex_64);

    lv_style_init(&btn_style);
    lv_style_set_bg_color(&btn_style, lv_palette_main(LV_PALETTE_RED));
    lv_style_set_text_opa(&btn_style, LV_OPA_100);
    lv_style_set_text_color(&btn_style, lv_color_white());
    lv_style_set_text_font(&btn_style, &lv_font_montserrat_22);

    lv_style_init(&time_recoder_style);
    lv_style_set_bg_color(&time_recoder_style, lv_color_black());
    lv_style_set_text_opa(&time_recoder_style, LV_OPA_100);
    lv_style_set_text_color(&time_recoder_style, lv_palette_main(LV_PALETTE_CYAN));
    //lv_style_set_text_font(&time_recoder_style, &lv_font_montserrat_22);
    lv_style_set_border_color(&time_recoder_style, lv_palette_main(LV_PALETTE_GREY));
    lv_style_set_border_width(&time_recoder_style, 2);
    lv_style_set_radius(&time_recoder_style, 8);
    lv_style_set_text_align(&time_recoder_style, LV_TEXT_ALIGN_CENTER);

}

static void tictac_init(void)
{
    recode_counter = 0;
    tictac_style_init();

    lv_obj_add_style(scr_tictac, &scr_tictac_style, 0);
// Title
    title = lv_obj_create(scr_tictac);
    lv_obj_add_style(title, &title_style, LV_PART_MAIN);
    lv_obj_set_size(title, 240, 50);
    titleLabel = lv_label_create(title);
    lv_obj_align(titleLabel, LV_ALIGN_CENTER, 0, 0);
    lv_label_set_text(titleLabel, "Tictac");
    lv_obj_add_style(titleLabel, &title_label_style, LV_PART_MAIN);
        
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 0);

// Timer
    timer1 = lv_label_create(scr_tictac);
    lv_obj_add_style(timer1, &timer_runner_style1, LV_PART_MAIN);
    lv_label_set_recolor(timer1, true);
    lv_label_set_text_fmt(timer1, "#ff0000 %01d#%02d#ffa500 %02d#",0, 00, 00);

    timer2 = lv_label_create(scr_tictac);
    lv_obj_add_style(timer2, &timer_runner_style2, LV_PART_MAIN);
    lv_label_set_recolor(timer2, true);
    lv_label_set_text_fmt(timer2, "%01d", 0);

    lv_obj_align(timer1, LV_ALIGN_TOP_LEFT, 0, 40);
    lv_obj_align(timer2, LV_ALIGN_TOP_LEFT, 205, 70);


// Button
    btn_start_stop = lv_btn_create(scr_tictac);
    lv_obj_add_style(btn_start_stop, &btn_style, LV_PART_MAIN);
    lv_obj_set_size(btn_start_stop, 100, 32);
    btn_start_stop_label = lv_label_create(btn_start_stop);
    lv_obj_align(btn_start_stop_label,LV_ALIGN_CENTER, 0, 0);
    lv_label_set_text(btn_start_stop_label, "Start");

    btn_flag = lv_btn_create(scr_tictac);
    lv_obj_add_style(btn_flag, &btn_style, LV_PART_MAIN);
    lv_obj_set_size(btn_flag, 100, 32);
    btn_flag_label = lv_label_create(btn_flag);
    lv_obj_align(btn_flag_label, LV_ALIGN_CENTER, 0, 0);
    lv_label_set_text(btn_flag_label, "Flag");

    btn_back = lv_btn_create(scr_tictac);
    lv_obj_add_style(btn_back, &btn_style, LV_PART_MAIN);
    lv_obj_set_size(btn_back, 100, 32);
    btn_back_label = lv_label_create(btn_back);
    lv_obj_align(btn_back_label, LV_ALIGN_CENTER, 0, 0);
    lv_label_set_text(btn_back_label, "Back");
    
    lv_obj_add_event_cb(btn_start_stop, btn_start_stop_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(btn_flag, btn_flag_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(btn_back, btn_back_cb, LV_EVENT_CLICKED, NULL);

    lv_group_add_obj(tictac_group, btn_start_stop);
    lv_group_add_obj(tictac_group, btn_flag);
    lv_group_add_obj(tictac_group, btn_back);
    
    lv_obj_align(btn_start_stop, LV_ALIGN_BOTTOM_RIGHT, -5, -85);
    lv_obj_align(btn_flag, LV_ALIGN_BOTTOM_RIGHT, -5, -45);
    lv_obj_align(btn_back, LV_ALIGN_BOTTOM_RIGHT, -5, -5);

// TimeRecoder
    for(int i = 0; i < 5; i++)
    {    
        time_recoder_label[i] = lv_label_create(scr_tictac);
        lv_obj_add_style(time_recoder_label[i], &time_recoder_style, 0);
        lv_obj_set_size(time_recoder_label[i], 110, 20);
        lv_obj_align(time_recoder_label[i], LV_ALIGN_BOTTOM_LEFT, 13, -3-24*(4-i));
        lv_label_set_text_fmt(time_recoder_label[i], "%d: %d:%02d:%02d:%d", i+1, 0, 0, 0, 0);
    }
}

void tictac(void)
{
    tictac_group = lv_group_create();
    lv_indev_set_group(indev_encoder, tictac_group);

    scr_tictac = lv_scr_act();
    lv_obj_clean(scr_tictac);

    tictac_init();
    
}