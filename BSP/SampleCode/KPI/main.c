/**************************************************************************//**
 * @file     main.c
 * @brief    N9H26 series keypad demo code
 *
 * SPDX-License-Identifier: Apache-2.0
 * @copyright (C) 2020 Nuvoton Technology Corp. All rights reserved.
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "N9H26.h"

int main(void)
{

	WB_UART_T uart;	
	UINT32 u32ExtFreq;
	unsigned int key;

	u32ExtFreq = sysGetExternalClock();
    	sysUartPort(1);
    	uart.uart_no = WB_UART_1; 		
	uart.uiFreq = u32ExtFreq;	
    uart.uiBaudrate = 115200;
    uart.uiDataBits = WB_DATA_BITS_8;
    uart.uiStopBits = WB_STOP_BITS_1;
    uart.uiParity = WB_PARITY_NONE;
    uart.uiRxTriggerLevel = LEVEL_1_BYTE;
    sysInitializeUART(&uart);

	sysprintf("start kpi test...\n");

	kpi_init();
	kpi_open(3); // use nIRQ0 as external interrupt source
	
	while(1) {
#ifdef _KPI_NONBLOCK_
		key = kpi_read(KPI_NONBLOCK);
#else
		key = kpi_read(KPI_BLOCK);
#endif		
		sysprintf("key is %d\n", key);
		//sysDelay(20);
	
	}
	kpi_close();
	sysprintf("quit kpi test...\n");
	return(0);
	
}
