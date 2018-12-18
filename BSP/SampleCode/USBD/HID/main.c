/****************************************************************************
 * @file     main.c
 * @version  V1.00
 * $Revision: 4 $
 * $Date: 18/04/25 11:43a $
 * @brief    HID Class Device sample header file
 *
 * @note
 * Copyright (C) 2018 Nuvoton Technology Corp. All rights reserved.
 *****************************************************************************/
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "N9H26.h"
#include "HID.h"

int main(void)
{
    WB_UART_T uart;
    UINT32 u32ExtFreq;
    sysUartPort(1);
    u32ExtFreq = sysGetExternalClock();        /* Hz unit */
    uart.uiFreq = u32ExtFreq;
    uart.uiBaudrate = 115200;
    uart.uiDataBits = WB_DATA_BITS_8;
    uart.uart_no=WB_UART_1;
    uart.uiStopBits = WB_STOP_BITS_1;
    uart.uiParity = WB_PARITY_NONE;
    uart.uiRxTriggerLevel = LEVEL_1_BYTE;
    sysInitializeUART(&uart);

    sysprintf("Sample code Start\n");

#ifdef HID_KEYPAD
    DrvADC_Open();
#endif
    /* Enable USB */
    udcOpen();
    hidInit();
    udcInit();
    while(1)
    {
#ifdef HID_KEYBOARD
        HID_SetInReport();
#endif
#ifdef HID_MOUSE
        HID_UpdateMouseData();
#endif
    };
}

