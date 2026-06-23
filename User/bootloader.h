/*********************************************************
 * @File Name: bootloader.h
 * @Description: 函数初始化、跳转函数定义
 * @Author: WANG LEI
 * @Create Date: 2026-06-22
 * @Modify Record: 无
 * @Version: V1.0
 * @Copyright: 本文件为项目自研开源文件，禁止私自篡改传播
**********************************************************/

#ifndef BOOTLOADER_H
#define BOOTLOADER_H

#include "mm32_device.h"
#include <stdint.h>
#include <stdio.h>

void BOOT_InitConsole(uint32_t Baudrate);
void BOOTLOAD2APP(void);

#endif
