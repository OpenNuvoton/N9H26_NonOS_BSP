/**************************************************************************//**
 * @file     N9H26_SDIO.h
 * @version  V3.00
 * @brief    N9H26 series SIC/SDIO driver header file
 *
 * SPDX-License-Identifier: Apache-2.0
 * @copyright (C) 2020 Nuvoton Technology Corp. All rights reserved.
*****************************************************************************/

#ifndef _N9H26_SDIO_H
#define _N9H26_SDIO_H

//#define OPT_SW_WP         // use GPA7 as software Write Protect pin; 1 is normal; 0 is protected

#define FMI_SDIO_CARD   0
#define FMI_SM_CARD     1
#define FMI_SDIO1_CARD  2

#define FMISDIO_ERR_ID              0xFFF00100

#define FMISDIO_TIMEOUT             (FMISDIO_ERR_ID|0x01)
#define FMISDIO_NO_MEMORY           (FMISDIO_ERR_ID|0x02)
/* SD error */
#define FMISDIO_NO_SD_CARD          (FMISDIO_ERR_ID|0x10)
#define FMISDIO_ERR_DEVICE          (FMISDIO_ERR_ID|0x11)
#define FMISDIO_SD_INIT_TIMEOUT     (FMISDIO_ERR_ID|0x12)
#define FMISDIO_SD_SELECT_ERROR     (FMISDIO_ERR_ID|0x13)
#define FMISDIO_SD_WRITE_PROTECT    (FMISDIO_ERR_ID|0x14)
#define FMISDIO_SD_INIT_ERROR       (FMISDIO_ERR_ID|0x15)
#define FMISDIO_SD_CRC7_ERROR       (FMISDIO_ERR_ID|0x16)
#define FMISDIO_SD_CRC16_ERROR      (FMISDIO_ERR_ID|0x17)
#define FMISDIO_SD_CRC_ERROR        (FMISDIO_ERR_ID|0x18)
#define FMISDIO_SD_CMD8_ERROR       (FMISDIO_ERR_ID|0x19)
#define FMISDIO_SD_DITO_ERROR       (FMISDIO_ERR_ID|0x1A)
#define FMISDIO_SD_RITO_ERROR       (FMISDIO_ERR_ID|0x1B)

typedef struct fmi_sdio_info_t
{
    UINT32  uCardType;      // sd2.0, sd1.1, or mmc
    UINT32  uRCA;           // relative card address
    BOOL    bIsCardInsert;
} FMI_SDIO_INFO_T;

extern FMI_SDIO_INFO_T *pSDIO0;
extern FMI_SDIO_INFO_T *pSDIO1;

/* extern function */
#define SDIO_SET_CLOCK          0
#define SDIO_SET_CALLBACK       1
#define SDIO_GET_CARD_STATUS    2
#define SDIO0_GET_CARD_STATUS   3
#define SDIO1_GET_CARD_STATUS   4
#define SDIO_SET_CARD_DETECT    5
#define SDIO0_SET_CARD_DETECT   6
#define SDIO1_SET_CARD_DETECT   7

extern INT32 g_SDIO0_card_detect;
extern INT32 g_SDIO1_card_detect;


// MMC run 20MHz under N9H26
#define SDIO_MMC_FREQ   20000

// eMMC run 26MHz under N9H26
#define SDIO_EMMC_FREQ  26000

// SD run 24MHz under N9H26
#define SDIO_SD_FREQ    24000

// SDHC run 24MHz under N9H26
#define SDIO_SDHC_FREQ  24000


// function prototype for SIC
void sdioOpen(void);
void sdioClose(void);
VOID sdioIoctl(INT32 sdioFeature, INT32 sdioArg0, INT32 sdioArg1, INT32 sdioArg2);

// function prototype for SIC/SDIO
INT sdioSdOpen(void);
INT sdioSdOpen0(void);
INT sdioSdOpen1(void);

VOID sdioSdClose(void);
VOID sdioSdClose0(void);
VOID sdioSdClose1(void);

INT sdioSdRead(INT32 sdSectorNo, INT32 sdSectorCount, INT32 sdTargetAddr);
INT sdioSdRead0(INT32 sdSectorNo, INT32 sdSectorCount, INT32 sdTargetAddr);
INT sdioSdRead1(INT32 sdSectorNo, INT32 sdSectorCount, INT32 sdTargetAddr);

INT sdioSdWrite(INT32 sdSectorNo, INT32 sdSectorCount, INT32 sdSourceAddr);
INT sdioSdWrite0(INT32 sdSectorNo, INT32 sdSectorCount, INT32 sdSourceAddr);
INT sdioSdWrite1(INT32 sdSectorNo, INT32 sdSectorCount, INT32 sdSourceAddr);

/* Declare callback function in waiting loop of SD driver */
#if defined (__GNUC__)
    __attribute__((weak)) void schedule(void);
#else
    __weak void schedule(void);
#endif

#endif  //end of _N9H26_SDIO_H
