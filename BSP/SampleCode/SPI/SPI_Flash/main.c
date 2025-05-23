/**************************************************************************//**
 * @file     main.c
 * @brief    Access SPI flash through SPI interface.
 *
 * SPDX-License-Identifier: Apache-2.0
 * @copyright (C) 2020 Nuvoton Technology Corp. All rights reserved.
 *****************************************************************************/

#include <stdio.h>
#include <string.h>
#include "N9H26.h"

#define TEST_SIZE	512 * 2 * 64
#if defined(__GNUC__)
__attribute__((aligned(4096))) UINT8 WriteBuffer[TEST_SIZE];
__attribute__((aligned(4096))) UINT8 ReadBuffer[TEST_SIZE];
#else
__align(4096) UINT8 WriteBuffer[TEST_SIZE];
__align(4096) UINT8 ReadBuffer[TEST_SIZE];
#endif

int main()
{
	WB_UART_T uart;
	UINT32 u32ExtFreq;
	unsigned char *pSrc, *pDst;
	int volatile i;
	BOOL bLoop = TRUE;
	UINT8 u8Item, u8data;

	u32ExtFreq = sysGetExternalClock();    	/* Hz unit */	
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

	for (i=0; i<TEST_SIZE; i++)
		*(pSrc+i) = i & 0xff;

	spiFlashInit();

	spiFlashEraseAll();

	while(bLoop)
	{
		sysprintf("\nPlease Select Test Item\n");
		sysprintf(" 1 : Read/Write Test with Winbond W25Q256FV\n");
		sysprintf(" 2 : Qude Read/Write Test with Winbond W25Q256FV\n");
		sysprintf(" 3 : Qude Read/Write Test with EON EN25QH256\n");
		sysprintf(" 9 : Exit\n>");

		u8Item = sysGetChar();

		switch(u8Item)
		{
			case '1':	
				usiStatusWrite1(0x00, 0x00);	// clear block protect , disable QE
				usiStatusRead(0x35, &u8data);
				sysprintf("SpiFlash Register-2 Status 0x%x \n", u8data);
				
				sysprintf("\tErase SpiFlash Block 0\n");
				spiFlashEraseBlock(0, 1);
				
				sysprintf("\tWrite SpiFlash\n");
				spiFlashWrite(0, TEST_SIZE, pSrc);

				memset(pDst, 0, TEST_SIZE);

				sysprintf("\tRead and Compare SpiFlash\n");
				spiFlashRead(0, TEST_SIZE, pDst);

				for (i=0; i<TEST_SIZE; i++)
				{
					if (*(pSrc+i) != *(pDst+i))
					{
						sysprintf("error!! Src[%d] = 0x%X, Dst[%d] = 0x%X\n", i, *(pSrc+i), i, *(pDst+i));
						break;
					}
				}
					
				break;

			case '2':	
				usiStatusWrite1(0x00, 0x02);	// clear block protect , set QE
				usiStatusRead(0x35, &u8data);
				sysprintf("SpiFlash Register-2 Status 0x%x \n", u8data);
				
				sysprintf("\tErase SpiFlash Block 0\n");
				spiFlashEraseBlock(0, 1);
				
				sysprintf("\tWrite SpiFlash\n");
				spiFlashQuadWrite(0, TEST_SIZE, pSrc);

				memset(pDst, 0, TEST_SIZE);

				sysprintf("\tRead and Compare SpiFlash\n");
				spiFlashFastReadQuad(0, TEST_SIZE, pDst);

				for (i=0; i<TEST_SIZE; i++)
				{
					if (*(pSrc+i) != *(pDst+i))
					{
						sysprintf("error!! Src[%d] = 0x%X, Dst[%d] = 0x%X\n", i, *(pSrc+i), i, *(pDst+i));
						break;
					}
				}
					
				break;

			case '3':	
				sysprintf("\tErase SpiFlash Block 0\n");
				spiFlashEraseBlock(0, 1);
				
				sysprintf("\tWrite SpiFlash\n");
				spiEONFlashQuadWrite(0, TEST_SIZE, pSrc);

				memset(pDst, 0, TEST_SIZE);

				sysprintf("\tRead and Compare SpiFlash\n");
				spiEONFlashFastReadQuad(0, TEST_SIZE, pDst);

				for (i=0; i<TEST_SIZE; i++)
				{
					if (*(pSrc+i) != *(pDst+i))
					{
						sysprintf("error!! Src[%d] = 0x%X, Dst[%d] = 0x%X\n", i, *(pSrc+i), i, *(pDst+i));
						break;
					}
				}
					
				break;

			case '9':	

				bLoop = FALSE;
				break;

			default:
					sysprintf("Write Item\n");
					break;
		}			
				
	}

	sysprintf("finish SPI test\n");
	return 0;
}

