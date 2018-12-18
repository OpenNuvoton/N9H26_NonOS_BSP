/***************************************************************************
 *                                                                         *
 * Copyright (c) 2008 Nuvoton Technolog. All rights reserved.              *
 *                                                                         *
 ***************************************************************************/

/****************************************************************************
* FILENAME
*   wb_power.c
*
* VERSION
*   1.0
*
* DESCRIPTION
*   The power managemnet related function of Nuvoton ARM9 MCU
*
* DATA STRUCTURES
*   None
*
* FUNCTIONS
*   sysDisableAllPM_IRQ
*   sysEnablePM_IRQ
*   sysPMStart
*
* HISTORY
*   2008-07-24  Ver 1.0 Modified by Min-Nan Cheng
*
* REMARK
*   When enter PD or MIDLE mode, the sysPMStart function will disable cache
*   (in order to access SRAM) and then recovery it after wake up.
****************************************************************************/
#include <stdio.h>
#include <string.h>
#include "wblib.h"

#define REG_DLLMODE_R	0xb0003058	/* Not fix in B version. still read in 3058 */
#define DBG_PRINTF(...)
UINT8  _tmp_buf[PD_RAM_SIZE];
static void Sample_PowerDown(void)
{
	register int reg2, reg1, reg0;

	UINT32 u32RstDebounce = inp32(REG_EXTRST_DEBOUNCE);
	outp32(REG_EXTRST_DEBOUNCE, 0x0);					/* Disable Reset debounce as power down */
	
	__asm
	{/* Dealy */
		mov 	reg2, #100
		mov		reg1, #0
		mov		reg0, #1
	loopz:		add reg1, reg1, reg0
		cmp 	reg1, reg2
		bne		loopz
	}
	
	//outp32(REG_AHBCLK, inp32(REG_AHBCLK)|GVE_CKE);			/* The clock need to be enable if power down. Otherwise, pressing reset button will be useless */
	
	outp32(REG_SDOPM, inp32(REG_SDOPM) & ~AUTOPDN);			             
	outp32(REG_DLLMODE,  (inp32(REG_DLLMODE_R)&~0x8) | 0x10);	// Disable chip's DLL
	/* Enter self refresh */	
	outp32(REG_SDCMD, (inp32(REG_SDCMD) & ~0x20) | 0x10);	
				
	__asm
	{/* Dealy */
		mov 	reg2, #100
		mov		reg1, #0
		mov		reg0, #1
	loopa:		add reg1, reg1, reg0
		cmp 	reg1, reg2
		bne		loopa
	}	
	
//	outp32(REG_DLLMODE,  (inp32(REG_DLLMODE_R)&~0x8) | 0x10);	// Disable chip's DLL
	  
	/* Change the system clock souce to 12M crystal*/
	outp32(REG_CLKDIV0, (inp32(REG_CLKDIV0) & (~0x18)) );				
	__asm
	{/* Delay */
		mov 	reg2, #100
		mov		reg1, #0
		mov		reg0, #1
	loop0:	add 		reg1, reg1, reg0
		cmp 		reg1, reg2
		bne		loop0
	}		

	outp32(REG_UPLLCON, inp32(REG_UPLLCON) | 0x4000);		/* Power down UPLL and APLL */
	outp32(REG_APLLCON, inp32(REG_APLLCON) | 0x4000);			
	__asm
	{
			mov 		reg2, #300
			mov		reg1, #0
			mov		reg0, #1
	loop1:	add 		reg1, reg1, reg0
			cmp 		reg1, reg2
			bne		loop1
	}		
	/* Fill and enable the pre-scale for power down wake up */
	
	__asm
	{
		mov 	reg2, #10
		mov		reg1, #0
		mov		reg0, #1
	loop2:	add 	reg1, reg1, reg0
		cmp 		reg1, reg2
		bne		loop2
	}	
	
	///////////////////////////////////////////////////////////////
	/*  Enter power down. (Stop the external clock */
	///////////////////////////////////////////////////////////////
	__asm 
    {/* Power down */
    		mov 	reg2, 0xb0000200
    		ldmia 	reg2, {reg0}
    		bic		reg0, reg0, #0x01
    		stmia 	reg2, {reg0}
    }   
	__asm
	{/* Wake up*/ 
			mov 	reg2, #300
			mov		reg1, #0
			mov		reg0, #1
	loop3:	add 	reg1, reg1, reg0
			cmp 	reg1, reg2
			bne		loop3
	}
	///////////////////////////////////////////////////////////////
	
	 /* Force UPLL and APLL in normal mode */ 
	outp32(REG_UPLLCON, inp32(REG_UPLLCON) & (~0x4000));		
	outp32(REG_APLLCON, inp32(REG_APLLCON) & (~0x4000));	
	while((inp32(REG_POR_LVRD)&APLL_LKDT)==0);				// Wait PLL lock bit.
/*
	__asm
	{// Delay a moment for PLL stable 
			mov 	r2, #5000
			mov		r1, #0
			mov		r0, #1
	loop4:	add 	r1, r1, r0
			cmp 	r1, r2
			bne		loop4
	}		
*/
	/* Change system clock to PLL and delay a moment.  Let DDR/SDRAM lock the frequency */
	outp32(REG_CLKDIV0, inp32(REG_CLKDIV0) | 0x18);					
/*
	__asm
	{
			mov 	r2, #500
			mov		r1, #0
			mov		r0, #1
	loop5:	add 	r1, r1, r0
			cmp 		r1, r2
			bne		loop5
	}
*/
	/*Force DDR/SDRAM escape self-refresh */
	outp32(0xB0003004,  0x20);		

	__asm
	{/*  Delay a moment until the escape self-refresh command reached to DDR/SDRAM */
			mov 	reg2, #100
			mov		reg1, #0
			mov		reg0, #1
	loop6:	add 	reg1, reg1, reg0
			cmp 	reg1, reg2
			bne		loop6
	}	

	outp32(REG_SDMR,  0x532);   								// RESET DLL(bit[8]) of DDR2 
	__asm
	{/*  Delay a moment until the escape self-refresh command reached to DDR/SDRAM */
			mov 	reg2, #100
			mov		reg1, #0
			mov		reg0, #1
	loop7:	add 	reg1, reg1, reg0
			cmp 	reg1, reg2
			bne		loop7
	}	
	outp32(REG_SDMR,  0x432);  									// RESET DLL(bit[8]) of DDR2 		


	outp32(REG_DLLMODE,   inp32(REG_DLLMODE_R)       | 0x18);	// Enable chip's DLL
	__asm														/*  Wait chip's DLL lock time > 100us */
	{
			mov 	reg2, #10000
			mov		reg1, #0
			mov		reg0, #1
	loop8:	add 	reg1, reg1, reg0
			cmp 	reg1, reg2
			bne		loop8
	}	
	outp32(REG_EXTRST_DEBOUNCE, u32RstDebounce);	
//	while (!(inpw(0xb8008118) & 0x400000));
//	outpb(0xb8008100, '6');		
			
}



static void Entry_PowerDown(UINT32 u32WakeUpSrc)
{
	UINT32 j;
	UINT32 u32IntEnable, u32IntEnableH;
	void (*wb_fun)(void);
	UINT32 u32RamBase = PD_RAM_BASE;
	UINT32 u32RamSize = PD_RAM_SIZE;	
	BOOL bIsEnableIRQ = FALSE;
	
	if( sysGetIBitState()==TRUE )
	{
		bIsEnableIRQ = TRUE;
		sysSetLocalInterrupt(DISABLE_IRQ);	
	}		
	memcpy((char*)((UINT32)_tmp_buf| 0x80000000), (char*)(u32RamBase | 0x80000000), u32RamSize);
	memcpy((char*)(u32RamBase | 0x80000000), (char*)((UINT32)Sample_PowerDown | 0x80000000), u32RamSize);
	if(memcmp((char*)(u32RamBase | 0x80000000), (char*)((UINT32)Sample_PowerDown | 0x80000000), u32RamSize)!=0)
	{
		sysprintf("Memcpy copy wrong\n");
	}
	sysFlushCache(I_CACHE);		
	wb_fun = (void(*)(void)) u32RamBase;
	
	DBG_PRINTF("Jump to SRAM (Suspend)\n");
	
	u32IntEnable = inp32(REG_AIC_IMR);
	u32IntEnableH = inp32(REG_AIC_IMRH);	
	
	outp32(REG_AIC_MDCR, 0xFFFFFFFE);
	outp32(REG_AIC_MDCRH, 0xFFFFFFFE);	
	outp32(REG_AIC_MECR, 0x00000000);	
	outp32(REG_AIC_MECRH, 0x00000000);
	j = 0x800;
	while(j--);
	if(u32WakeUpSrc>WE_UHC20)
		outp32(REG_MISSR, ((u32WakeUpSrc<<16)|(u32WakeUpSrc<<8)));	//Enable and Clear interrupt
	else	
		outp32(REG_MISSR, ((u32WakeUpSrc<<14)|(u32WakeUpSrc<<6)));
	wb_fun();

	if(u32WakeUpSrc>WE_EMAC)
		outp32(REG_MISSR, ((u32WakeUpSrc<<16)|(u32WakeUpSrc<<8)));	//Enable and Clear interrupt
	else	
		outp32(REG_MISSR, ((u32WakeUpSrc<<14)|(u32WakeUpSrc<<6)));
	memcpy((VOID *)u32RamBase, (VOID *)_tmp_buf, PD_RAM_SIZE);
	DBG_PRINTF("Exit to SRAM (Suspend)\n");
	outp32(REG_AIC_MECR, u32IntEnable);								/*  Restore the interrupt channels */		
	outp32(REG_AIC_MECRH, u32IntEnableH);	
	if(bIsEnableIRQ==TRUE)
		sysSetLocalInterrupt(ENABLE_IRQ);	
}
ERRCODE sysPowerDown(UINT32 u32WakeUpSrc)
{
	Entry_PowerDown(u32WakeUpSrc);
	return Successful;
}
