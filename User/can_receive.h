/*********************************************************
 * @File Name: can_receive.h
 * @Description: 串口驱动对外接口声明、宏定义、数据类型定义
 * @Author: WANG LEI
 * @Create Date: 2026-06-22
 * @Modify Record: 无
 * @Version: V1.0
 * @Copyright: 本文件为项目自研开源文件，禁止私自篡改传播
**********************************************************/

#ifndef __CAN_RECEIVE_H
#define __CAN_RECEIVE_H

#include "can.h"

typedef struct
{
    u32 id;
    u8 length;
    u8 data[8];
    u8 is_extended;
} can_rx_msg_t;

void CAN_ReceiveTest_Init(void);
void CAN_ReceiveTest_Task(void);
u8 CAN_ReceiveTest_Read(can_rx_msg_t* msg);
void Can_RxMessage_Test(void);

#endif /* __CAN_RECEIVE_H */
