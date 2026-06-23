#ifndef __CAN_H
#define __CAN_H

#include "hal_flexcan.h"

// 接收宏定义
#define CAN_EXT_RX_PREFIX_BASE  0x18000000U
#define CAN_EXT_RX_PREFIX_MASK  0x1F000000U
#define CAN_STD_RX_ID_101       0x101U

#define CAN_BAUDRATE  250000

#define RX_MESSAGE_BUFFER_NUM   (9)
#define TX_MESSAGE_BUFFER_NUM1  (8)
#define TX_MESSAGE_BUFFER_NUM2  (10)

// 全局变量声明
extern u8 can_data_ready;
extern u16 can_error_cnt;
extern u32 can_fifo_filter_table[2];
extern flexcan_frame_t RxMessage;
extern flexcan_frame_t TxMessage;

// 函数声明
void CAN_Build_Rx_Filter_Table(void);
void FLEX_CAN_IRQHandler(void);
flexcan_frame_t* CAN_Rd_Msg(void);

#endif /* __CAN_H */
