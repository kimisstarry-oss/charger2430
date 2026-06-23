#include <stdio.h>
#include <stdlib.h>
#include "hal_iwdg.h"
#include "hal_conf.h"
#include "bootloader.h"
#include "can.h"
#include "can_receive.h"

int main(void)
{
    BOOT_InitConsole(115200);
    CAN_Configuration();
    CAN_ReceiveTest_Init();

    printf("\r\n==============================");
    printf("\r\n[Boot] CAN reply mode");
    printf("\r\n[Boot] RX STD ID 0x101");
    printf("\r\n[Boot] RX EXT ID 0x18xxxxxx");
    printf("\r\n[Boot] Reply STD ID 0x141");
    printf("\r\n==============================\r\n");

    while (1)
    {
        Can_RxMessage_Test();
    }
}
