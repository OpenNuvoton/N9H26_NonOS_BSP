/**************************************************************************//**
 * @file     spiloader.h
 * @brief    Spiloader header file
 *
 * SPDX-License-Identifier: Apache-2.0
 * @copyright (C) 2020 Nuvoton Technology Corp. All rights reserved.
*****************************************************************************/

/* Enable 4 bit mode */
//#define __OTP_4BIT__

//#define __RTC_HW_PWOFF__
//#define __No_RTC__
 
#ifdef __OTP_4BIT__
    #ifdef __Security__
        #define DATE_CODE   "20210609 4-Bit Mode with Security"
    #else
        #define DATE_CODE   "20210609 4-Bit Mode"
    #endif
#else
    #ifdef __Security__
        #define DATE_CODE   "20210609 with Security"    
    #else            
        #define DATE_CODE   "20210609"
    #endif            
#endif


#define IMAGE_BUFFER    0xA00000

/* Start for option for VPOST frame buffer */
#if defined(__TV__)
    #ifdef __TV_QVGA__ 
    #define PANEL_WIDTH        320
    #define PANEL_HEIGHT       240
    #else
    #define PANEL_WIDTH        640
    #define PANEL_HEIGHT       480
    #endif
#elif defined( __LCM_800x600__) 
    #define PANEL_WIDTH        800
    #define PANEL_HEIGHT       600
#elif defined( __LCM_480x272__)
    #define PANEL_WIDTH        480
    #define PANEL_HEIGHT       272
#elif defined( __LCM_800x480__)
    #define PANEL_WIDTH        800
    #define PANEL_HEIGHT       480
#elif defined( __LCM_QVGA__)
    #define PANEL_WIDTH        320
    #define PANEL_HEIGHT       240
#elif defined( __LCM_128x64__)
    #define PANEL_WIDTH        128
    #define PANEL_HEIGHT        64    
#elif defined(__LCM_1024x600__)
    #define PANEL_WIDTH       1024
    #define PANEL_HEIGHT       600
#else
    #define PANEL_WIDTH     480
    #define PANEL_HEIGHT    272
#endif
/* End for option for VPOST frame buffer */

#define PANEL_BPP        2
#define FB_ADDR          0x500000

 void SPI_OpenSPI(void);
 int SPIReadFast(BOOL bEDMAread, UINT32 addr, UINT32 len, UINT32 *buf);
 INT spiFlashFastReadQuads(UINT32 addr, UINT32 len, UINT32* buf);
 void JEDEC_Probe (void);
 
 /* Turn on the optional. Back light enable */
 /* Turn off the optional, ICE can connect to */
 /* Default Demo Board  GPD1 keep pull high */
 /*                                 */ 
//#define __BACKLIGHT__
 
