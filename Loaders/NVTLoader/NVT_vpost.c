/**************************************************************************//**
 * @file     NVT_vpost.c
 * @brief    VPOST initialize function
 *
 * SPDX-License-Identifier: Apache-2.0
 * @copyright (C) 2020 Nuvoton Technology Corp. All rights reserved.
*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "N9H26.h"
#include "nvtloader.h"
static BOOL bIsInitVpost=FALSE;
LCDFORMATEX lcdInfo;
void initVPostShowLogo(BOARD_S* ps_board)
{
#ifdef __HAVE_VPOST__
    if(ps_board->lcmpower_init != NULL)
        ps_board->lcmpower_init();
    if(ps_board->lcmpower_enable != NULL)
        ps_board->lcmpower_enable();
    if(bIsInitVpost==FALSE)
    {
        bIsInitVpost = TRUE;
        lcdInfo.ucVASrcFormat = DRVVPOST_FRAME_RGB565;
        lcdInfo.nScreenWidth = PANEL_WIDTH;
        lcdInfo.nScreenHeight = PANEL_HEIGHT;
        vpostLCMInit(&lcdInfo, (UINT32*)FB_ADDR);
        //vpostEnaBacklight();
    }
#endif
}
