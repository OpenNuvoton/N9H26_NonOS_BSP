/****************************************************************************
*                                                                           *
* Copyright (c) 2009 Nuvoton Tech. Corp. All rights reserved.               *
*                                                                           *
*****************************************************************************/

/****************************************************************************
* FILENAME
*   main.c
*
* VERSION
*   1.0
*
* DESCRIPTION
*   SPU sample application using SPU library. 
*
* DATA STRUCTURES
*   None
*
* FUNCTIONS
*
* HISTORY
*
* REMARK
*   None
****************************************************************************/
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
	WB_UART_T uart;
	UINT32 u32ExtFreq;	

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
