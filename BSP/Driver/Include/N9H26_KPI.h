/****************************************************************************
*                                                                           *
* Copyright (c) 2009 Nuvoton Tech. Corp. All rights reserved.               *
*                                                                           *
*****************************************************************************/

/****************************************************************************
* FILENAME
*   N9H26_KPI.h
*
* VERSION
*   1.0
*
* DESCRIPTION
*   KPI library header file
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
#include "N9H26_reg.h"

#define KPI_NONBLOCK	0
#define KPI_BLOCK		1

#define KEY_COUNT		6

#define KEY_UP		1
#define KEY_DOWN	2
#define KEY_LEFT	4
#define KEY_RIGHT	8
#define KEY_ENTER	16
#define KEY_ESC		32

extern void kpi_init(void);
extern int kpi_open(unsigned int src);
extern void kpi_close(void);
extern int kpi_read(unsigned char mode);
