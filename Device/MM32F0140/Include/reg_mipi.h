////////////////////////////////////////////////////////////////////////////////
/// @file     reg_mipi.h
/// @author   AE TEAM
/// @brief    THIS FILE CONTAINS ALL THE FUNCTIONS PROTOTYPES FOR THE SERIES OF
///           MM32 FIRMWARE LIBRARY.
////////////////////////////////////////////////////////////////////////////////
/// @attention
///
/// THE EXISTING FIRMWARE IS ONLY FOR REFERENCE, WHICH IS DESIGNED TO PROVIDE
/// CUSTOMERS WITH CODING INFORMATION ABOUT THEIR PRODUCTS SO THEY CAN SAVE
/// TIME. THEREFORE, MINDMOTION SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT OR
/// CONSEQUENTIAL DAMAGES ABOUT ANY CLAIMS ARISING OUT OF THE CONTENT OF SUCH
/// HARDWARE AND/OR THE USE OF THE CODING INFORMATION CONTAINED HEREIN IN
/// CONNECTION WITH PRODUCTS MADE BY CUSTOMERS.
///
/// <H2><CENTER>&COPY; COPYRIGHT MINDMOTION </CENTER></H2>
////////////////////////////////////////////////////////////////////////////////

// Define to prevent recursive inclusion

#ifndef __REG_MIPI_H
#define __REG_MIPI_H

// Files includes

#include <stdint.h>
#include <stdbool.h>
#include "types.h"




#if defined ( __CC_ARM )
#pragma anon_unions
#endif





/////////////////////////////////////////////////////////////////////////////////

/// @brief MIPI Base Address Definition

/////////////////////////////////////////////////////////////////////////////////

#define MIPI_BASE                        (APB1PERIPH_BASE + 0x00008400)              ///< Base Address: 0x40008400


/////////////////////////////////////////////////////////////////////////////////

/// @brief MIPI register

/////////////////////////////////////////////////////////////////////////////////

typedef struct {

    __IO u32 CR;                                                                  ///< MIPI control register                              offset: 0x00

    __IO u32 CMDR;                                                                ///< MIPI command register                              offset: 0x04

    __IO u32 TDR;                                                                 ///< MIPI send data register                            offset: 0x08

    __IO u32 Reserved;                                                            ///< reserved                                           offset: 0x0C

    __IO u32 SR;                                                                  ///< MIPI current status register                       offset: 0x10

    __IO u32 RDR;                                                                 ///< MIPI receive data register                         offset: 0x14

    __IO u32 MATCH_DR;                                                            ///< MIPI data match register                           offset: 0x18

    __IO u32 FIFOR;                                                               ///< MIPI receive data FIFO register                     offset: 0x1C

    __IO u32 ANOTHER_MATCH_DR;                                                    ///< MIPI another set of data matching registers         offset: 0x20

} MIPI_TypeDef;

/////////////////////////////////////////////////////////////////////////////////

/// @briefMIPI type pointer Definition

/////////////////////////////////////////////////////////////////////////////////

#define MIPI                              ((MIPI_TypeDef*) MIPI_BASE)

/////////////////////////////////////////////////////////////////////////////////

/// @brief MIPI_CR Register Bit Definition

/////////////////////////////////////////////////////////////////////////////////

#define MIPI_CR_ANOTHER_MATCH_INT_EN_Pos         (29)

#define MIPI_CR_ANOTHER_MATCH_INT_EN             (0x01U << MIPI_CR_ANOTHER_MATCH_INT_EN_Pos)                                        ///< another match interrupt enable

#define MIPI_CR_AUTO_BTA_ACK_EN_Pos              (28)

#define MIPI_CR_AUTO_BTA_ACK_EN                  (0x01U << MIPI_CR_AUTO_BTA_ACK_EN_Pos)                                             ///< auto BAT ack enable

#define MIPI_CR_BTA_ACK_EN_Pos                   (27)

#define MIPI_CR_BTA_ACK_EN                       (0x01U << MIPI_CR_BTA_ACK_EN_Pos)                                                  ///< BAT ack enable

#define MIPI_CR_STOP_INT_EN_Pos                  (26)

#define MIPI_CR_STOP_INT_EN                      (0x01U << MIPI_CR_STOP_INT_EN_Pos)                                                 ///< stop interrupt enable

#define MIPI_CR_IGNORE_BTA_Pos                   (25)

#define MIPI_CR_IGNORE_BTA                       (0x01U << MIPI_CR_IGNORE_BTA_Pos)                                                  ///< ignore BAT 

#define MIPI_CR_MIPI_EN_Pos                      (24)

#define MIPI_CR_MIPI_EN                          (0x01U << MIPI_CR_MIPI_EN_Pos)                                                     ///< MIPI enable

#define MIPI_CR_ESC_INT_EN_Pos                   (23)

#define MIPI_CR_ESC_INT_EN                       (0x01U << MIPI_CR_ESC_INT_EN_Pos)                                                  ///< Escape receive interrupt enable

#define MIPI_CR_MATCH_INT_EN_Pos                 (22)

#define MIPI_CR_MATCH_INT_EN                     (0x01U << MIPI_CR_MATCH_INT_EN_Pos)                                                ///< data match interrupt enable

#define MIPI_CR_DN_COMP_HYSCTRL_Pos              (20)

#define MIPI_CR_DN_COMP_HYSCTRL                  (0x 3U << MIPI_CR_DN_COMP_HYSCTRL_Pos)                                             ///< DN comparator hysteresis voltage control 

#define MIPI_CR_DP_COMP_HYSCTRL_Pos              (18)

#define MIPI_CR_DP_COMP_HYSCTRL_0                (0x00U << MIPI_CR_DP_COMP_HYSCTRL_Pos)                                            ///< select 0 mv
#define MIPI_CR_DP_COMP_HYSCTRL_1                (0x01U << MIPI_CR_DP_COMP_HYSCTRL_Pos)                                            ///< select 15 mv
#define MIPI_CR_DP_COMP_HYSCTRL_2                (0x02U << MIPI_CR_DP_COMP_HYSCTRL_Pos)                                            ///< select 30 mv
#define MIPI_CR_DP_COMP_HYSCTRL_3                (0x03U << MIPI_CR_DP_COMP_HYSCTRL_Pos)                                            ///< select 90 mv

#define MIPI_CR_DN_COMP_EN_Pos                   (17)

#define MIPI_CR_DN_COMP_EN                       (0x01U << MIPI_CR_DN_COMP_EN_Pos)                                                  ///< DN comparator enable

#define MIPI_CR_DP_COMP_EN_Pos                   (16)

#define MIPI_CR_DP_COMP_EN                       (0x01U << MIPI_CR_DP_COMP_EN_Pos)                                                  ///< DP comparator enable

#define MIPI_CR_DN_MUX_EN_Pos                    (15)

#define MIPI_CR_DN_MUX_EN                        (0x01U << MIPI_CR_DN_MUX_EN_Pos)                                                   ///< DN input select

#define MIPI_CR_DP_MUX_EN_Pos                    (14)

#define MIPI_CR_DP_MUX_EN                        (0x01U << MIPI_CR_DP_MUX_EN_Pos)                                                   ///< DP input select

#define MIPI_CR_RX_DMA_EN_Pos                    (13)

#define MIPI_CR_RX_DMA_EN                        (0x01U << MIPI_CR_RX_DMA_EN_Pos)                                                   ///< receive DMA enable

#define MIPI_CR_TX_DMA_EN_Pos                    (12)

#define MIPI_CR_TX_DMA_EN                        (0x01U << MIPI_CR_TX_DMA_EN_Pos)                                                   ///< send DMA enable 

#define MIPI_CR_DRIVE_TIMER_Pos                  (8)

#define MIPI_CR_DRIVE_TIMER                      (0x FU << MIPI_CR_DRIVE_TIMER_Pos)                                                 ///< drive timer select

#define MIPI_CR_GLITCH_REMOVE_EN_Pos             (7)

#define MIPI_CR_GLITCH_REMOVE_EN                 (0x01U << MIPI_CR_GLITCH_REMOVE_EN_Pos)                                            ///< DP/DN burr removal enable

#define MIPI_CR_BTA_SEND_INT_EN_Pos              (6)

#define MIPI_CR_BTA_SEND_INT_EN                  (0x01U << MIPI_CR_BTA_SEND_INT_EN_Pos)                                             ///< BTA signal transmission interrupt enable

#define MIPI_CR_BTA_ACK_INT_EN_Pos               (5)

#define MIPI_CR_BTA_ACK_INT_EN                   (0x01U << MIPI_CR_BTA_ACK_INT_EN_Pos)                                              ///< BTA signal response interrupt enable

#define MIPI_CR_DATA_SEND_INT_EN_Pos             (4)

#define MIPI_CR_DATA_SEND_INT_EN                 (0x01U << MIPI_CR_DATA_SEND_INT_EN_Pos)                                            ///< data transmission interrupt enable

#define MIPI_CR_DATA_RECV_INT_EN_Pos             (3)

#define MIPI_CR_DATA_RECV_INT_EN                 (0x01U << MIPI_CR_DATA_RECV_INT_EN_Pos)                                            ///< data receive interrupt enable

#define MIPI_CR_INT_EN_Pos                       (2)

#define MIPI_CR_INT_EN                           (0x01U << MIPI_CR_INT_EN_Pos)                                                      ///< MIPI interrupt enable

#define MIPI_CR_TA_GET_Pos                       (0)

#define MIPI_CR_TA_GET                           (0x 3U << MIPI_CR_TA_GET_Pos)                                                       ///< The time interval parameter of the drive response

/////////////////////////////////////////////////////////////////////////////////

/// @brief MIPI_CMDR Register Bit Definition

/////////////////////////////////////////////////////////////////////////////////

#define MIPI_CMDR_TRIGGER_IDLE_Pos               (7)

#define MIPI_CMDR_TRIGGER_IDLE                   (0x01U << MIPI_CMDR_TRIGGER_IDLE_Pos)                                              ///< jump to IDLE state

#define MIPI_CMDR_TRIGGER_STOP_Pos               (6)

#define MIPI_CMDR_TRIGGER_STOP                   (0x01U << MIPI_CMDR_TRIGGER_STOP_Pos)                                              ///< trigger a stop signal

#define MIPI_CMDR_TRIGGER_DATA_Pos               (1)

#define MIPI_CMDR_TRIGGER_DATA                   (0x01U << MIPI_CMDR_TRIGGER_DATA_Pos)                                              ///< trigger a escape signal

#define MIPI_CMDR_TRIGGER_BTA_Pos                (0)

#define MIPI_CMDR_TRIGGER_BTA                    (0x01U << MIPI_CMDR_TRIGGER_BTA_Pos)                                               ///< trigger a BAT signal

/////////////////////////////////////////////////////////////////////////////////

/// @brief MIPI_TDR Register Bit Definition

/////////////////////////////////////////////////////////////////////////////////

#define MIPI_TDR_TXREG_Pos                       (0)

#define MIPI_TDR_TXREG                           (0xFFU << MIPI_TDR_TXREG_Pos)                                                      ///< Transmit data register

/////////////////////////////////////////////////////////////////////////////////

/// @brief MIPI_SR Register Bit Definition

/////////////////////////////////////////////////////////////////////////////////

#define MIPI_SR_ANOTHER_MATCH_FAIL_Pos           (9)

#define MIPI_SR_ANOTHER_MATCH_FAIL               (0x01U << MIPI_SR_ANOTHER_MATCH_FAIL_Pos)                                          ///< another match failed

#define MIPI_SR_ANOTHER_MATCH_Pos                (8)

#define MIPI_SR_ANOTHER_MATCH                    (0x01U << MIPI_SR_ANOTHER_MATCH_Pos)                                               ///< another match successful

#define MIPI_SR_STOP_DETCT_Pos                   (7)

#define MIPI_SR_STOP_DETCT                       (0x01U << MIPI_SR_STOP_DETCT_Pos)                                                  ///< stop signal detct

#define MIPI_SR_MATCH_FAIL_Pos                   (6)

#define MIPI_SR_MATCH_FAIL                       (0x01U << MIPI_SR_MATCH_FAIL_Pos)                                                  ///< data matching failed

#define MIPI_SR_ESC_DETCT_Pos                    (5)

#define MIPI_SR_ESC_DETCT                        (0x01U << MIPI_SR_ESC_DETCT_Pos)                                                   ///< detct escape  signal

#define MIPI_SR_MATCH_Pos                        (4)

#define MIPI_SR_MATCH                            (0x01U << MIPI_SR_MATCH_Pos)                                                       ///< data match success

#define MIPI_SR_BTA_SEND_Pos                     (3)

#define MIPI_SR_BTA_SEND                         (0x01U << MIPI_SR_BTA_SEND_Pos)                                                    ///< BAT signal send

#define MIPI_SR_BTA_ACK_Pos                      (2)

#define MIPI_SR_BTA_ACK                          (0x01U << MIPI_SR_BTA_ACK_Pos)                                                     ///< BAT signal acked

#define MIPI_SR_DATA_SEND_Pos                    (1)

#define MIPI_SR_DATA_SEND                        (0x01U << MIPI_SR_DATA_SEND_Pos)                                                   ///< data send

#define MIPI_SR_DATA_RECV_Pos                    (0)

#define MIPI_SR_DATA_RECV                        (0x01U << MIPI_SR_DATA_RECV_Pos)                                                   ///< data received

/////////////////////////////////////////////////////////////////////////////////

/// @brief MIPI_RDR Register Bit Definition

/////////////////////////////////////////////////////////////////////////////////

#define MIPI_RDR_RXREG_Pos                       (0)

#define MIPI_RDR_RXREG                           (0xFFU << MIPI_RDR_RXREG_Pos)                                                      ///< Receive data register

/////////////////////////////////////////////////////////////////////////////////

/// @brief MIPI_MATCH_DR Register Bit Definition

/////////////////////////////////////////////////////////////////////////////////

#define MIPI_MATCH_DR_MATCH_NUMBER_Pos           (8)

#define MIPI_MATCH_DR_MATCH_NUMBER               (0xFFU << MIPI_MATCH_DR_MATCH_NUMBER_Pos)                                          ///< preset match number

#define MIPI_MATCH_DR_MATCH_DATA_Pos             (0)

#define MIPI_MATCH_DR_MATCH_DATA                 (0xFFU << MIPI_MATCH_DR_MATCH_DATA_Pos)                                            ///<preset match data

/////////////////////////////////////////////////////////////////////////////////

/// @brief MIPI_FIFOR Register Bit Definition

/////////////////////////////////////////////////////////////////////////////////

#define MIPI_FIFOR_FIFO_LEVEL_Pos                (3)

#define MIPI_FIFOR_FIFO_LEVEL                    (0x FU << MIPI_FIFOR_FIFO_LEVEL_Pos)                                               ///< FIFO level 

#define MIPI_FIFOR_FIFO_FULL_Pos                 (2)

#define MIPI_FIFOR_FIFO_FULL                     (0x01U << MIPI_FIFOR_FIFO_FULL_Pos)                                                ///<receive FIFO full flag

#define MIPI_FIFOR_FIFO_EMPTY_Pos                (1)

#define MIPI_FIFOR_FIFO_EMPTY                    (0x01U << MIPI_FIFOR_FIFO_EMPTY_Pos)                                               ///< receive FIFO empty flag

#define MIPI_FIFOR_FIFO_EN_Pos                   (0)

#define MIPI_FIFOR_FIFO_EN                       (0x01U << MIPI_FIFOR_FIFO_EN_Pos)                                                  ///< receive FIFO enable

/////////////////////////////////////////////////////////////////////////////////

/// @brief MIPI_ANOTHER_MATCH_DR Register Bit Definition

/////////////////////////////////////////////////////////////////////////////////

#define MIPI_ANOTHER_MATCH_DR_ANOTHER_MATCH_NUMBER_Pos  (8)

#define MIPI_ANOTHER_MATCH_DR_ANOTHER_MATCH_NUMBER      (0xFFU << MIPI_ANOTHER_MATCH_DR_ANOTHER_MATCH_NUMBER_Pos)                    ///< another preset match number

#define MIPI_ANOTHER_MATCH_DR_ANOTHER_MATCH_DATA_Pos    (0)

#define MIPI_ANOTHER_MATCH_DR_ANOTHER_MATCH_DATA        (0xFFU << MIPI_ANOTHER_MATCH_DR_ANOTHER_MATCH_DATA_Pos)                    ///< another preset match data  

/// @}

/// @}

/// @}

////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
