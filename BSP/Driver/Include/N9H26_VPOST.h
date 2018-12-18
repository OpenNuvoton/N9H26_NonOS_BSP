/***************************************************************************
 *                                                                         *
 * Copyright (c) 2007 - 2009 Nuvoton Technology Corp. All rights reserved.*
 *                                                                         *
 ***************************************************************************/
 
/****************************************************************************
 * 
 * FILENAME
 *     N9H26K_vpost.h
 *
 * VERSION
 *     0.1 
 *
 * DESCRIPTION
 *
 *
 *
 *
 * DATA STRUCTURES
 *     None
 *
 * FUNCTIONS
 *
 *
 *     
 * HISTORY
 *     2009.03.16		Created by Shu-Ming Fan
 *
 *
 * REMARK
 *     None
 *
 *
 **************************************************************************/
 
#ifndef _N9H26K_VPOST_H_
#define _N9H26K_VPOST_H_

#include "wbtypes.h"
#include "wbio.h"
#include "wblib.h"


#define LCM_ERR_ID				0xFFF06000	/* LCM library ID */

/* error code */
#define ERR_NULL_BUF 			(LCM_ERR_ID | 0x04)	//error memory location
#define ERR_NO_DEVICE			(LCM_ERR_ID | 0x05)	//error no device
#define ERR_BAD_PARAMETER       (LCM_ERR_ID | 0x06) //error for bad parameter
#define ERR_POWER_STATE			(LCM_ERR_ID | 0x07)	//error power state control

// Frame buffer data selection
#define DRVVPOST_FRAME_RGB555     0x0   // RGB555
#define DRVVPOST_FRAME_RGB565     0x1   // RGB565
#define DRVVPOST_FRAME_RGBx888    0x2   // Dummy+x888
#define DRVVPOST_FRAME_RGB888x    0x3   // 888+Dummy
#define DRVVPOST_FRAME_CBYCRY     0x4   // Cb0  Y0  Cr0  Y1
#define DRVVPOST_FRAME_YCBYCR     0x5   // Y0  Cb0  Y1  Cr0
#define DRVVPOST_FRAME_CRYCBY     0x6   // Cr0  Y0  Cb0  Y1
#define DRVVPOST_FRAME_YCRYCB     0x7   // Y0  Cr0  Y1  Cb0

// LCD and TV source selection
#define	DRVVPOST_COLOR_LINEBUFFER       0x0 // Line Buffer
#define	DRVVPOST_COLOR_FRAMEBUFFER      0x1 // Frame Buffer
#define	DRVVPOST_COLOR_REGISTERSETTING  0x2 // Register Setting Color
#define	DRVVPOST_COLOR_COLORBAR         0x3 // Internal Color Bar

#define DRVVPOST_CLK_13_5MHZ	0x0       // TV Color Modulation Method (Using 13.5MHz)
#define DRVVPOST_CLK_27MHZ      0x1       // TV Color Modulation Method (Using 27MHz)
#define DRVVPOST_CLK_PLL        0x4       // TV Color Modulation Method (Using PLL output)

// MPU Command & Display Mode selection
#define DRVVPOST_MPU_CMD_MODE				0x1
#define DRVVPOST_MPU_NORMAL_MODE			0x0

// Data Format Transfer selection
#define DRVVPOST_DataFormatTransfer_RGB888	0x1
#define DRVVPOST_DataFormatTransfer_YCbCr	0x0

// VPOST interrupt Flag
#define DRVVPOST_HINT						0x1
#define DRVVPOST_VINT						0x2
#define DRVVPOST_TVFIELDINT					0x4
#define DRVVPOST_MPUCPLINT					0x10

// LCD data interface selection
#define DRVVPOST_LCDDATA_YUV422   0x0   // YUV422(CCIR601)
#define DRVVPOST_LCDDATA_RGB      0x1   // Dummy serial (R, G, B Dummy) (default)
#define DRVVPOST_LCDDATA_CCIR656  0x2   // CCIR656 interface
#define DRVVPOST_LCDDATA_RGBGBR   0x4   // Odd line data is RGB, even line is GBR
#define DRVVPOST_LCDDATA_BGRRBG   0x5   // Odd line data is BGR, even line is RBG
#define DRVVPOST_LCDDATA_GBRRGB   0x6   // Odd line data is GBR, even line is RGB
#define DRVVPOST_LCDDATA_RBGBGR   0x7   // Odd line data is RBG, even line is BGR

// LCD device type
#define DRVVPOST_LCD_HIGHRESOLUTION  0x0  // High Resolution mod
#define DRVVPOST_LCD_SYNCTYPE_TFT    0x1  // Sync-type TFT LCD
#define DRVVPOST_LCD_STNCTYPE_STN    0x2  // Sync-type Color STN LCD
#define DRVVPOST_LCD_MPUTYPE         0x3  // MPU-type LCD

// LCD information structure
typedef struct
{
    UINT32 ucVASrcFormat;         //User input Display source format
    UINT32 nScreenWidth;          //Driver output,LCD width
    UINT32 nScreenHeight;         //Driver output,LCD height
    UINT32 nFrameBufferSize;      //Driver output,Frame buffer size(malloc by driver)
    UINT8 ucROT90;                //Rotate 90 degree or not
}LCDFORMATEX,*PLCDFORMATEX;


typedef enum {
				eDRVVPOST_QVGA = 0x00, 
				eDRVVPOST_HVGA,
				eDRVVPOST_VGA,
				eDRVVPOST_D1,
				eDRVVPOST_480x272
											} E_DRVVPOST_TV_SIZE;

typedef enum {
				eDRVVPOST_HINT = 0x01, 
				eDRVVPOST_VINT = 0x02,
				eDRVVPOST_TVFIELDINT = 0x04,
				eDRVVPOST_MPUCPLINT = 0x10
											} E_DRVVPOST_INT;

typedef enum {
				eDRVVPOST_SYNC_TV = 0, 
				eDRVVPOST_ASYNC_TV
											} E_DRVVPOST_TIMING_TYPE;
											
typedef enum {
				eDRVVPOST_RESERVED = 0, 
				eDRVVPOST_FRAME_BUFFER,
				eDRVVPOST_REGISTER_SETTING,				
				eDRVVPOST_COLOR_BAR
											} E_DRVVPOST_IMAGE_SOURCE;
											
											
typedef enum {
				eDRVVPOST_DUPLICATED = 0, 
				eDRVVPOST_INTERPOLATION
											} E_DRVVPOST_IMAGE_SCALING;
											
typedef enum {
				eDRVVPOST_HIGH_RESOLUTINO_SYNC = 0,	// 16/18/24-bit sync type LCM
				eDRVVPOST_SYNC = 1, 				// 8-bit sync type LCM				
				eDRVVPOST_MPU = 3					// 8/9/16/18/24-bit MPU type LCM

											} E_DRVVPOST_LCM_TYPE;

typedef enum {
				eDRVVPOST_I80 = 0,	
				eDRVVPOST_M68
											} E_DRVVPOST_MPU_TYPE;
											
typedef enum {
				eDRVVPOST_SRGB_YUV422 = 0,	// CCIR601
				eDRVVPOST_SRGB_RGBDUMMY, 	// RGB Dummy Mode
				eDRVVPOST_SRGB_CCIR656,		// CCIR656
				eDRVVPOST_SRGB_RGBTHROUGH	// RGB ThroughMode				
											} E_DRVVPOST_8BIT_SYNCLCM_INTERFACE;

typedef enum {
				eDRVVPOST_CCIR656_360 = 0,	// CCIR656 320 mode
				eDRVVPOST_CCIR656_320 		// CCIR656 320 mode
											} E_DRVVPOST_CCIR656_MODE;

typedef enum {
				eDRVVPOST_YUV_BIG_ENDIAN = 0,	// Bigh endian
				eDRVVPOST_YUV_LITTLE_ENDIAN 	// Little endian
											} E_DRVVPOST_ENDIAN;

typedef enum {
				eDRVVPOST_SRGB_RGB = 0,		
				eDRVVPOST_SRGB_BGR, 		
				eDRVVPOST_SRGB_GBR,			
				eDRVVPOST_SRGB_RBG		
											} E_DRVVPOST_SERAIL_SYNCLCM_COLOR_ORDER;

typedef enum {
				eDRVVPOST_PRGB_16BITS = 0,		
				eDRVVPOST_PRGB_18BITS, 		
				eDRVVPOST_PRGB_24BITS			
											} E_DRVVPOST_PARALLEL_SYNCLCM_INTERFACE;

typedef enum {
				eDRVVPOST_SYNC_8BITS = 0,		
				eDRVVPOST_SYNC_9BITS,
				eDRVVPOST_SYNC_16BITS,				
				eDRVVPOST_SYNC_18BITS,		
				eDRVVPOST_SYNC_24BITS						
											} E_DRVVPOST_SYNCLCM_DATABUS;

typedef enum {
				eDRVVPOST_MPU_8_8 = 0,
				eDRVVPOST_MPU_2_8_8,
				eDRVVPOST_MPU_6_6_6,
				eDRVVPOST_MPU_8_8_8,
				eDRVVPOST_MPU_9_9,
				eDRVVPOST_MPU_16,
				eDRVVPOST_MPU_16_2,
				eDRVVPOST_MPU_2_16,
				eDRVVPOST_MPU_16_8,
				eDRVVPOST_MPU_18,
				eDRVVPOST_MPU_18_6,
				eDRVVPOST_MPU_24
											} E_DRVVPOST_MPULCM_DATABUS;


typedef enum {
				eDRVVPOST_FRAME_RGB555 = 0,		
				eDRVVPOST_FRAME_RGB565, 		
				eDRVVPOST_FRAME_RGBx888,
				eDRVVPOST_FRAME_RGB888x,
				eDRVVPOST_FRAME_CBYCRY,
				eDRVVPOST_FRAME_YCBYCR,
				eDRVVPOST_FRAME_CRYCBY,
				eDRVVPOST_FRAME_YCRYCB
											} E_DRVVPOST_FRAME_DATA_TYPE;

typedef struct {
				UINT16 u16HSize;  	 		// Horizontal size
				UINT16 u16VSize;    		// Vertical size
											} S_DRVVPOST_FRAME_SIZE;

typedef struct {
				UINT16 u16HSize;  	 		// Horizontal size
				UINT16 u16VSize;    		// Vertical size
											} S_DRVVPOST_SCALING_OUTPUT;

typedef struct {
				UINT8  u8PulseWidth;  	 	// Horizontal sync pulse width
				UINT16 u16BackPorch;    	// Horizontal back porch
				UINT16 u16FrontPorch;    	// Horizontal front porch
											} S_DRVVPOST_SYNCLCM_HTIMING;

typedef struct {
				UINT8 u8PulseWidth;   		// Vertical sync pulse width
				UINT8 u8BackPorch;    		// Vertical back porch
				UINT8 u8FrontPorch;    		// Vertical front porch
											} S_DRVVPOST_SYNCLCM_VTIMING;

typedef struct {
				UINT16 u16PixelPerLine;		// for Sync-type LCD or TV, specify the number of pixel clock in each line or row of screen
				UINT16 u16LinePerPanel;    	// for Sync-type LCD or TV, specify the number of active lines per screen
				UINT16 u16MPUPixelPerLine;  // for Mpu-type LCD, specify the number of pixel in each line or row of screen
											} S_DRVVPOST_SYNCLCM_WINDOW;

typedef enum {
				eDRVVPOST_DATA_8BITS = 0,		
				eDRVVPOST_DATA_9BITS,
				eDRVVPOST_DATA_16BITS,			
				eDRVVPOST_DATA_18BITS,					
				eDRVVPOST_DATA_24BITS		
											} E_DRVVPOST_DATABUS;

typedef struct {
				BOOL bIsVsyncActiveLow;		// Vsync polarity
				BOOL bIsHsyncActiveLow;		// Hsync polarity
				BOOL bIsVDenActiveLow;		// VDEN polarity				
				BOOL bIsDClockRisingEdge;		// VDEN polarity								
											} S_DRVVPOST_SYNCLCM_POLARITY;

// for MVsync & FrameMark signal
typedef struct {
				UINT16 u16LinePerPanel;    	// Specify the number of active lines per screen
				UINT16 u16PixelPerLine;   	// Specify the number of pixel in each line or row of screen
											} S_DRVVPOST_MPULCM_WINDOW; 	

typedef struct {
				UINT8  u8CSnF2DCt;			// CSn fall edge to Data change clock counter
				UINT8  u8WRnR2CSnRt;		// WRn rising edge to CSn rising  clock counter
				UINT8  u8WRnLWt;			// WR Low pulse clock counter
				UINT8  u8CSnF2WRnFt;		// Csn fall edge To WR falling edge clock counter
											} S_DRVVPOST_MPULCM_TIMING;


typedef struct {
				BOOL  	bIsSyncWithTV;				
				BOOL  	bIsVsyncSignalOut;
				BOOL  	bIsFrameMarkSignalIn;				
				E_DRVVPOST_IMAGE_SOURCE		eSource;
				E_DRVVPOST_LCM_TYPE			eType;
				E_DRVVPOST_MPU_TYPE			eMPUType;				
				E_DRVVPOST_MPULCM_DATABUS	eBus;
				S_DRVVPOST_MPULCM_WINDOW*	psWindow;			
				S_DRVVPOST_MPULCM_TIMING*	psTiming;
				
												} S_DRVVPOST_MPULCM_CTRL;

typedef struct 
{
	UINT16 u16VSize;			// Specify OSD vertical size
	UINT16 u16HSize;    		// Specify OSD vertical size

} S_DRVVPOST_OSD_SIZE;

typedef struct 
{
	UINT16 u16VSize;			// Specify OSD vertical size	
	UINT16 u16HSize;    		// Specify OSD vertical size	

	UINT16 u16VStart_1st;		// Specify 1st OSD bar Vertical START position
	UINT16 u16VEnd_1st;			// Specify 1st OSD bar Vertical END position
	UINT16 u16VOffset_2nd;		// Specify 2st OSD bar Vertiacal OFFSET position (2nd_Start - 1st_End)
	UINT16 u16HStart_1st;		// Specify 1st OSD bar Horizontal START position
	UINT16 u16HEnd_1st;			// Specify 1st OSD bar Horizontal END position
	UINT16 u16HOffset_2nd;		// Specify 2st OSD bar Horizontal OFFSET position (2nd_Start - 1st_End)

} S_DRVVPOST_OSD_POS;


typedef enum 
{
	eDRVVPOST_OSD_RGB555 = 8,		
	eDRVVPOST_OSD_RGB565 = 9, 		
	eDRVVPOST_OSD_RGBx888 = 10,
	eDRVVPOST_OSD_RGB888x = 11,
	eDRVVPOST_OSD_ARGB888 = 12,	
	eDRVVPOST_OSD_CB0Y0CR0Y1 = 0,
	eDRVVPOST_OSD_Y0CB0Y1CR0 = 1,	
	eDRVVPOST_OSD_CR0Y0CB0Y1 = 2,
	eDRVVPOST_OSD_Y0CR0Y1CB0 = 3,	
	eDRVVPOST_OSD_Y1CR0Y0CB0 = 4,		
	eDRVVPOST_OSD_CR0Y1CB0Y0 = 5,			
	eDRVVPOST_OSD_Y1CB0Y0CR0 = 6,		
	eDRVVPOST_OSD_CB0Y1CR0Y0 = 7
} E_DRVVPOST_OSD_DATA_TYPE;

typedef struct 
{
	BOOL	bIsOSDEnabled;
	UINT32	u32Address;
	E_DRVVPOST_OSD_DATA_TYPE 	eType;
	S_DRVVPOST_OSD_SIZE* 		psSize;
	S_DRVVPOST_OSD_POS* 		psPos;
} S_DRVVPOST_OSD_CTRL;


typedef struct 
{
	E_DRVVPOST_OSD_DATA_TYPE 	eType;
	S_DRVVPOST_OSD_SIZE* 		psSize;
	S_DRVVPOST_OSD_POS* 		psPos;
} S_DRVVPOST_OSD;

typedef enum 
{
	eDRVVPOST_OSD_TRANSPARENT_RGB565 = 0, 		
	eDRVVPOST_OSD_TRANSPARENT_YUV,
	eDRVVPOST_OSD_TRANSPARENT_RGB888
} E_DRVVPOST_OSD_TRANSPARENT_DATA_TYPE;

typedef enum 
{
	eDRVVPOST_LCD_QVGA = 0,	
	eDRVVPOST_LCD_HVGA,			
	eDRVVPOST_LCD_VGA,		
	eDRVVPOST_LCD_D1,		
	eDRVVPOST_LCD_SVGA,			
	eDRVVPOST_LCD_480x272,				
	eDRVVPOST_LCD_800x480,					
	eDRVVPOST_LCD_1024x680,					
	eDRVVPOST_LCD_1020x680,						
	eDRVVPOST_LCD_Other		
} E_DRVVPOST_LCD_SIZEE;

typedef int (*PFN_DRVVPOST_INT_CALLBACK)(UINT8*, UINT32);

/**********************************************************
 * 			Driver for External API Function List         *
 **********************************************************/
VOID vpostSetFrameBuffer(UINT32 pFramebuf);
VOID *vpostGetFrameBuffer(VOID);
INT32 vpostLCMDeinit(VOID);
INT32 vpostLCMInit(PLCDFORMATEX plcdformatex, UINT32 *pFramebuf);
void vpostSetOSD_Enable(void);
void vpostSetOSD_Disable(void);
void vpostSetOSD_Size(S_DRVVPOST_OSD_SIZE* psSize);
void vpostSetOSD_Pos(S_DRVVPOST_OSD_POS* psPos);
void vpostSetOSD_DataType(E_DRVVPOST_OSD_DATA_TYPE eType);
void vpostSetOSD_Transparent_Enable(void);
void vpostSetOSD_Transparent_Disable(void);
int vpostSetOSD_Transparent(E_DRVVPOST_OSD_TRANSPARENT_DATA_TYPE eType, UINT32 u32Pattern);
void vpostSetOSD_BaseAddress(UINT32 u32BaseAddress);

#endif   /* _N9H26K_VPOST_H_*/

