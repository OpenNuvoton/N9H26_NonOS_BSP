/**************************************************************************//**
 * @file     main.c
 * @version  V3.00
 * @brief    N9H26 series SPU demo code
 *
 * SPDX-License-Identifier: Apache-2.0
 * @copyright (C) 2020 Nuvoton Technology Corp. All rights reserved.
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wblib.h"
#include "N9H26.h"

static UINT32 u32FragSize;
static volatile UINT8 bPlaying = TRUE;
static UINT16 u16IntCount = 2;

#if defined (__GNUC__) && !(__CC_ARM)
__attribute__ ((aligned (32))) UINT8 g_AudioPattern[] = {
		#include "PCM16_raw.dat"
};
#else
__align (32) UINT8 g_AudioPattern[] = {
		#include "PCM16_raw.dat"
};
#endif

extern VOID spuDacOn(UINT8 level);
extern VOID spuSetDacSlaveMode(void);

int playCallBack(UINT8 * pu8Buffer)
{	
	UINT32 u32Offset = 0;
	UINT32 len = sizeof(g_AudioPattern);
	
	u32Offset = ( u32FragSize / 2) * u16IntCount;
	if (u32Offset >= len)		// Reach the end of the song, restart from beginning
	{		
		u16IntCount = 2;
		u32Offset = 0;
		bPlaying = FALSE;
		return TRUE;
	}	
	memcpy(pu8Buffer, &g_AudioPattern[u32Offset], u32FragSize/2);	
	u16IntCount++;
	return FALSE;
}

int main(void)
{
	UINT32 u32TestChannel, uSamplingRate;	
	UINT8 u8SrcFormat;
    WB_UART_T uart;
    UINT32 u32ExtFreq, u32PllOutKHz;

    u32ExtFreq = sysGetExternalClock();
    uart.uart_no = WB_UART_1;
    uart.uiFreq = u32ExtFreq;   //use APB clock
    uart.uiBaudrate = 115200;
    uart.uiDataBits = WB_DATA_BITS_8;
    uart.uiStopBits = WB_STOP_BITS_1;
    uart.uiParity = WB_PARITY_NONE;
    uart.uiRxTriggerLevel = LEVEL_1_BYTE;
    sysInitializeUART(&uart);
    sysSetLocalInterrupt(ENABLE_FIQ_IRQ);

    u32PllOutKHz = sysGetPLLOutputHz(eSYS_UPLL, u32ExtFreq);
    sysprintf("Power on UPLL out frequency %d Khz\n", u32PllOutKHz);
    u32PllOutKHz = sysGetPLLOutputHz(eSYS_MPLL, u32ExtFreq);
    sysprintf("Power on MPLL out frequency %d Khz\n", u32PllOutKHz);

    u32PllOutKHz = sysGetAPBClock();
    sysprintf("APB %d Khz\n", u32PllOutKHz);
		
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
    sysSetCPUClock(240000000/2);
#endif
#if 0
	/* enable U10 ISD8101 in N9H26 HMI demo board */
	outpw(REG_GPAFUN0, inpw(REG_GPAFUN0)&~MF_GPA0);	// enable LPCLK pin
	outpw(REG_GPIOA_OMD, REG_GPIOA_OMD| 0x00000001);
	outpw(REG_GPIOA_DOUT, inpw(REG_GPIOA_DOUT)| 0x00000001);
#endif

	/* enable cache */
	sysEnableCache(I_D_CACHE);
	
	/* set SPU volume and sampling rate */
	spuOpen(eDRVSPU_FREQ_44100);
	spuIoctl(SPU_IOCTL_SET_VOLUME, 0x3f, 0x3f);
	
	/* get default fragment size */	
	spuIoctl(SPU_IOCTL_GET_FRAG_SIZE, (UINT32)&u32FragSize, 0);

	/* enable SPU DAC */
	spuDacOn(1);
	spuSetDacSlaveMode();

	/* start SPU playing */
	sysprintf("Start Playing...\n");	
    spuStartPlay((PFN_DRVSPU_CB_FUNC *) playCallBack, (UINT8 *)g_AudioPattern);
	
	/* wait to finish playing */
   	while(bPlaying == TRUE);
	
	/* SPU stop */
    spuClose();
	
    while(1);	
}
