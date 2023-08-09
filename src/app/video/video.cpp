/*
 * @Author: @ClimbSnail
 * @Date: 2022-04-02 23:34:52
 * @LastEditors: WYJ
 * @LastEditTime: 2022-04-03 13:36:59
 * @FilePath: \littleVisual\src\app\video\video.cpp
 * @Description: 
 */

#include "video.h"
#include "lvgl.h"
#include "driver/sd_card.h"
#include "mjpeg_docoder.h"
#include "DMADrawer.h"

extern SdCard tf;
extern lv_indev_t * indev_encoder;

static MediaAppRunData *video_data = NULL;
static hw_timer_t *timer_video = NULL;

static lv_group_t * video_group;
static lv_obj_t * scr_video;


static void video_player_init(void);
static File_Info *get_next_file(File_Info *p_cur_file, int direction);
static bool video_start(bool create_new);
static void IRAM_ATTR Video_display(void);
static void release_player_docoder(void);



void video(void)
{
    video_group = lv_group_create();
    lv_indev_set_group(indev_encoder, video_group);

    scr_video = lv_scr_act();
    lv_obj_clean(scr_video);

    video_player_init();

    // 创建播放
    video_start(false);

    timer_video = timerBegin(1,80,true);
    timerAttachInterrupt(timer_video, Video_display, true);
    timerAlarmWrite(timer_video, 50*1000, true);
    timerAlarmEnable(timer_video);

}

static void video_player_init(void)
{
    video_data = (MediaAppRunData *)calloc(1, sizeof(MediaAppRunData));
    video_data->player_docoder = NULL;
    video_data->movie_pos_increate = 1;
    video_data->movie_file = NULL; // movie文件夹下的文件指针头
    video_data->pfile = NULL;      // 指向当前播放的文件节点
    video_data->preTriggerKeyMillis = millis();

    video_data->movie_file = tf.listDir(MOVIE_PATH);
    if (NULL != video_data->movie_file)
    {
        video_data->pfile = get_next_file(video_data->movie_file->next_node, 1);
    }


}


static File_Info *get_next_file(File_Info *p_cur_file, int direction)
{
    // 得到 p_cur_file 的下一个 类型为FILE_TYPE_FILE 的文件（即下一个非文件夹文件）
    if (NULL == p_cur_file)
    {
        return NULL;
    }

    File_Info *pfile = direction == 1 ? p_cur_file->next_node : p_cur_file->front_node;
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

static bool video_start(bool create_new)
{
    if (NULL == video_data->pfile)
    {
        // 视频文件夹空 就跳出去
        return false;
    }

    if (true == create_new)
    {
        video_data->pfile = get_next_file(video_data->pfile, video_data->movie_pos_increate);
    }

    char file_name[FILENAME_MAX_LEN] = {0};
    snprintf(file_name, FILENAME_MAX_LEN, "%s/%s", video_data->movie_file->file_name, video_data->pfile->file_name);

    video_data->file = SD.open(file_name);
    if (NULL != strstr(video_data->pfile->file_name, ".mjpeg") || NULL != strstr(video_data->pfile->file_name, ".MJPEG"))
    {
        // 直接解码mjpeg格式的视频
        video_data->player_docoder = new MjpegPlayDocoder(&video_data->file, true);
        Serial.print(F("MJPEG video start --------> "));
    }

    Serial.println(file_name);
    return true;
}

static void IRAM_ATTR Video_display(void)
{
    if (!video_data->file)
    {
        Serial.println(F("Failed to open file for reading"));
        return;
    }
    
    if (video_data->file.available())
    {
        // 播放一帧数据
        video_data->player_docoder->video_play_screen();
    }
    else
    {
        // 结束播放
        release_player_docoder();
        video_data->file.close();
        
        // 创建播放(播放下一个)
        video_start(true);
    }

}

static void release_player_docoder(void)
{
    // 释放具体的播放对象
    if (NULL != video_data->player_docoder)
    {
        delete video_data->player_docoder;
        video_data->player_docoder = NULL;
    }
}