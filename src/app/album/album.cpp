/*
 * @Author: WYJ
 * @Date: 2022-03-25 23:00:12
 * @LastEditors: WYJ
 * @LastEditTime: 2022-04-03 15:06:18
 * @FilePath: \littleVisual\src\app\album\album.cpp
 * @Description: 
 */

#include "lvgl.h"
#include "album.h"
#include "app/apps.h"
#include "time.h"
#include "config.h"
// #include "TFT_eSPI.h"
// #include "TJpg_Decoder.h"
// #include "driver/lv_timer.h"

// extern TFT_eSPI screen;
extern lv_indev_t * indev_encoder;          // 旋转编码器，绑定组使用
extern SdCard tf;
extern ConfigStruct cfgstruct;

static hw_timer_t *timer_album = NULL;
static lv_group_t * album_group = NULL;

static AlbumStructure *album_data = NULL;

static lv_obj_t * scr_album = NULL;
static lv_obj_t * photo_image = NULL;
static lv_obj_t * a_touming_button1;
static lv_obj_t * a_touming_button2;

static lv_style_t scr_album_style;
static lv_style_t touming_style;

char file_name[PIC_FILENAME_MAX_LEN] = {0};
static int whichisfocused = 1;

static void scr_album_cb(lv_event_t *e);
static File_Info * IRAM_ATTR get_next_file(File_Info *p_cur_file, int direction);
static void album_style_reset(void);
static void album_style_init(void);
static void album_clean(void);
static void album_init(void);
static void display_photo(const char *file_name);
static void IRAM_ATTR Album_auto_display_cb(void);

static void album_clean(void)
{
    lv_obj_clean(scr_album);
    album_style_reset();
    lv_group_del(album_group);
}

static File_Info * IRAM_ATTR get_next_file(File_Info *p_cur_file, int direction)
{
    // 得到 p_cur_file 的下一个 类型为FILE_TYPE_FILE 的文件（即下一个非文件夹文件）
    if (NULL == p_cur_file)
    {
        return NULL;
    }

    File_Info * pfile = direction == 1 ? p_cur_file->next_node : p_cur_file->front_node;
    while (pfile != p_cur_file)
    {
        if (FILE_TYPE_FILE == pfile->file_type)
        {
            break;
        }
        pfile = direction == 1 ? pfile->next_node : pfile->front_node;
    }
    return pfile;
}

static void album_style_reset(void)
{
    lv_style_reset(&scr_album_style);
    lv_style_reset(&touming_style);
}

static void album_style_init(void)
{
    lv_style_init(&scr_album_style);
    lv_style_set_bg_color(&scr_album_style, lv_color_black());
    // lv_obj_add_style(scr_album, &scr_album_style, LV_PART_MAIN);

    lv_style_init(&touming_style); 
    lv_style_set_opa(&touming_style, LV_OPA_0);
}

// // This next function will be called during decoding of the jpeg file to
// // render each block to the TFT.  If you use a different TFT library
// // you will need to adapt this function to suit.
// bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t *bitmap)
// {   
//     // Stop further decoding as image is running off bottom of screen
//     if (y >= screen.height())
//         return 0;

//     // This function will clip the image block rendering automatically at the TFT boundaries
//     screen.pushImage(x, y, w, h, bitmap);

//     // This might work instead if you adapt the sketch to use the Adafruit_GFX library
//     // tft.drawRGBBitmap(x, y, bitmap, w, h);

//     // Return 1 to decode next block
//     return 1;
// }

/**
 * @description: 相册的准备工作 
 * @param {*}
 * @return {*}
 */
static void album_init(void)
{
    album_style_init();

    album_data = (AlbumStructure *)malloc(sizeof(AlbumStructure));

    album_data->image_file = NULL;
    album_data->pfile = NULL;
    album_data->image_pos_increase = 1;

    album_data->image_file = tf.listDir(IMAGE_PATH);

    if(album_data->image_file != NULL)
    {
        album_data->pfile = get_next_file(album_data->image_file->next_node, 1);
    }

    // TJpgDec.setSwapBytes(true); 
    // // The jpeg image can be scaled by a factor of 1, 2, 4, or 8
    // TJpgDec.setJpgScale(1);
    // // The decoder must be given the exact name of the rendering function above
    // TJpgDec.setCallback(tft_output);
}

static void display_photo(const char *file_name)
{
    lv_obj_clean(scr_album); // 清空此前页面

    photo_image = lv_img_create(scr_album);
    char lv_file_name[PIC_FILENAME_MAX_LEN] = {0};
    sprintf(lv_file_name, "S:%s", file_name);
    lv_img_set_src(photo_image, lv_file_name);
    lv_obj_align(photo_image, LV_ALIGN_CENTER, 0, 0);

    // 创建一个按钮，用于回调
    a_touming_button1 = lv_btn_create(scr_album);
    lv_obj_align(a_touming_button1, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_add_event_cb(a_touming_button1,scr_album_cb,LV_EVENT_ALL,NULL);
    lv_obj_add_style(a_touming_button1, &touming_style, LV_PART_MAIN);

    a_touming_button2 = lv_btn_create(scr_album);
    lv_obj_align(a_touming_button2, LV_ALIGN_TOP_RIGHT, 0, 0);
    lv_obj_add_event_cb(a_touming_button2,scr_album_cb,LV_EVENT_ALL,NULL);
    lv_obj_add_style(a_touming_button2, &touming_style, LV_PART_MAIN);

    lv_group_add_obj(album_group, a_touming_button1);
    lv_group_add_obj(album_group, a_touming_button2);

    if(whichisfocused == 1)
    {
        lv_group_focus_obj(a_touming_button1);
    }
    else if(whichisfocused == 2)
    {
        lv_group_focus_obj(a_touming_button2);
    }
}

// 在这个界面自动切换图片
static void IRAM_ATTR Album_auto_display_cb(void)
{   
    // timerRestart(timer_album);
    // timerWrite(timer_album, 0);
    if(cfgstruct.Autoflush)
    {
        timerAlarmDisable(timer_album);   
    }

    if(album_data->image_file != NULL)
    {  
       album_data->pfile = get_next_file(album_data->pfile, album_data->image_pos_increase); 
    }

    snprintf(file_name, PIC_FILENAME_MAX_LEN, "%s/%s", album_data->image_file->file_name, album_data->pfile->file_name);

    Serial.print(F("Decode image: "));
    Serial.println(file_name);
        
    if (NULL != strstr(file_name, ".bin") || NULL != strstr(file_name, ".BIN"))
    {
        // 使用LVGL的bin格式的图片
        display_photo(file_name);
    }
    if(cfgstruct.Autoflush)
    {
        timerRestart(timer_album);
        timerAlarmEnable(timer_album);   
    }
}

static void scr_album_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);    // 获取触发事件
    lv_obj_t *obj = lv_event_get_target(e);
    
    if(code == LV_EVENT_LONG_PRESSED)
    {
        if(cfgstruct.Autoflush)
        {
            timerAlarmDisable(timer_album);
            timerEnd(timer_album);
        }
        
        Serial.println("LONG PRESSED!!!!!!!!");
        if (NULL != photo_image)
        {
            lv_obj_clean(photo_image); // 清空此前页面
            photo_image = NULL;
        }
        album_clean();
        // 释放文件名链表
        release_file_info(album_data->image_file);
        free(album_data);
        whichisfocused = 1;
        APP_selector();
    }
    else if(code == LV_EVENT_CLICKED)
    {
        Serial.println("CLICKED!!!");
        if(obj == a_touming_button1)
        {
            album_data->image_pos_increase = 1;
            whichisfocused = 1;
            // 主动刷新
            Album_auto_display_cb();
        }
        else if(obj == a_touming_button2)
        {
            album_data->image_pos_increase = -1;
            whichisfocused = 2;
            // 主动刷新
            Album_auto_display_cb();
        }
    }
}

void album(void)
{   
#if MEM_MONITOR
    memory_print();
#endif

    album_group = lv_group_create();
    lv_indev_set_group(indev_encoder, album_group);

    scr_album = lv_scr_act();
    lv_obj_clean(scr_album);

    // Serial.println(cfgstruct.Autoflush);

    album_init();

    snprintf(file_name, PIC_FILENAME_MAX_LEN, "%s/%s", album_data->image_file->file_name, album_data->pfile->file_name);

    Serial.print(F("Decode image: "));
    Serial.println(file_name);
    
    if (NULL != strstr(file_name, ".bin") || NULL != strstr(file_name, ".BIN"))
    {
        // 使用LVGL的bin格式的图片
        display_photo(file_name);
    }

    if(cfgstruct.Autoflush)
    {
        // 自动滚动照片
        timer_album = timerBegin(1,80,true);
        timerAttachInterrupt(timer_album, Album_auto_display_cb, true);
        timerAlarmWrite(timer_album, AUTO_DISPLAY_INTERVAL*1000000, true);
        timerAlarmEnable(timer_album);
    }
}