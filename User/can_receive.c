/*********************************************************
 * @File Name: can_receive.c
 * @Description:can发送数据实现
 * @Author: WANG LEI
 * @Create Date: 2026-06-22
 * @Modify Record: 
 * @Version: V1.0
 * @Copyright: 本文件为项目自研开源文件，禁止私自篡改传播
**********************************************************/

#include "can_receive.h"

#include <stdio.h>

static can_rx_msg_t g_can_rx_msg;
static u8 g_can_rx_ready = 0;

static u32 CAN_Receive_GetId(const flexcan_frame_t* frame)
{
    if (frame->format == Enum_Flexcan_FrameFormatExtend)
    {
        return frame->id;
    }

    return (u32)((frame->id >> CAN_ID_STD_SHIFT) & 0x7FFU);
}

static void CAN_Receive_Save(const flexcan_frame_t* frame)
{
    g_can_rx_msg.id = CAN_Receive_GetId(frame);
    g_can_rx_msg.length = frame->length;
    g_can_rx_msg.is_extended = (frame->format == Enum_Flexcan_FrameFormatExtend) ? 1U : 0U;

    g_can_rx_msg.data[0] = frame->dataByte0;
    g_can_rx_msg.data[1] = frame->dataByte1;
    g_can_rx_msg.data[2] = frame->dataByte2;
    g_can_rx_msg.data[3] = frame->dataByte3;
    g_can_rx_msg.data[4] = frame->dataByte4;
    g_can_rx_msg.data[5] = frame->dataByte5;
    g_can_rx_msg.data[6] = frame->dataByte6;
    g_can_rx_msg.data[7] = frame->dataByte7;
}

void CAN_ReceiveTest_Init(void)
{
    u8 i;

    g_can_rx_msg.id = 0;
    g_can_rx_msg.length = 0;
    g_can_rx_msg.is_extended = 0;

    for (i = 0; i < 8; i++)
    {
        g_can_rx_msg.data[i] = 0;
    }

    g_can_rx_ready = 0;
}

void CAN_ReceiveTest_Task(void)
{
    flexcan_frame_t* frame;
    u32 id;

    frame = CAN_Rd_Msg();
    if (frame == NULL)
    {
        return;
    }

    id = CAN_Receive_GetId(frame);

    if ((frame->format == Enum_Flexcan_FrameFormatStandard) && (id == CAN_STD_RX_ID_101))
    {
        CAN_Receive_Save(frame);
        g_can_rx_ready = 1;

        printf("\r\n[CAN RX] STD 0x101 DLC=%u DATA=%02X %02X %02X %02X %02X %02X %02X %02X",
               g_can_rx_msg.length,
               g_can_rx_msg.data[0],
               g_can_rx_msg.data[1],
               g_can_rx_msg.data[2],
               g_can_rx_msg.data[3],
               g_can_rx_msg.data[4],
               g_can_rx_msg.data[5],
               g_can_rx_msg.data[6],
               g_can_rx_msg.data[7]);
        (void)CAN_SendStd141Frame(g_can_rx_msg.data[0],
                                  g_can_rx_msg.data[1],
                                  g_can_rx_msg.data[2],
                                  g_can_rx_msg.data[3],
                                  g_can_rx_msg.data[4],
                                  g_can_rx_msg.data[5],
                                  g_can_rx_msg.data[6],
                                  g_can_rx_msg.data[7]);
        printf("\r\n[CAN TX] reply STD 0x141 DATA=%02X %02X %02X %02X %02X %02X %02X %02X",
               g_can_rx_msg.data[0],
               g_can_rx_msg.data[1],
               g_can_rx_msg.data[2],
               g_can_rx_msg.data[3],
               g_can_rx_msg.data[4],
               g_can_rx_msg.data[5],
               g_can_rx_msg.data[6],
               g_can_rx_msg.data[7]);
        return;
    }

    if ((frame->format == Enum_Flexcan_FrameFormatExtend) &&
        ((id & CAN_EXT_RX_PREFIX_MASK) == CAN_EXT_RX_PREFIX_BASE))
    {
        CAN_Receive_Save(frame);
        g_can_rx_ready = 1;

        printf("\r\n[CAN RX] EXT 0x%08lX DLC=%u DATA=%02X %02X %02X %02X %02X %02X %02X %02X",
               (unsigned long)id,
               g_can_rx_msg.length,
               g_can_rx_msg.data[0],
               g_can_rx_msg.data[1],
               g_can_rx_msg.data[2],
               g_can_rx_msg.data[3],
               g_can_rx_msg.data[4],
               g_can_rx_msg.data[5],
               g_can_rx_msg.data[6],
               g_can_rx_msg.data[7]);
        (void)CAN_SendStd141Frame(0x11, 0x22, 0x33, 0x44, 0x00, 0x00, 0x00, 0x00);
        printf("\r\n[CAN TX] reply STD 0x141 DATA=11 22 33 44 00 00 00 00");
    }
}

u8 CAN_ReceiveTest_Read(can_rx_msg_t* msg)
{
    if ((msg == NULL) || (g_can_rx_ready == 0U))
    {
        return 0U;
    }

    *msg = g_can_rx_msg;
    g_can_rx_ready = 0;
    return 1U;
}

void Can_RxMessage_Test(void)
{
    CAN_ReceiveTest_Task();
}
