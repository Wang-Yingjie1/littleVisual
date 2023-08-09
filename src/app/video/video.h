/*
 * @Author: 
 * @Date: 2022-04-02 23:34:58
 * @LastEditors: WYJ
 * @LastEditTime: 2022-04-03 00:02:53
 * @FilePath: \littleVisual\src\app\video\video.h
 * @Description: 
 */

#ifndef VIDEO_H
#define VIDEO_H

#include "mjpeg_docoder.h"
#include "driver/sd_card.h"

#define VIDEO_WIDTH 240L
#define VIDEO_HEIGHT 240L
#define MOVIE_PATH "/movie"

struct MediaAppRunData
{
    PlayDocoderBase *player_docoder;
    unsigned long preTriggerKeyMillis; // 最近一回按键触发的时间戳
    int movie_pos_increate;
    File_Info *movie_file; // movie文件夹下的文件指针头
    File_Info *pfile;      // 指向当前播放的文件节点
    File file;
};


void video(void);


#endif