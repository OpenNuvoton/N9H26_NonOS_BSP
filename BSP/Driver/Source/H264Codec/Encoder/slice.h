/**************************************************************************//**
 * @file     slice.h
 * @version  V3.00
 * @brief    N9H26 series H.264 driver header file
 *
 * SPDX-License-Identifier: Apache-2.0
 * @copyright (C) 2020 Nuvoton Technology Corp. All rights reserved.
*****************************************************************************/

#ifndef _SLICE_H_
#define _SLICE_H_

#include "N9H26_avcodec.h"
#include "common.h"
#include "port.h"

void start_nalu_header(h264_encoder *pEnc);
unsigned int slice_type(h264_encoder *pEnc);
void start_slice(h264_encoder *pEnc, int last);
void terminate_slice(h264_encoder *pEnc);

#endif

