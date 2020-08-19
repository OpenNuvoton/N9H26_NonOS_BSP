/**************************************************************************//**
 * @file     SPU.c
 * @version  V3.00
 * @brief    N9H26 series SPU driver source file
 *
 * SPDX-License-Identifier: Apache-2.0
 * @copyright (C) 2020 Nuvoton Technology Corp. All rights reserved.
 *****************************************************************************/
 
#include "string.h"
#include "stdlib.h"

#include "N9H26_SPU.h"

extern const int N_table[32];
extern const int sp_Digi_table[11];
extern const int sp_Ana_table[11];
extern const int ear_Digi_table[11];
extern const int ear_Ana_table[11];

volatile int g_i32FragSize = FRAG_SIZE;
/* buffer */
UINT8	*_pucPlayAudioBuff;
#if defined (__GNUC__) && !(__CC_ARM)
__attribute__ ((aligned (256))) UINT8 playbuffer[FRAG_SIZE];
#else
__align(256) UINT8 playbuffer[FRAG_SIZE];
#endif

/* function declaration */
ERRCODE DrvSPU_InstallCallBack(
	E_DRVSPU_CHANNEL eChannel,
	UINT32 eIntSource,
	PFN_DRVSPU_INT_CALLBACK	pfnCallback,
	PFN_DRVSPU_INT_CALLBACK *pfnOldCallback );

ERRCODE DrvSPU_Open(void);
void DrvSPU_Close(void);
void DrvSPU_ISR(void);
ERRCODE DrvSPU_ChannelOpen(E_DRVSPU_CHANNEL eChannel);
ERRCODE	DrvSPU_ChannelClose(E_DRVSPU_CHANNEL eChannel);
BOOL DrvSPU_IsChannelEnabled(E_DRVSPU_CHANNEL eChannel);	
ERRCODE	DrvSPU_EnableInt(E_DRVSPU_CHANNEL eChannel, UINT32 eInt, PFN_DRVSPU_CB_FUNC* pfnCallBack);
BOOL DrvSPU_IsIntEnabled(E_DRVSPU_CHANNEL eChannel, UINT32 eInt);
ERRCODE	DrvSPU_DisableInt(E_DRVSPU_CHANNEL eChannel, UINT32 eInt);
ERRCODE	DrvSPU_ClearInt(E_DRVSPU_CHANNEL eChannel, UINT32 eInt);
ERRCODE DrvSPU_PollInt(E_DRVSPU_CHANNEL eChannel, UINT32 eInt);
ERRCODE DrvSPU_SetBaseAddress(E_DRVSPU_CHANNEL eChannel, UINT32 u32Address);
ERRCODE DrvSPU_SetThresholdAddress(E_DRVSPU_CHANNEL eChannel, UINT32 u32Address);
ERRCODE DrvSPU_SetEndAddress(E_DRVSPU_CHANNEL eChannel, UINT32 u32Address);
ERRCODE DrvSPU_SetPauseAddress(E_DRVSPU_CHANNEL eChannel, UINT32 u32Address);
ERRCODE DrvSPU_GetBaseAddress(E_DRVSPU_CHANNEL eChannel, UINT32* pu32Address);
ERRCODE DrvSPU_GetThresholdAddress(E_DRVSPU_CHANNEL eChannel, UINT32* pu32Address);
ERRCODE DrvSPU_GetCurrentAddress(E_DRVSPU_CHANNEL eChannel, UINT32* pu32Address);
ERRCODE DrvSPU_GetLoopStartAddress(E_DRVSPU_CHANNEL eChannel, UINT32* pu32Address);
ERRCODE DrvSPU_GetEndAddress(E_DRVSPU_CHANNEL eChannel, UINT32* pu32Address);	
ERRCODE DrvSPU_GetPauseAddress(E_DRVSPU_CHANNEL eChannel, UINT32* pu32Address);
ERRCODE DrvSPU_GetUserEventIndex(E_DRVSPU_CHANNEL eChannel, UINT8* pu8EventIndex, UINT8* pu8SubIndex);
#ifdef OPT_DIRECT_SET_DFA
ERRCODE DrvSPU_SetDFA(E_DRVSPU_CHANNEL eChannel, UINT16 u16DFA);
#else
ERRCODE DrvSPU_SetDFA(E_DRVSPU_CHANNEL eChannel, UINT16 u16SrcSampleRate, UINT16 u16OutputSampleRate);	
#endif	
ERRCODE DrvSPU_GetDFA(E_DRVSPU_CHANNEL eChannel, UINT16* pu16DFA);
ERRCODE DrvSPU_SetPAN(E_DRVSPU_CHANNEL eChannel, UINT16 u16PAN);	
ERRCODE DrvSPU_GetPAN(E_DRVSPU_CHANNEL eChannel, UINT16* pu16PAN);
ERRCODE DrvSPU_SetSrcType(E_DRVSPU_CHANNEL eChannel, E_DRVSPU_FORMAT eDataFormat);
ERRCODE	DrvSPU_GetSrcType(E_DRVSPU_CHANNEL eChannel, UINT16* pu16SrcType);
ERRCODE	DrvSPU_SetChannelVolume(E_DRVSPU_CHANNEL eChannel, UINT8 u8Volume);
ERRCODE DrvSPU_GetChannelVolume(E_DRVSPU_CHANNEL eChannel, UINT8* pu8Volume);
ERRCODE DrvSPU_SetChannelTone(E_DRVSPU_CHANNEL eChannel, S_TONE_CTRL* psToneCtrl);
ERRCODE DrvSPU_GetChannelTone(E_DRVSPU_CHANNEL eChannel, S_TONE_CTRL* psToneCtrl);
void DrvSPU_EqOpen(E_DRVSPU_EQ_BAND eEQBAND, E_DRVSPU_EQ_GAIN eEQGAIN);
void DrvSPU_EqClose(void);
void DrvSPU_SetVolume(UINT16 u16Volume);
void DrvSPU_GetVolume(UINT16* pu16Volume);
void DrvSPU_StartPlay(void);
void DrvSPU_StopPlay(void);
BOOL DrvSPU_IsSPUPlaying(void);
UINT32 DrvSPU_SetSampleRate(E_DRVSPU_SAMPLING eSampleRate);	
ERRCODE	DrvSPU_UploadChannelContents(E_DRVSPU_CHANNEL eChannel);
ERRCODE DrvSPU_ChannelCtrl(S_CHANNEL_CTRL *psChannelCtrl);	
ERRCODE	DrvSPU_ChannelPause(E_DRVSPU_CHANNEL eChannel);
ERRCODE	DrvSPU_ChannelResume(E_DRVSPU_CHANNEL eChannel);	
ERRCODE DrvSPU_SetToneAmp(E_DRVSPU_CHANNEL eChannel, UINT32 u32Amp);
ERRCODE DrvSPU_SetTonePulse(E_DRVSPU_CHANNEL eChannel, UINT32 u32Pulse);
UINT8 DrvSPU_ReadDACReg(UINT8 DACRegIndex);
VOID DrvSPU_WriteDACReg(UINT8 DACRegIndex, UINT8 DACRegData);
void DrvSPU_IntHandler(void);

void spuSwitchVolume(int uDirection)		// 1: for speaker -> earphone; 0: for earphone -> speaker
{
	int currentVolume, ii, jj;

#ifdef __KLE_ONLY
    if (uDirection==0) 		// earphone --> speaker
    {
		ucVol_R = DrvSPU_ReadDACReg(0x0A);			    	
		for (ii= 0; ii<11; ii++)
		{
			if(ucVol_R >= ear_Digi_table[ii])
			{
				ucVol_R = 10 - ii;
				break;
			}			
		}		
	
		ucVol_L = DrvSPU_ReadDACReg(0x09);
		for (ii= 0; ii<11; ii++)
		{
			if(ucVol_L >= ear_Digi_table[ii])
			{
				ucVol_L = 10 - ii;
				break;
			}			
		}		
		
		r_ucVol_L = sp_Digi_table[10-ucVol_L];
		r_ucVol_R = sp_Digi_table[10-ucVol_R];
		DrvSPU_WriteDACReg(0x09, (UINT8)r_ucVol_L);
		DrvSPU_WriteDACReg(0x0A, (UINT8)r_ucVol_R);	

		r_ucVol_L = sp_Ana_table[10-ucVol_L];
		r_ucVol_R = sp_Ana_table[10-ucVol_R];
		DrvSPU_WriteDACReg(0x00, (UINT8)r_ucVol_L);
		DrvSPU_WriteDACReg(0x01, (UINT8)r_ucVol_R);	
	}
	else				// speaker --> earphone
	{
		ucVol_R = DrvSPU_ReadDACReg(0x0A);				
		for (ii= 0; ii<11; ii++)
		{
			if(ucVol_R >= sp_Digi_table[ii])
			{
				ucVol_R = 10 - ii;
				break;
			}			
		}		
	
		ucVol_L = DrvSPU_ReadDACReg(0x09);
		for (ii= 0; ii<11; ii++)
		{
			if(ucVol_L >= sp_Digi_table[ii])
			{
				ucVol_L = 10 - ii;
				break;
			}			
		}		
		
		r_ucVol_L = ear_Digi_table[10-ucVol_L];
		r_ucVol_R = ear_Digi_table[10-ucVol_R];
		DrvSPU_WriteDACReg(0x09, (UINT8)r_ucVol_L);
		DrvSPU_WriteDACReg(0x0A, (UINT8)r_ucVol_R);	

		r_ucVol_L = ear_Ana_table[10-ucVol_L];
		r_ucVol_R = ear_Ana_table[10-ucVol_R];
		DrvSPU_WriteDACReg(0x00, (UINT8)r_ucVol_L);
		DrvSPU_WriteDACReg(0x01, (UINT8)r_ucVol_R);	
	}		    	

#else
	jj = 0xff;
	currentVolume = DrvSPU_ReadDACReg(0x0A);		
	
	for (ii=0; ii<32; ii++)
	{
		if (currentVolume >= N_table[ii])
		{
			jj = ii;
			break;
		}
	}
	
	if (jj==0xff)
		jj = 0;
			
	switch(uDirection)
	{
		case 0:			// for earphone -> speaker
			DrvSPU_WriteDACReg(0x00, 1);		// -2dB
			DrvSPU_WriteDACReg(0x01, 1);
			jj++;
			if (jj==32)
				jj = 31;	

			break;		
		
		case 1:			// for speaker -> earphone
		default:
			DrvSPU_WriteDACReg(0x00, 3);		// -6dB
			DrvSPU_WriteDACReg(0x01, 3);
			if (jj!=0)
				jj--;;	
	
			break;
	}		

	DrvSPU_WriteDACReg(0x09, N_table[jj]);
	DrvSPU_WriteDACReg(0x0A, N_table[jj]);
#endif	
}

void spuSetSampleRate(UINT32 u32SampleRate)
{	
	DrvSPU_SetSampleRate((E_DRVSPU_SAMPLING)u32SampleRate);
}

void spuSetBaseAddress(UINT32 u32BaseAddress)
{
	DrvSPU_SetBaseAddress((E_DRVSPU_CHANNEL)0, u32BaseAddress);	
	DrvSPU_SetBaseAddress((E_DRVSPU_CHANNEL)1, u32BaseAddress);	
}

UINT32 spuGetBaseAddress(void)
{
	UINT32 u32Address;
	
	DrvSPU_GetBaseAddress((E_DRVSPU_CHANNEL)0, &u32Address);
	return u32Address;
}

void spuSetThresholdAddress(UINT32 u32TH1Address)
{
	DrvSPU_SetThresholdAddress((E_DRVSPU_CHANNEL)0, u32TH1Address);	
	DrvSPU_SetThresholdAddress((E_DRVSPU_CHANNEL)1, u32TH1Address);	
}

UINT32 spuGetThresholdAddress(void)
{
	UINT32 u32Address;
	
	DrvSPU_GetThresholdAddress((E_DRVSPU_CHANNEL)0,&u32Address);
	return u32Address;	
}

void spuSetEndAddress(UINT32 u32TH2Address)
{
	DrvSPU_SetEndAddress((E_DRVSPU_CHANNEL)0, u32TH2Address);	
	DrvSPU_SetEndAddress((E_DRVSPU_CHANNEL)1, u32TH2Address);
}

UINT32 spuGetEndAddress(void)
{
	UINT32 u32Address;
	
	DrvSPU_GetEndAddress((E_DRVSPU_CHANNEL)0,&u32Address);
	return u32Address;	
}

UINT32 spuGetCurrentAddress(void)
{
	UINT32 u32Address;
	
	DrvSPU_GetCurrentAddress((E_DRVSPU_CHANNEL)0,&u32Address);
	return u32Address;		
}

void spuSetStereo(void)
{
	DrvSPU_SetSrcType((E_DRVSPU_CHANNEL)0, eDRVSPU_STEREO_PCM16_LEFT);		// Stereo PCM16 left	
	DrvSPU_SetSrcType((E_DRVSPU_CHANNEL)1, eDRVSPU_STEREO_PCM16_RIGHT);		// Stereo PCM16 right	
}

void spuSetMono(void)
{
	DrvSPU_SetSrcType((E_DRVSPU_CHANNEL)0, eDRVSPU_MONO_PCM16);		// Mono PCM16	
	DrvSPU_SetSrcType((E_DRVSPU_CHANNEL)1, eDRVSPU_MONO_PCM16);		// Mono PCM16	
}

BOOL spuIsMono(void)
{
	UINT16 u16SrcType;
	
	DrvSPU_GetSrcType((E_DRVSPU_CHANNEL)0, &u16SrcType);

	if (u16SrcType == eDRVSPU_MONO_PCM16)
		return TRUE;
		
	return FALSE;				
}

void spuSetVolume(UINT16 u16CHRVolume, UINT16 u16CHLVolume)
{
#ifdef __KLE_ONLY
	volatile int ucLeftVol, ucRightVol;
	volatile int r_ucLeftVol, r_ucRightVol;	

	if(u16CHRVolume >=100)
		u16CHRVolume = 100;

	if(u16CHLVolume >=100)
		u16CHLVolume = 100;
		
	ucLeftVol = u16CHLVolume/10;
	ucRightVol = u16CHRVolume/10;

    if (inp32(REG_GPIOG_PIN) & 0x0080) 		// GPIOG_7, headset plugged in
    {			
		r_ucLeftVol = ear_Digi_table[10-ucLeftVol];
		r_ucRightVol = ear_Digi_table[10-ucRightVol];
		DrvSPU_WriteDACReg(0x09, (UINT8)r_ucLeftVol);
		DrvSPU_WriteDACReg(0x0A, (UINT8)r_ucRightVol);	

		r_ucLeftVol = ear_Ana_table[10-ucLeftVol];
		r_ucRightVol = ear_Ana_table[10-ucRightVol];
		DrvSPU_WriteDACReg(0x00, (UINT8)r_ucLeftVol);
		DrvSPU_WriteDACReg(0x01, (UINT8)r_ucRightVol);	
	}
	else					// headset plugged out
	{
		r_ucLeftVol = sp_Digi_table[10-ucLeftVol];
		r_ucRightVol = sp_Digi_table[10-ucRightVol];
		DrvSPU_WriteDACReg(0x09, (UINT8)r_ucLeftVol);
		DrvSPU_WriteDACReg(0x0A, (UINT8)r_ucRightVol);	

		r_ucLeftVol = sp_Ana_table[10-ucLeftVol];
		r_ucRightVol = sp_Ana_table[10-ucRightVol];
		DrvSPU_WriteDACReg(0x00, (UINT8)r_ucLeftVol);
		DrvSPU_WriteDACReg(0x01, (UINT8)r_ucRightVol);	
	}		
#else	
        // spu reg_0x09 can't be different from 0x80 (20140312)
    	if(u16CHRVolume >=100)
    		u16CHRVolume = 100;

    	if(u16CHLVolume >=100)
    		u16CHLVolume = 100;

        u16CHRVolume *= 0x1F;
        u16CHRVolume /= 100;        
        u16CHRVolume = 0x1F - u16CHRVolume;
        u16CHLVolume *= 0x1F;
        u16CHLVolume /= 100;       
        u16CHLVolume = 0x1F - u16CHLVolume;

    	DrvSPU_WriteDACReg(0x01, (u16CHRVolume & 0xFF));	
    	DrvSPU_WriteDACReg(0x00, (u16CHLVolume & 0xFF));	    	
#endif
}

void spuSetPowerDown(UINT16 u16PowerDown)
{
	UINT8 u8Ret;
	
	u8Ret = u16PowerDown & 0xFF;
	DrvSPU_WriteDACReg(0x05, u8Ret);	
}

UINT16 spuGetPowerDown(void)
{
	UINT8 u8Ret;

	u8Ret = DrvSPU_ReadDACReg(0x05);
	return u8Ret;
}

UINT8 DacOnOffLevel;

//must use this function before calling spuStartPlay()
VOID spuDacOn(UINT8 level)
{
	DacOnOffLevel = level;	

	// enable SPU engine clock 
	outp32(REG_AHBCLK, inp32(REG_AHBCLK) | ADO_CKE | SPU_CKE | HCLK4_CKE);			// enable SPU engine clock 
	
	DrvSPU_WriteDACReg(0x08, 0x00);     // reset digital interface

	DrvSPU_WriteDACReg(0x08, 0x02);	

	DrvSPU_WriteDACReg(0x00, 0x1F);
	
	DrvSPU_WriteDACReg(0x01, 0x1F);	

	DrvSPU_WriteDACReg(0x07, 0x01);	

	DrvSPU_WriteDACReg(0x09, 0x64);	

	DrvSPU_WriteDACReg(0x0A, 0x64);	
	
	// 2013/10/24, enable ADC VMID bit in NandLoader/SDLoader to remove the second pop-noise.
    outp32(REG_APBCLK, inp32(REG_APBCLK) | ADC_CKE);
    outp32(REG_CLKDIV3, (inp32(REG_CLKDIV3) & ~(ADC_N1 | ADC_S| ADC_N0)) );                                         /* Fed to ADC clock need 12MHz=External clock */
    outp32(REG_SDADC_CTL, 0x308121bf);      // enable ADC VMID
	DrvSPU_WriteDACReg(0x05, 0xDF);	
}

//must use this function after calling spuStopPlay()
VOID spuDacOff(void)
{
	DrvSPU_WriteDACReg(0x00, 0x1F);	
	DrvSPU_WriteDACReg(0x01, 0x1F);	
	sysDelay(50);
	DrvSPU_WriteDACReg(0x05, 0xFF);	
}

VOID spuSetDacSlaveMode(void)
{
	DrvSPU_WriteDACReg(0x05, 0x13);	
	DrvSPU_WriteDACReg(0x05, 0x10);	
	DrvSPU_WriteDACReg(0x00, 0x05);	
	DrvSPU_WriteDACReg(0x01, 0x05);	
	DrvSPU_WriteDACReg(0x08, 0x03);	
}

VOID spuStartPlay(PFN_DRVSPU_CB_FUNC *fnCallBack, UINT8 *data)
{	
	DrvSPU_EnableInt(eDRVSPU_CHANNEL_0, DRVSPU_THADDRESS_INT, (PFN_DRVSPU_CB_FUNC*) fnCallBack);
	DrvSPU_EnableInt(eDRVSPU_CHANNEL_0, DRVSPU_ENDADDRESS_INT, (PFN_DRVSPU_CB_FUNC*) fnCallBack);	
	memcpy(playbuffer, data, g_i32FragSize);
	DrvSPU_StartPlay();	
}

VOID spuStopPlay(void)
{
	int ii;     
	
	// enable SPU engine clock 
	outp32(REG_AHBCLK, inp32(REG_AHBCLK) | ADO_CKE | SPU_CKE | HCLK4_CKE);			// enable SPU engine clock 
	for (ii=0; ii<32; ii++)
	{
		DrvSPU_DisableInt((E_DRVSPU_CHANNEL)ii, DRVSPU_ENDADDRESS_INT); 
		DrvSPU_DisableInt((E_DRVSPU_CHANNEL)ii, DRVSPU_THADDRESS_INT);
	}
	DrvSPU_StopPlay();
	sysDisableInterrupt(IRQ_SPU);
}

VOID spuIoctl(UINT32 cmd, UINT32 arg0, UINT32 arg1)
{
	switch(cmd)
	{		
		case SPU_IOCTL_SET_VOLUME:
			spuSetVolume(arg0, arg1);
			break;
		case SPU_IOCTL_SET_MONO:
			spuSetMono();
			break;
		case SPU_IOCTL_SET_STEREO:
			spuSetStereo();
			break;			
		case SPU_IOCTL_GET_FRAG_SIZE:
			*((UINT32 *)arg0) = g_i32FragSize;
			break;
		case SPU_IOCTL_SET_FRAG_SIZE:
			if ( arg0 && (*((UINT32 *)arg0) <= FRAG_SIZE) )
			{
				g_i32FragSize = *((UINT32 *)arg0);
				spuSetThresholdAddress((UINT32)_pucPlayAudioBuff + (g_i32FragSize>>1) );	
				spuSetEndAddress((UINT32)_pucPlayAudioBuff + g_i32FragSize);				
			}		
			break;		
		default:
			break;
	}		
}

VOID spuOpen(UINT32 u32SampleRate)
{	
	_pucPlayAudioBuff = (UINT8 *)((UINT32)playbuffer | 0x80000000);
	spuStopPlay();	// SPU must be disabled before to enable again	

	// 1.Check I/O pins. If I/O pins are used by other IPs, return error code.
	// 2.Enable IP¡¦s clock
	// 3.Reset IP			
	DrvSPU_Open();	
	DrvSPU_SetPAN((E_DRVSPU_CHANNEL)0, 0x1f00);
	DrvSPU_SetPAN((E_DRVSPU_CHANNEL)1, 0x001f);
	DrvSPU_SetChannelVolume((E_DRVSPU_CHANNEL)0, 0x7F);
	DrvSPU_SetChannelVolume((E_DRVSPU_CHANNEL)1, 0x7F);
	DrvSPU_ChannelOpen((E_DRVSPU_CHANNEL)0);	
	DrvSPU_ChannelOpen((E_DRVSPU_CHANNEL)1);	
	DrvSPU_SetSampleRate((E_DRVSPU_SAMPLING)u32SampleRate);
	DrvSPU_SetSrcType((E_DRVSPU_CHANNEL)0, eDRVSPU_STEREO_PCM16_LEFT);	
	DrvSPU_SetSrcType((E_DRVSPU_CHANNEL)1, eDRVSPU_STEREO_PCM16_RIGHT);
	DrvSPU_SetDFA((E_DRVSPU_CHANNEL)0, 0x400);
	DrvSPU_SetDFA((E_DRVSPU_CHANNEL)1, 0x400);
	spuSetBaseAddress((UINT32)_pucPlayAudioBuff);
	spuSetThresholdAddress((UINT32)_pucPlayAudioBuff + HALF_FRAG_SIZE);	
	spuSetEndAddress((UINT32)_pucPlayAudioBuff + FRAG_SIZE);
	sysInstallISR(IRQ_LEVEL_1, IRQ_SPU, (PVOID)DrvSPU_IntHandler);	
	sysSetLocalInterrupt(ENABLE_IRQ);
	sysEnableInterrupt(IRQ_SPU);
}

VOID spuClose (void)
{
	sysDisableInterrupt(IRQ_SPU);
	DrvSPU_Close();
}

VOID spuEqOpen (E_DRVSPU_EQ_BAND eEqBand, E_DRVSPU_EQ_GAIN eEqGain)
{
	DrvSPU_EqOpen(eEqBand, eEqGain);
}

VOID spuEqClose (void)
{
	DrvSPU_EqClose();
}

VOID spuDacPLLEnable (void)
{
    DrvSPU_WriteDACReg(0x05, 0x3F);
}

VOID spuDacPrechargeEnable (void)
{
    // software reset DAC in I2S master
    DrvSPU_WriteDACReg(0x08, 0x00);	                   
    DrvSPU_WriteDACReg(0x08, 0x02);
        		
    // keep DAC ON and enable DAC channel
    DrvSPU_WriteDACReg(0x05, 0x33);   

    // set DAC analog volume to mute
    DrvSPU_WriteDACReg(0x00, 0x1f);	
    DrvSPU_WriteDACReg(0x01, 0x1f);	

    // set DAC bias enable
    DrvSPU_WriteDACReg(0x05, 0x13);	        
            
    // set PCHGL/PCHGR=1, precharge with big AC coupled Cap. (220uF) for Headphone(HP).  ? Reg_0x03 = 0x35
    DrvSPU_WriteDACReg(0x03, 0x35);	 
}

VOID spuADCVmidEnable (void)
{
    // enable ADC VMID
    outp32(REG_APBCLK, inp32(REG_APBCLK) | ADC_CKE);
    outp32(REG_CLKDIV3, (inp32(REG_CLKDIV3) & ~(ADC_N1 | ADC_S| ADC_N0)) );     // Fed to ADC clock need 12MHz=External clock 
    outp32(REG_SDADC_CTL, 0x308121bf);      // enable ADC VMID
   	while(inp32(REG_SDADC_CTL) & AR_BUSY);	
   	
   	// enable SPU DAC digital part
    DrvSPU_WriteDACReg(0x07, 0x01);	   	
}

VOID spuDacEnable (int volumeLevel)    
{
    // open power amplifer 
    DrvSPU_WriteDACReg(0x05, 0x10);
    DrvSPU_WriteDACReg(0x03, 0x05);
    
    // given analog and digital volume
    spuSetVolume(volumeLevel&0xFF, volumeLevel&0xFF);   // set reg_0x00 and reg_0x01           
    
	DrvSPU_WriteDACReg(0x09, 0x80);	
	DrvSPU_WriteDACReg(0x0A, 0x80);	
	  
    // software reset DAC is I2S slave mode 
    DrvSPU_WriteDACReg(0x08, 0x01);	                   
    DrvSPU_WriteDACReg(0x08, 0x03);
}
