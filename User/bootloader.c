/*********************************************************
 * @File Name: bootloader.c
 * @Description: 串口初始化、核心跳转函数收功能实现
 * @Author: XXX
 * @Create Date: 2026-06-22
 * @Modify Record: 
 * @Version: V1.0
 * @Copyright: 本文件为项目自研开源文件，禁止私自篡改传播
**********************************************************/

#include "bootloader.h"
#include "hal_conf.h"

// ---------------------------------------------------------
// 1. 基础定义与函数指针
// ---------------------------------------------------------
#define APP_START_ADDR 0x08004000  // 必须与 APP 工程的 IROM1 起始地址一致

typedef void (*pFunction)(void);

// ---------------------------------------------------------
// 2. 串口初始化与重定向
// ---------------------------------------------------------

// 重定向 fputc 函数，使得 printf 能够输出到 UART1
int fputc(int ch, FILE *f)
{
    UART_SendData(UART1, (uint8_t)ch);
    // 等待发送完成
    while (RESET == UART_GetFlagStatus(UART1, UART_FLAG_TXEPT))
    {
    }
    return (ch);
}

// 初始化 UART1 (PA9 作为 TX)
void BOOT_InitConsole(uint32_t Baudrate)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    UART_InitTypeDef UART_InitStruct;

    // 1. 开启 UART1 和 GPIOA 时钟
    RCC_APB2PeriphClockCmd(RCC_APB2ENR_UART1, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOA, ENABLE);

    // 2. 配置 PA9 为复用推挽输出 (UART1_TX)
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);

    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_9;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    // 3. 配置 UART1 参数
    UART_StructInit(&UART_InitStruct);
    UART_InitStruct.BaudRate      = Baudrate;
    UART_InitStruct.WordLength    = UART_WordLength_8b;
    UART_InitStruct.StopBits      = UART_StopBits_1;
    UART_InitStruct.Parity        = UART_Parity_No;
    UART_InitStruct.HWFlowControl = UART_HWFlowControl_None;
    UART_InitStruct.Mode          = UART_Mode_Tx;
    UART_Init(UART1, &UART_InitStruct);

    // 4. 使能 UART1
    UART_Cmd(UART1, ENABLE);
}

// ---------------------------------------------------------
// 3. 核心跳转函数
//						APP跳转BOOT
// ---------------------------------------------------------
void BOOTLOAD2APP(void) {
    uint32_t *pFlash = (uint32_t*)APP_START_ADDR;

    // 检查 APP 起始地址的内容是否合法
    // 判断栈顶指针是否指向 SRAM 范围内 (0x2000xxxx)
    if ((*pFlash & 0x2FFE0000) == 0x20000000) {
        
        // --- [关键：环境清理] ---
        __disable_irq(); // 1. 彻底关闭总中断

        // 2. 彻底关闭 SysTick 滴答定时器，防止跳转后触发 Bootloader 的中断导致死机
        SysTick->CTRL = 0;
        SysTick->LOAD = 0;
        SysTick->VAL  = 0;

        // 3. 关闭所有开启的外设中断，清除挂起标志 (M0内核只有一个ICER/ICPR寄存器)
        NVIC->ICER[0] = 0xFFFFFFFF;
        NVIC->ICPR[0] = 0xFFFFFFFF;

        // 4. 设置主堆栈指针 (MSP) 为 APP 向量表的第一项
        __set_MSP(pFlash[0]);

        // 5. 获取 APP 的复位向量 (Reset_Handler) 并执行跳转
        pFunction JumpToApp = (pFunction)pFlash[1];
        JumpToApp();
    } else {
        // 如果没发现合法的 APP，串口报错
        printf("\r\n[Boot] ERROR: No valid APP found at 0x08004000!");
    }
}