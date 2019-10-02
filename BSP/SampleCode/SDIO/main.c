/***************************************************************************
 * Copyright (c) 2013 Nuvoton Technology. All rights reserved.
 *
 * FILENAME
 *     main.c
 * DESCRIPTION
 *     The main file for SDIO demo code.
 **************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "N9H26.h"

/*-----------------------------------------------------------------------------
 * For test configuration
 *---------------------------------------------------------------------------*/
#define OK      TRUE
#define FAIL    FALSE

/* Basic unit for SD card is sector. One sector equal to 512 bytes. */
#define SECTOR_SIZE         512
/* Max sector number for one SD card access in this test. */
#define MAX_SECTOR_COUNT    100
/* RAM buffer size for one SD card access in this test. */
#define BUF_SIZE            (SECTOR_SIZE * MAX_SECTOR_COUNT)


/*-----------------------------------------------------------------------------
 * For global variables
 *---------------------------------------------------------------------------*/
#if defined (__GNUC__)
    UINT8 g_ram0[BUF_SIZE] __attribute__((aligned (32)));
    UINT8 g_ram1[BUF_SIZE] __attribute__((aligned (32)));
#else
    __align (32) UINT8 g_ram0[BUF_SIZE];
    __align (32) UINT8 g_ram1[BUF_SIZE];
#endif


/*-----------------------------------------------------------------------------
 * Show data by hex format
 *---------------------------------------------------------------------------*/
void show_hex_data(unsigned char *ptr, unsigned int length)
{
    unsigned int line_len = 8;
    unsigned int i;

    for (i=0; i<length; i++)
    {
        if (i % line_len == 0)
            sysprintf("        ");
        sysprintf("0x%02x ", *(ptr+i));
        if (i % line_len == line_len-1)
            sysprintf("\n");
    }
    if (i % line_len != 0)
        sysprintf("\n");
}


/*-----------------------------------------------------------------------------
 * To do test for SD card access. Write, read, and compare data on random sectors.
 *---------------------------------------------------------------------------*/
unsigned int sd_access_test(int sdport, int sectorNum)
{
    UINT32 ii, sectorIndex;
    UINT32 result;
    UINT32 u32SecCnt;
    UINT8  *ptr_g_ram0, *ptr_g_ram1;

    ptr_g_ram0 = (UINT8 *)((UINT32)g_ram0 | 0x80000000);    // non-cache
    ptr_g_ram1 = (UINT8 *)((UINT32)g_ram1 | 0x80000000);    // non-cache

    /* Initial random data for SD card access test */
    for(ii=0; ii<BUF_SIZE; ii++)
    {
        ptr_g_ram0[ii] = rand() & 0xFF;
    }

    /* Initial random sector number for SD card access test */
    sectorIndex = rand() % sectorNum;
    u32SecCnt   = rand() % MAX_SECTOR_COUNT;
    if (u32SecCnt == 0)
        u32SecCnt = 1;
    if (sectorIndex + u32SecCnt > sectorNum)
        sectorIndex = sectorNum - u32SecCnt;

    switch (sdport)
    {
        case 0: result = sdioSdWrite0(sectorIndex, u32SecCnt, (UINT32)ptr_g_ram0);   break;
        case 1: result = sdioSdWrite1(sectorIndex, u32SecCnt, (UINT32)ptr_g_ram0);   break;
    }
    sysprintf("    Write g_ram0 to SD card, result = 0x%x\n", result);
    show_hex_data(ptr_g_ram0, 16);

    memset(ptr_g_ram1, 0x5a, u32SecCnt * SECTOR_SIZE);
    switch (sdport)
    {
        case 0: result = sdioSdRead0(sectorIndex, u32SecCnt, (UINT32)ptr_g_ram1);    break;
        case 1: result = sdioSdRead1(sectorIndex, u32SecCnt, (UINT32)ptr_g_ram1);    break;
    }
    sysprintf("    Read back SD card to g_ram1, result = 0x%x\n", result);
    show_hex_data(ptr_g_ram1, 16);

    if(memcmp(ptr_g_ram0, ptr_g_ram1, u32SecCnt * SECTOR_SIZE) == 0)
    {
        result = OK;
        sysprintf("    Data compare OK at sector %d, sector count = %d\n", sectorIndex, u32SecCnt);
    }
    else
    {
        result = FAIL;
        sysprintf("    ERROR: Data compare ERROR at sector %d, sector count = %d\n", sectorIndex, u32SecCnt);
    }

    return result;
}


/*-----------------------------------------------------------------------------
 * Initial UART1.
 *---------------------------------------------------------------------------*/
void init_UART()
{
    UINT32 u32ExtFreq;
    WB_UART_T uart;

    u32ExtFreq = sysGetExternalClock();
    sysUartPort(1);
    uart.uiFreq = u32ExtFreq;   //use APB clock
    uart.uiBaudrate = 115200;
    uart.uiDataBits = WB_DATA_BITS_8;
    uart.uiStopBits = WB_STOP_BITS_1;
    uart.uiParity = WB_PARITY_NONE;
    uart.uiRxTriggerLevel = LEVEL_1_BYTE;
    uart.uart_no = WB_UART_1;
    sysInitializeUART(&uart);
}


/*-----------------------------------------------------------------------------
 * Initial Timer0 interrupt for system tick.
 *---------------------------------------------------------------------------*/
void init_timer()
{
    sysSetTimerReferenceClock(TIMER0, sysGetExternalClock());   // External Crystal
    sysStartTimer(TIMER0, 100, PERIODIC_MODE);                  // 100 ticks/per sec ==> 1tick/10ms
    sysSetLocalInterrupt(ENABLE_IRQ);
}


/*-----------------------------------------------------------------------------*/

int main(void)
{
    int result, i, pass, fail;
    int target_port = 1;    /* 0 for SDIO port 0; 1 for SDIO port 1 */
    int sectorNum;

    init_UART();
    init_timer();

    sysprintf("\n=====> N9H26 Non-OS SDIO Driver Sampe Code [tick %d] <=====\n", sysGetTicks(0));

#if defined (__GNUC__)
#else
    srand(time(NULL));
#endif

    //--- Initial system clock for SD
    sdioIoctl(SDIO_SET_CLOCK, sysGetPLLOutputHz(eSYS_UPLL, sysGetExternalClock())/1000, 0, 0);    // clock from PLL

    //--- Enable AHB clock for SDIO, interrupt ISR, DMA, and FMI engineer
    sdioOpen();

    //--- Initial SD card on target SDIO port
    switch (target_port)
    {
        case 0: sectorNum = sdioSdOpen0();    break;
        case 1: sectorNum = sdioSdOpen1();    break;
    }
    if (sectorNum < 0)
    {
        sysprintf("ERROR: Open SD card on SDIO port %d fail. Return = 0x%x.\n", target_port, sectorNum);
    }
    else
    {
        sysprintf("Detect SD card on SDIO port %d with %d sectors.\n", target_port, sectorNum);

        sysprintf("Do basic SDIO port %d access test ...\n", target_port);

        i = 0;
        pass = 0;
        fail = 0;
        while(i<10)
        {
            i++;
            sysprintf("=== Loop %d ...\n", i);
            result = sd_access_test(target_port, sectorNum);
            if (result == OK)
            {
                sysprintf("SDIO access test is SUCCESSFUL!!!\n");
                pass++;
            }
            else
            {
                sysprintf("SDIO access test is FAIL!!!\n");
                fail++;
            }
            sysprintf("=== Pass %d, Fail %d\n", pass, fail);
        }

        switch (target_port)
        {
            case 0: sdioSdClose0();    break;
            case 1: sdioSdClose1();    break;
        }
    }

    sysprintf("\n===== THE END ===== [tick %d]\n", sysGetTicks(0));
    while(1);
}
