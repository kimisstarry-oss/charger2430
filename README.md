# charger2430_new

面向 MindMotion MM32F0144（Cortex-M0）充电器的 Keil 嵌入式固件工程，工程名为 **Bootloader_CAN**。当前固件实际运行 CAN 通信测试模式；Bootloader 跳转与向量表重映射等能力已实现但未接入主流程。

## 项目定位

| 项目属性 | 说明 |
|---------|------|
| 类型 | 嵌入式裸机固件（无 RTOS） |
| 目标 MCU | MindMotion **MM32F0144C6P**（ARM Cortex-M0，64KB Flash / 8KB SRAM） |
| 工具链 | Keil MDK-ARM（工程 `Keil Project/Bootloader_CAN.uvprojx`） |
| 产品场景 | 充电器 2430 系列，CAN 总线通信 |
| 代码规模 | 用户代码约 5 个文件；ROM 占用约 5KB |

## 目录结构

```
charger2430_new/
├── User/              # 应用层：main、串口、CAN、Bootloader 跳转
├── Device/            # MindMotion CMSIS + HAL + 启动文件
└── Keil Project/      # Keil 工程配置与编译产物
```

- **User/** — 业务逻辑（入口、串口、CAN、Bootloader 跳转）
- **Device/** — MindMotion 官方 CMSIS + HAL + 启动文件
- **Keil Project/** — 工程配置与编译产物（`Objects/Bootloader_CAN.hex`）

## 启动与运行流程

1. 复位 → `startup_mm32f0140_keil.s` → `SystemInit()`（72MHz 时钟）
2. `main()` → `BOOT_InitConsole(115200)` → `CAN_Configuration()`
3. 主循环：周期性调用 `Can_TxMessage_Test()` 发送扩展帧 `0x18FF50E5`

当前 `User/main.c` 只做 CAN 测试，**未调用** `BOOTLOAD2APP()`。

## 已实现功能清单

### 1. 系统时钟（Device 层，已启用）

- **72 MHz 系统时钟**：HSE 8MHz + PLL，定义于 `Device/MM32F0140/Source/system_mm32f0140.c`（`SYSCLK_HSE_XXMHz = 72000000`）
- 上电后由 `SystemInit()` 自动配置

### 2. 调试串口控制台（已实现并启用）

文件：`User/bootloader.c`

| 功能 | 细节 |
|------|------|
| `BOOT_InitConsole()` | 初始化 UART1，TX 引脚 **PA9**，复用 AF1 |
| `fputc()` 重定向 | 使 `printf` 通过 UART1 输出（MicroLib bare printf） |
| 波特率 | 115200，8N1，仅发送模式 |

主程序启动时会打印 Boot 提示信息。

### 3. FlexCAN 驱动与配置（已实现并启用）

文件：`User/can.c`、`User/can.h`

| 功能 | 细节 |
|------|------|
| 引脚配置 | **PA11** = CAN_RX（浮空输入），**PA12** = CAN_TX（复用推挽 AF3） |
| 波特率 | **250 kbps** |
| 时钟源 | `Enum_Flexcan_ClkSrc0` |
| 发送邮箱 | MB8、MB10 配置为 TX，并使能中断 |
| RX FIFO 过滤器 | Filter0：扩展帧 ID 前缀 `0x18xxxxxx`（mask `0x1F000000`）；Filter1：标准帧 ID `0x101` |
| 中断 | NVIC 优先级 1；处理 RX FIFO 有帧/警告/溢出、TX 完成、总线错误、Bus-Off |
| 错误恢复 | 发生 Error/Bus-Off 中断时自动重新调用 `CAN_Configuration()` |
| `Can_TxMessage_Test()` | 发送扩展帧 ID **0x18FF50E5**，8 字节固定载荷 `A1 B2 C3 D4 E5 F6 5A A5` |
| `CAN_Rd_Msg()` | 从中断接收缓冲区读取一帧（设置 `can_data_ready` 标志） |
| `can_error_cnt` | 统计 RX FIFO 溢出次数 |

**主循环行为**：每隔约软件延时循环一次，持续调用 `Can_TxMessage_Test()` 发送测试帧。

### 4. CAN 中断服务（已实现并启用）

- `FLEX_CAN_IRQHandler()` 覆盖启动文件中的弱符号
- 接收帧写入全局 `RxMessage`，置位 `can_data_ready = 1`
- 清除各类 MB/Status 标志；Bus-Off/Error 时重初始化 CAN

> 注意：虽然 RX 中断和 `CAN_Rd_Msg()` 已实现，**main 中未调用读取逻辑**，接收功能处于就绪但未使用状态。

### 5. Bootloader 跳转 APP（已实现，未启用）

文件：`User/bootloader.c`

| 功能 | 细节 |
|------|------|
| APP 起始地址 | `0x08004000`（预留 16KB Boot + 48KB APP 分区） |
| 合法性检查 | 读取 APP 向量表首字，验证栈指针是否在 SRAM 范围（`0x2000xxxx`） |
| 环境清理 | 关中断、清 SysTick、清 NVIC 挂起/使能、设置 MSP |
| 跳转 | 取 APP Reset_Handler 地址并执行 |
| 失败处理 | 串口打印 `[Boot] ERROR: No valid APP found at 0x08004000!` |

链接器 map 显示 `BOOTLOAD2APP` 为 **dead code**（未被 main 调用）。

### 6. Bootloader 向量表 RAM 重映射（已实现，条件编译未开启）

文件：`Device/MM32F0140/Source/system_mm32f0140.c`

- 宏 `BUILD_BOOTLOADER` 定义时：
  - 将 APP 区（`0x08004000`）的 48 个中断向量复制到 RAM `0x20000000`
  - 通过 SYSCFG 将 RAM 映射到 `0x00000000`（解决 Cortex-M0 无 VTOR 的问题）
- **当前 Keil 工程未定义该宏**，此逻辑不会编译进固件

### 7. 启动与底层 HAL（SDK 提供，工程已链接）

- Keil 启动汇编 `Device/MM32F0140/Source/KEIL_StartAsm/startup_mm32f0140_keil.s`
- 完整 MindMotion HAL 库（GPIO、RCC、UART、FlexCAN、Flash 等 21 个模块）
- 实际运行中主要用到：`hal_rcc`、`hal_gpio`、`hal_uart`、`hal_flexcan`、`hal_misc`

## 设计意图 vs 当前状态

| 能力 | 代码状态 | 运行时状态 |
|------|---------|-----------|
| CAN 发送测试帧 | 完整实现 | **正在运行** |
| UART 调试输出 | 完整实现 | **正在运行** |
| CAN 接收 + 过滤 | 完整实现 | 中断就绪，main 未读 |
| 跳转 APP | 完整实现 | **未调用** |
| 向量表 RAM 重映射 | 完整实现 | **宏未开启** |
| CAN Bootloader 协议（固件升级） | 无 | **未实现** |
| Flash 编程 / OTA | HAL 已链接 | **未使用** |
| 独立 APP 工程 | 代码引用 `0x08004000` | **仓库中不存在** |

## Flash 内存规划（设计）

```
0x08000000 ─┬─ Bootloader 区（约 16KB，当前链接器仍占满 64KB）
            │
0x08004000 ─┴─ Application 区（约 48KB，APP 工程 IROM1 应对齐此地址）
```

## 硬件接口汇总

| 接口 | 引脚 | 参数 |
|------|------|------|
| UART1 TX（调试） | PA9 | 115200 bps |
| CAN RX | PA11 | 250 kbps |
| CAN TX | PA12 | 250 kbps |

## 构建与烧录

1. 使用 Keil µVision 打开 `Keil Project/Bootloader_CAN.uvprojx`
2. 需要 MindMotion 设备包：`MindMotion.MM32F0140_DFP.1.2.2`
3. 编译后烧录 `Keil Project/Objects/Bootloader_CAN.hex`
4. 连接 UART（PA9，115200）查看启动日志；CAN 总线（PA11/PA12，250 kbps）可观测 ID `0x18FF50E5` 测试帧

## 已知问题 / 待完善项

1. **Bootloader 名不副实** — 工程叫 Bootloader_CAN，实际为 CAN 测试程序
2. **`Keil Project/can.h`** 为过时副本，与 `User/can.h` 不同步
3. **`hal_sqrt.c/h`** 为空文件但仍被工程引用
4. **`BUILD_BOOTLOADER` 未定义** — 向量表重映射未生效
5. **全量 HAL 链接** — 多数外设驱动未使用，靠链接器 dead-strip 减小体积

## 总结

本项目是一个 **MM32F0144 充电器 CAN Bootloader 脚手架**。当前固件实际完成并运行的是：

1. 72MHz 系统初始化
2. UART1 调试控制台（printf）
3. FlexCAN 250kbps 初始化、过滤、中断、错误恢复
4. 周期性发送扩展 CAN 测试帧 `0x18FF50E5`

 additionally 已实现但未接入主流程的能力：

5. CAN 接收与 `CAN_Rd_Msg()` 读取接口
6. 从 `0x08004000` 跳转 APP 的 `BOOTLOAD2APP()`
7. Bootloader 模式下 APP 向量表 RAM 重映射（需定义 `BUILD_BOOTLOADER`）

尚未实现：CAN 固件升级协议、Flash 擦写流程、Boot 与 APP 切换逻辑、完整双区工程布局。
