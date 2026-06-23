/*********************************************************
 * @File Name: can.c
 * @Description: 串口初始化、can数据接收、中断处理功能实现
 * @Author: XXX
 * @Create Date: 2026-06-22
 * @Modify Record: 
 * @Version: V1.0
 * @Copyright: 本文件为项目自研开源文件，禁止私自篡改传播
**********************************************************/

#include "can.h"

#include <stdio.h>
#include "hal_gpio.h"
#include "hal_misc.h"
#include "hal_rcc.h"

u8 can_data_ready = 0;
u16 can_error_cnt = 0;
u32 can_fifo_filter_table[2];

flexcan_frame_t RxMessage;
flexcan_frame_t TxMessage;

static u32 Can_Get_Rx_Id(const flexcan_frame_t* frame)
{
    if (frame->format == Enum_Flexcan_FrameFormatExtend)
    {
        return frame->id;
    }

    return (frame->id >> CAN_ID_STD_SHIFT) & 0x7FFU;
}

static u8 Can_Is_Ext_18_Frame(const flexcan_frame_t* frame)
{
    return (frame->format == Enum_Flexcan_FrameFormatExtend) &&
           ((Can_Get_Rx_Id(frame) & CAN_EXT_RX_PREFIX_MASK) == CAN_EXT_RX_PREFIX_BASE);
}

void CAN_Build_Rx_Filter_Table(void)
{
    can_fifo_filter_table[0] = FLEXCAN_RX_FIFO_EXT_FILTER_TYPE_A(CAN_EXT_RX_PREFIX_BASE, 0, 1);
    can_fifo_filter_table[1] = FLEXCAN_RX_FIFO_STD_FILTER_TYPE_A(CAN_STD_RX_ID_101, 0, 0);
}

void CAN_Configuration(void)
{
    u32 apb1_clk;
    flexcan_config_t flexcanConfig;
    flexcan_rx_fifo_config_t rxFifoConfig;
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1ENR_CAN, ENABLE);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_3);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_3);

    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    apb1_clk = RCC_GetPCLK1Freq();
    FLEXCAN_GetDefaultConfig(&flexcanConfig);
    flexcanConfig.clkSrc = FLEX_CAN1_CLK_SOURCE;
    flexcanConfig.baudRate = CAN_BAUDRATE;
    flexcanConfig.enableLoopBack = false;
    flexcanConfig.enableIndividMask = true;
    FLEXCAN_Init(FLEX_CAN1, &flexcanConfig, apb1_clk);

    FLEXCAN_SetTxMbConfig(FLEX_CAN1, TX_MESSAGE_BUFFER_NUM1, true);
    FLEXCAN_EnableMbInterrupts(FLEX_CAN1, 1U << TX_MESSAGE_BUFFER_NUM1);

    FLEXCAN_SetTxMbConfig(FLEX_CAN1, TX_MESSAGE_BUFFER_NUM2, true);
    FLEXCAN_EnableMbInterrupts(FLEX_CAN1, 1U << TX_MESSAGE_BUFFER_NUM2);

    CAN_Build_Rx_Filter_Table();
    rxFifoConfig.idFilterTable = can_fifo_filter_table;
    rxFifoConfig.idFilterNum = 2;
    rxFifoConfig.idFilterType = Enum_Flexcan_RxFifoFilterTypeA;
    rxFifoConfig.priority = Enum_Flexcan_RxFifoPrioHigh;
    FLEXCAN_SetRxFifoConfig(FLEX_CAN1, &rxFifoConfig, true);

    // Filter0: match all extended IDs in 0x18000000 ~ 0x18FFFFFF.
    FLEXCAN_SetRxIndividualMask(FLEX_CAN1, 0,
        FLEXCAN_RX_FIFO_EXT_MASK_TYPE_A(CAN_EXT_RX_PREFIX_MASK, 0, 1));

    // Filter1: match only standard ID 0x101.
    FLEXCAN_SetRxIndividualMask(FLEX_CAN1, 1,
        FLEXCAN_RX_FIFO_STD_MASK_TYPE_A(0x7FFU, 0, 1));

    FLEXCAN_EnableMbInterrupts(FLEX_CAN1,
        (u32)Enum_Flexcan_RxFifoOverflowFlag |
        (u32)Enum_Flexcan_RxFifoWarningFlag |
        (u32)Enum_Flexcan_RxFifoFrameAvlFlag);

    FLEXCAN_EnableInterrupts(FLEX_CAN1,
        Enum_Flexcan_ErrorInterruptEnable |
        Enum_Flexcan_BusOffInterruptEnable);

    NVIC_InitStructure.NVIC_IRQChannel = FLEX_CAN_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void FLEX_CAN_IRQHandler(void)
{
    if (0U != FLEXCAN_GetMbStatusFlags(FLEX_CAN1, (u32)Enum_Flexcan_RxFifoFrameAvlFlag))
    {
        (void)FLEXCAN_ReadRxFifo(FLEX_CAN1, &RxMessage);
        FLEXCAN_ClearMbStatusFlags(FLEX_CAN1, (u32)Enum_Flexcan_RxFifoFrameAvlFlag);
        can_data_ready = 1;
    }

    if (0U != FLEXCAN_GetMbStatusFlags(FLEX_CAN1, (u32)Enum_Flexcan_RxFifoWarningFlag))
    {
        FLEXCAN_ClearMbStatusFlags(FLEX_CAN1, (u32)Enum_Flexcan_RxFifoWarningFlag);
    }

    if (0U != FLEXCAN_GetMbStatusFlags(FLEX_CAN1, (u32)Enum_Flexcan_RxFifoOverflowFlag))
    {
        FLEXCAN_ClearMbStatusFlags(FLEX_CAN1, (u32)Enum_Flexcan_RxFifoOverflowFlag);
        can_error_cnt++;
    }

    if (0U != FLEXCAN_GetMbStatusFlags(FLEX_CAN1, 1U << TX_MESSAGE_BUFFER_NUM1))
    {
        FLEXCAN_ClearMbStatusFlags(FLEX_CAN1, 1U << TX_MESSAGE_BUFFER_NUM1);
    }

    if (0U != FLEXCAN_GetMbStatusFlags(FLEX_CAN1, 1U << TX_MESSAGE_BUFFER_NUM2))
    {
        FLEXCAN_ClearMbStatusFlags(FLEX_CAN1, 1U << TX_MESSAGE_BUFFER_NUM2);
    }

    if (0U != (FLEXCAN_GetStatusFlags(FLEX_CAN1) & Enum_Flexcan_ErrorIntFlag))
    {
        FLEXCAN_ClearStatusFlags(FLEX_CAN1, Enum_Flexcan_ErrorIntFlag);
        CAN_Configuration();
    }

    if (0U != (FLEXCAN_GetStatusFlags(FLEX_CAN1) & Enum_Flexcan_BusOffInterruptEnable))
    {
        FLEXCAN_ClearStatusFlags(FLEX_CAN1, Enum_Flexcan_BusOffInterruptEnable);
        CAN_Configuration();
    }

    __DSB();
}

flexcan_frame_t* CAN_Rd_Msg(void)
{
    if (can_data_ready == 1)
    {
        can_data_ready = 0;
        return &RxMessage;
    }

    return NULL;
}

u32 CAN_SendStd141Frame(u8 data0, u8 data1, u8 data2, u8 data3,
                        u8 data4, u8 data5, u8 data6, u8 data7)
{
    u32 tx_status;

    TxMessage.format = Enum_Flexcan_FrameFormatStandard;
    TxMessage.type = Enum_Flexcan_FrameTypeData;
    TxMessage.id = FLEXCAN_ID_STD(CAN_STD_TX_ID_141);
    TxMessage.length = 8;

    TxMessage.dataByte0 = data0;
    TxMessage.dataByte1 = data1;
    TxMessage.dataByte2 = data2;
    TxMessage.dataByte3 = data3;
    TxMessage.dataByte4 = data4;
    TxMessage.dataByte5 = data5;
    TxMessage.dataByte6 = data6;
    TxMessage.dataByte7 = data7;

    tx_status = FLEXCAN_WriteTxMb(FLEX_CAN1, TX_MESSAGE_BUFFER_NUM1, &TxMessage);
    if (tx_status != Status_Flexcan_Success)
    {
        printf("\r\n[CAN TX] send fail, status=%lu", (unsigned long)tx_status);
        FLEXCAN_SetTxMbConfig(FLEX_CAN1, TX_MESSAGE_BUFFER_NUM1, true);
    }

    return tx_status;
}

void Can_TxMessage_Test(void)
{
    (void)CAN_SendStd141Frame(0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88);
}
