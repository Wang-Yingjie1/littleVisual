/*
 * @Author: WYJ
 * @Date: 2022-03-25 23:00:18
 * @LastEditors: WYJ
 * @LastEditTime: 2022-04-03 14:52:57
 * @FilePath: \littleVisual\src\app\album\album.h
 * @Description: 
 */

#ifndef ALBUM_H
#define ALBUM_H

#include "driver/sd_card.h"

#define IMAGE_PATH "/image"
#define AUTO_DISPLAY_INTERVAL   5    // s
/**
 * @description: 相册数据结构，借用B站UP主 @溜马小哥 AIO框架中的结构
 * @param {*}
 * @return {*}
 */
struct AlbumStructure
{
    // unsigned long pic_perMillis;      // 图片上一回更新的时间
    // unsigned long picRefreshInterval; // 图片播放的时间间隔(10s)

    File_Info *image_file;      // movie文件夹下的文件指针头
    File_Info *pfile;           // 指向当前播放的文件节点
    int image_pos_increase = 1; // 文件的遍历方向
};

#define PIC_FILENAME_MAX_LEN 100

void album(void);

#endif