/*********************************************************
 * @File Name: can.h
 * @Description: 串口驱动对外接口声明、宏定义、数据类型定义
 * @Author: WANG LEI
 * @Create Date: 2026-06-22
 * @Modify Record: 无
 * @Version: V1.0
 * @Copyright: 本文件为项目自研开源文件，禁止私自篡改传播
**********************************************************/

#ifndef __CAN_H
#define __CAN_H

#include "hal_device.h"
#include "hal_flexcan.h"

#define CAN_EXT_RX_PREFIX_BASE  0x18000000U
#define CAN_EXT_RX_PREFIX_MASK  0x1F000000U
#define CAN_STD_RX_ID_101       0x101U
#define CAN_STD_TX_ID_141       0x141U

#define CAN_BAUDRATE            250000U
#define FLEX_CAN1_CLK_SOURCE    (Enum_Flexcan_ClkSrc0)

#define RX_MESSAGE_BUFFER_NUM   (9U)
#define TX_MESSAGE_BUFFER_NUM1  (8U)
#define TX_MESSAGE_BUFFER_NUM2  (10U)

extern u8 can_data_ready;
extern u16 can_error_cnt;
extern u32 can_fifo_filter_table[2];
extern flexcan_frame_t RxMessage;
extern flexcan_frame_t TxMessage;

void CAN_Configuration(void);
void CAN_Build_Rx_Filter_Table(void);
void FLEX_CAN_IRQHandler(void);
flexcan_frame_t* CAN_Rd_Msg(void);
u32 CAN_SendStd141Frame(u8 data0, u8 data1, u8 data2, u8 data3,
                        u8 data4, u8 data5, u8 data6, u8 data7);
void Can_TxMessage_Test(void);

#endif /* __CAN_H */
