/**************************************************************************//**
 * @file     SpiToUart.c
 * @brief    Demonstrate how to use SPI to UART transfer data.
 *
 * SPDX-License-Identifier: Apache-2.0
 * @copyright (C) 2020 Nuvoton Technology Corp. All rights reserved.
 *****************************************************************************/

#include <stdio.h>
#include "N9H26.h"
#include "N9H26_SPI.h"
#include "N9H26_SPI_SPIToUART.h"


unsigned  char *pSrc,*pDst,*pDst2;
#define TEST_SIZE	512 * 2 * 64
#if defined(__GNUC__)
__attribute__((aligned(4096))) UINT8 WriteBuffer[TEST_SIZE];
__attribute__((aligned(4096))) UINT8 ReadBuffer[TEST_SIZE];
#else
__align(4096) UINT8 WriteBuffer[TEST_SIZE];
__align(4096) UINT8 ReadBuffer[TEST_SIZE];
#endif

void UARTTestItem()
{
	sysprintf("\n\n");
	sysprintf("+--------------------------------------------------+\n");
  sysprintf("| N9H20-MINI58  SPI-UART Sample Code              |\n");
  sysprintf("+--------------------------------------------------+\n");
	sysprintf("| [1] UART0_LOOPBACK                               |\n");
	sysprintf("| [2] UART1_LOOPBACK                               |\n");
	sysprintf("| [3] UART1&UART0_LOOPBACK                         |\n");
	sysprintf("| [4] Close all UART                               |\n");
	sysprintf("| [5] Reset all UART                               |\n");
	sysprintf("| [6] Clear TXRX Buffer                            |\n");
	sysprintf("+--------------------------------------------------+\n");
}


void UART_LOOPBACK(int SPI_UART)
{

	int tmp2,tmp3,tmp4;

	if(SPI_UART!=UART_ALL)
	{
		while(1)
		{
			while(vu_GetStatus()==1);
			while(VUART_RX_INT(SPI_UART))
			{
				tmp4=vu_GetRXAvailLen(SPI_UART);
				tmp2=vu_UARTRead(SPI_UART, tmp4,pDst);
				vu_UARTWrite(SPI_UART,pDst,tmp2);
			}
		}
	}
	else
	{
		while(1)
		{
			while(vu_GetStatus()==1); 							//wait mini58 response interrupt
			while(VUART_RX_INT(SPI_UART))
			{ 					//wait rx read interrupt
				tmp4=vu_GetRXAvailLen(SPI_UART);  		//get count of UART0 & UART1 rx data
				if((tmp4&0xf)>0)
				{
						tmp2=vu_UARTRead(0,tmp4&0xf,pDst);	//read uart0 rx and get count of UART0 rx data from uart0 rx buffer 
						vu_UARTWrite(0,pDst,tmp2);
				}
				
				if(((tmp4&0xf0)>>4)>0)
				{
					tmp3=vu_UARTRead(1,((tmp4&0xf0)>>4),pDst2); 	//read uart1 rx and get count of UART1 rx data from uart1 rx buffer 
					vu_UARTWrite(1,pDst2,tmp3);						//write # bytes to uart1 rx
				}
			}
		}
	}
}
int main()
{
	WB_UART_T uart;
	UINT32 u32ExtFreq;
	unsigned char  *pDst;
	int volatile i;
	char item;
	int  SPI_UART=1;
  u32ExtFreq = sysGetExternalClock();        /* Hz unit */    
  uart.uart_no = WB_UART_1;
  uart.uiFreq = u32ExtFreq;
  uart.uiBaudrate = 115200;
  uart.uiDataBits = WB_DATA_BITS_8;
  uart.uiStopBits = WB_STOP_BITS_1;
  uart.uiParity = WB_PARITY_NONE;
  uart.uiRxTriggerLevel = LEVEL_1_BYTE;
  sysInitializeUART(&uart);

    /********************************************************************************************** 
     * Clock Constraints: 
     * (a) If Memory Clock > System Clock, the source clock of Memory and System can come from
     *     different clock source. Suggestion MPLL for Memory Clock, UPLL for System Clock   
     * (b) For Memory Clock = System Clock, the source clock of Memory and System must come from 
     *     same clock source	 
     *********************************************************************************************/
#if 0 
    /********************************************************************************************** 
     * Slower down system and memory clock procedures:
     * If current working clock fast than desired working clock, Please follow the procedure below  
     * 1. Change System Clock first
     * 2. Then change Memory Clock
     * 
     * Following example shows the Memory Clock = System Clock case. User can specify different 
     * Memory Clock and System Clock depends on DRAM bandwidth or power consumption requirement. 
     *********************************************************************************************/
    sysSetSystemClock(eSYS_EXT, 12000000, 12000000);
    sysSetDramClock(eSYS_EXT, 12000000, 12000000);
#else 
    /********************************************************************************************** 
     * Speed up system and memory clock procedures:
     * If current working clock slower than desired working clock, Please follow the procedure below  
     * 1. Change Memory Clock first
     * 2. Then change System Clock
     * 
     * Following example shows to speed up clock case. User can specify different 
     * Memory Clock and System Clock depends on DRAM bandwidth or power consumption requirement.
     *********************************************************************************************/
    sysSetDramClock(eSYS_MPLL, 360000000, 360000000);
    sysSetSystemClock(eSYS_UPLL,            //E_SYS_SRC_CLK eSrcClk,
                      240000000,            //UINT32 u32PllKHz,
                      240000000);           //UINT32 u32SysKHz,
    sysSetCPUClock(240000000);
    sysSetAPBClock(60000000);	
#endif	

	sysprintf("SpiFlash Test...\n");

	pSrc = (UINT8 *)((UINT32)WriteBuffer | 0x80000000);
	pDst = (UINT8 *)((UINT32)ReadBuffer | 0x80000000);


	sysEnableCache(CACHE_WRITE_BACK);
	
	sysprintf("\nSpi to UART Test...\n");

	UARTTestItem();
	
	do
	{
		item = sysGetChar();
		sysprintf("%c\n",item);   
		switch(item)
		{
			case '1':
				vu_OpenUART(UART_PORT0);
				vu_ResetUART(UART_PORT0);
				vu_SetBaudRate(0,115200);
				UART_LOOPBACK(UART_PORT0); break;		
			case '2':
				vu_OpenUART(UART_PORT1);
				vu_ResetUART(UART_PORT1);
				vu_SetBaudRate(1,115200);
				UART_LOOPBACK(UART_PORT1); break;			
			case '3':
				vu_OpenUART(UART_ALL);
				vu_ResetUART(UART_ALL);
				vu_SetBaudRate(0,115200);
				vu_SetBaudRate(1,57600);
				UART_LOOPBACK(UART_ALL); break;
			case '4':
				vu_OpenUART(UART_ALL);
				vu_CloseUART(UART_ALL); break;
			case '5':
				vu_OpenUART(UART_ALL);
				vu_SetBaudRate(0,57600);
				vu_SetBaudRate(1,57600);
				sysprintf("Press any key to UART0 or UART1\n");
				while(VUART_RX_INT(UART_ALL)!=1);
				sysprintf("Berfor:(UART0,UART1)= (%d,%d) bytes data in RX buffer \n",(vu_GetRXAvailLen(UART_ALL)&0XF),(vu_GetRXAvailLen(UART_ALL)&0XF0)>>4);
				vu_ResetUART(UART_ALL); 
				sysprintf("After: (UART0,UART1)= (%d,%d) bytes data in RX buffer \n",(vu_GetRXAvailLen(UART_ALL)&0XF),(vu_GetRXAvailLen(UART_ALL)&0XF0)>>4);	
				vu_CloseUART(UART_ALL); break;
			case '6':
				vu_OpenUART(UART_PORT0);
				vu_SetBaudRate(0,115200);
				sysprintf("Press any key to UART0 \n");
				while(VUART_RX_INT(UART_PORT0)!=1);
				sysprintf("Berfor:UART0= %d bytes data in RX buffer \n",(vu_GetRXAvailLen(UART_ALL)&0XF));
				vu_ClearBuf(UART_PORT0);
				sysprintf("After:UART0= %d bytes data in RX buffer \n",(vu_GetRXAvailLen(UART_ALL)&0XF));	
				sysprintf("Press any key to UART0. \n");		
				while(VUART_RX_INT(UART_PORT0)!=1);		
				vu_UARTRead(SPI_UART, 1,pDst);
				sysprintf("READ(UART%d)=%c\n",UART_PORT0,*pDst); break;

			default:
				break;				
				
		}
	}while(item!=0x1B);


	while(1);


	return 0;
}

