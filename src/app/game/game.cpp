/*
 * @Author: WYJ
 * @Date: 2022-04-03 15:14:01
 * @LastEditors: WYJ
 * @LastEditTime: 2022-04-03 17:07:21
 * @FilePath: \littleVisual\src\app\game\game.cpp
 * @Description: 
 */


#include "game.h"
#include "lvgl.h"
#include "app/apps.h"
#include "driver/mpu.h"
#include "MPU6050.h"

LV_FONT_DECLARE(lv_font_montserrat_22);

extern MPU6050 mpu;
extern lv_indev_t * indev_encoder;
static lv_group_t *game_group;

static hw_timer_t *timer_game = NULL;

static bool ifMsgbox = true;       // 是否创建Msgbox
static int MPUState;

static lv_obj_t * scr_game;
static lv_obj_t * btn_init;
static lv_obj_t * btn_init_label;

static lv_obj_t * MsgLabel;

static lv_style_t btn_style;
static lv_style_t label_style;

static void game_init(void);
static void game_style_init(void);
static void btn_init_cb(lv_event_t * e);
static void IRAM_ATTR game_timer_cb(void);

void game(void)
{
    MPUState = UNINITIALIZED;
    
    scr_game = lv_scr_act();
    lv_obj_clean(scr_game);
    
    /* 添加组，encoder必须将部件加入组，并与indev绑定 */
    game_group = lv_group_create();
    lv_indev_set_group(indev_encoder, game_group);  // 重要，必须添加

    game_init();

    timer_game = timerBegin(1,80,true);
    timerAttachInterrupt(timer_game, game_timer_cb, true);
    timerAlarmWrite(timer_game, 1000*1000, true);            // 100ms 一次
    timerAlarmEnable(timer_game);
}

static void game_init(void)
{
    game_style_init();

    btn_init = lv_btn_create(scr_game);
    lv_obj_add_style(btn_init, &btn_style, LV_PART_MAIN);
    lv_obj_set_size(btn_init, 100, 32);
    lv_obj_center(btn_init);
    btn_init_label = lv_label_create(btn_init);
    lv_obj_align(btn_init_label,LV_ALIGN_CENTER, 0, 0);
    lv_label_set_text(btn_init_label, "Init");

    lv_obj_add_event_cb(btn_init, btn_init_cb, LV_EVENT_CLICKED, NULL);
    
    lv_group_add_obj(game_group, btn_init);
}

static void game_style_init(void)
{
    lv_style_init(&btn_style);
    lv_style_set_bg_color(&btn_style, lv_palette_main(LV_PALETTE_GREY));
    lv_style_set_text_color(&btn_style, lv_color_black());
    lv_style_set_text_opa(&btn_style, LV_OPA_100);
    lv_style_set_text_font(&btn_style, &lv_font_montserrat_22);

    lv_style_init(&label_style);
    lv_style_set_text_font(&label_style, &lv_font_montserrat_22);
    lv_style_set_text_color(&label_style, lv_color_black());
}

static void btn_init_cb(lv_event_t * e)
{
    lv_obj_t * obj = lv_event_get_target(e);
    lv_event_code_t code = lv_event_get_code(e);

    if(code == LV_EVENT_CLICKED)
    {
        if(obj == btn_init)
        {
            // MsgLabel = lv_label_create(scr_game);
            // lv_obj_add_style(MsgLabel, &label_style, 0);
            // lv_label_set_text(MsgLabel, "Init...\nPlease dont't move!");
            // ifMsgbox = !ifMsgbox;
            // MPUState = INITIALIZED;
        }
    }
}

static int16_t ax,ay,az,gx,gy,gz;

static void IRAM_ATTR game_timer_cb(void)
{
    if(MPUState == UNINITIALIZED)
    {
        return;
    }
    else if (MPUState == INITIALIZING)
    {
        // Serial.println("Init...");
        // timerAlarmDisable(timer_game);
        // if( MPU_init() )
        // {
        //     lv_label_set_text(MsgLabel, "Init OK!!");
        //     MPUState = INITIALIZED;
        // }
        // else
        // {
        //     lv_label_set_text(MsgLabel, "Init Failed!!");
        // }
        // timerRestart(timer_game);
        // timerAlarmEnable(timer_game);   
    }
    else if (MPUState == INITIALIZED)
    {
        // Serial.println("MPU data:");
        // mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
        // char mpu_info[60] = {0};
        // sprintf(mpu_info, "ax: %d, ay: %d, az: %d, gx: %d, gy: %d, gz: %d", ax, ay, az, gx, gy, gz);
        // Serial.println(mpu_info);
        
        // Serial.println( mpu.getAccelerationY() );
        // Serial.println( mpu.getAccelerationZ() );
    }
}