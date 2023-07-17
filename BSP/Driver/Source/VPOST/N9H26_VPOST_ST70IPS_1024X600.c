/**************************************************************************//**
 * @file     N9H26_VPOST_ST70IPS_1024x600.c
 * @version  V3.00
 * @brief    Panel driver file
 *
 * SPDX-License-Identifier: Apache-2.0
 * @copyright (C) 2020 Nuvoton Technology Corp. All rights reserved.
 *****************************************************************************/

#include "stdio.h"
#include "stdlib.h"
#include "N9H26_VPOST.h"

extern void LCDDelay(unsigned int nCount);

#if defined(__HAVE_ST70IPS_1024x600__)

static UINT32 g_nScreenWidth;
static UINT32 g_nScreenHeight;

typedef enum 
{
	eEXT 	= 0,
	eX32K 	= 1,
	eAPLL  	= 2,
	eUPLL  	= 3
}E_CLK;

static void BacklightControl(int OnOff)
{	
	// GPD[0] set OUTPUT mode  => control the backlight
	outpw(REG_GPIOD_OMD, (inpw(REG_GPIOD_OMD) & 0x0000FFFF)| 0x00000001);
	if(OnOff==TRUE) {
		// GPD[0] turn on the backlight
		outpw(REG_GPIOD_DOUT, (inpw(REG_GPIOD_DOUT) & 0x0000FFFF)| 0x00000001);
	} else {
		// GPD[0] diable backlight
		outpw(REG_GPIOD_DOUT, (inpw(REG_GPIOD_DOUT) & 0x0000FFFF) & 0xFFFFFFFE);
	}
}

INT vpostLCMInit_ST70IPS_1024x600(PLCDFORMATEX plcdformatex, UINT32 *pFramebuf)
{
	volatile S_DRVVPOST_SYNCLCM_WINDOW sWindow = {1024,600,1024};	
	volatile S_DRVVPOST_SYNCLCM_HTIMING sHTiming = {10,160,(UINT8)210};
	volatile S_DRVVPOST_SYNCLCM_VTIMING sVTiming = {10,23,22};
	volatile S_DRVVPOST_SYNCLCM_POLARITY sPolarity = {TRUE,TRUE,FALSE,TRUE};


	volatile S_DRVVPOST_FRAME_SIZE sFSize;
	volatile S_DRVVPOST_SCALING_OUTPUT sScal;	
	UINT32 nBytesPixel, u32PLLclk, u32ClockDivider, u32Clkin;

#define OPT_24BIT_MODE

	// VPOST clock control
	outpw(REG_AHBCLK, inpw(REG_AHBCLK) | VPOST_CKE | HCLK4_CKE);
	outpw(REG_AHBIPRST, inpw(REG_AHBIPRST) | VPOST_RST);
	outpw(REG_AHBIPRST, inpw(REG_AHBIPRST) & ~VPOST_RST);	
	
	u32Clkin = sysGetExternalClock();
	u32PLLclk = sysGetPLLOutputHz(eUPLL, u32Clkin);		// CLK_IN = 12 MHz
	u32ClockDivider = u32PLLclk / 48000000;
	u32ClockDivider--;
	outpw(REG_CLKDIV1, inpw(REG_CLKDIV1) & ~VPOST_N0 );						
	outpw(REG_CLKDIV1, (inpw(REG_CLKDIV1) & ~VPOST_N1) | ((u32ClockDivider & 0xFF) << 8));						
	outpw(REG_CLKDIV1, inpw(REG_CLKDIV1) & ~VPOST_S);
	outpw(REG_CLKDIV1, inpw(REG_CLKDIV1) | (3<<3));		// VPOST clock from UPLL	

	vpostVAStopTrigger();	

	// Enable VPOST function pins
#ifdef	OPT_24BIT_MODE		
	vpostSetDataBusPin(eDRVVPOST_DATA_24BITS);
#else
//	vpostSetDataBusPin(eDRVVPOST_DATA_16BITS);
	vpostSetDataBusPin(eDRVVPOST_DATA_18BITS);	
#endif	
		  
	// LCD image source select
	vpostSetLCM_ImageSource(eDRVVPOST_FRAME_BUFFER);
	
	// configure LCD interface
	vpostSetLCM_TypeSelect(eDRVVPOST_HIGH_RESOLUTINO_SYNC);

	// configure LCD timing sync or async with TV timing	
	vpostsetLCM_TimingType(eDRVVPOST_ASYNC_TV);
	
    // Configure Parallel LCD interface (16/18/24-bit data bus)
#ifdef	OPT_24BIT_MODE		
    vpostSetParalelSyncLCM_Interface(eDRVVPOST_PRGB_24BITS);
#else    
    vpostSetParalelSyncLCM_Interface(eDRVVPOST_PRGB_16BITS);
//    vpostSetParalelSyncLCM_Interface(eDRVVPOST_PRGB_18BITS);    
#endif    
    
    // set Horizontal scanning line timing for Syn type LCD 
    vpostSetSyncLCM_HTiming((S_DRVVPOST_SYNCLCM_HTIMING *)&sHTiming);

	// set Vertical scanning line timing for Syn type LCD   
    vpostSetSyncLCM_VTiming((S_DRVVPOST_SYNCLCM_VTIMING *)&sVTiming);
	
	// set both "active pixel per line" and "active lines per screen" for Syn type LCD   
	vpostSetSyncLCM_ImageWindow((S_DRVVPOST_SYNCLCM_WINDOW *)&sWindow);

  	// set Hsync/Vsync/Vden/Pclk poalrity
	vpostSetSyncLCM_SignalPolarity((S_DRVVPOST_SYNCLCM_POLARITY *)&sPolarity);
    
	sFSize.u16HSize = 1024;
	sFSize.u16VSize = 600;  
	sScal.u16HSize = 1024;
	sScal.u16VSize = 6000;  
//	vpostSetScalingOutput_Size((S_DRVVPOST_SCALING_OUTPUT*)&sScal);
//	outpw(REG_LCM_LCDCCtl, inpw(REG_LCM_LCDCCtl) | LCDCCtl_SC_EN); 	// enable scaling feature   			
	vpostSetFrameBuffer_Size((S_DRVVPOST_FRAME_SIZE*)&sFSize);
    // set frambuffer base address
    if(pFramebuf != NULL) {
		vpostAllocVABufferFromAP(pFramebuf);
	} else {
    	if( vpostAllocVABuffer(plcdformatex, nBytesPixel)==FALSE)
    		return ERR_NULL_BUF;
    }
	
	// set frame buffer data format
	vpostSetFrameBuffer_DataType(plcdformatex->ucVASrcFormat);
	
	vpostSetYUVEndianSelect(eDRVVPOST_YUV_LITTLE_ENDIAN);
	
	// enable LCD controller
	vpostVAStartTrigger();
	
//	BacklightControl(FALSE);
	return 0;
}

INT32 vpostLCMDeinit_ST70IPS_1024x600(VOID)
{
	vpostVAStopTrigger();
	vpostFreeVABuffer();
	outpw(REG_AHBCLK, inpw(REG_AHBCLK) & ~VPOST_CKE);	
	return 0;
}

VOID vpostEnaBacklight(void)
{
#define VPOST_ICE_DEBUG	
#ifndef VPOST_ICE_DEBUG
	/* set BL_EN (GPA6) to High */
//	outpw(REG_GPAFUN, inpw(REG_GPAFUN) & ~MF_GPE6);
//	outpw(REG_GPIOA_OMD, inpw(REG_GPIOA_OMD) | 0x00000040);
//	outpw(REG_GPIOA_DOUT, inpw(REG_GPIOA_DOUT) | 0x00000040);

	/* set BL_CTL (GPD0) to Low */
	outpw(REG_GPDFUN, inpw(REG_GPDFUN) & ~MF_GPD0);
	outpw(REG_GPIOD_OMD, inpw(REG_GPIOD_OMD) | 0x00000001);
	outpw(REG_GPIOD_DOUT, inpw(REG_GPIOD_DOUT) & ~0x00000001);
#endif
}

#endif    //HAVE_ST70IPS_1024x600