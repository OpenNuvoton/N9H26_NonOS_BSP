/*
    FreeRTOS V8.2.3 - Copyright (C) 2015 Real Time Engineers Ltd.
    All rights reserved

    VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation >>>> AND MODIFIED BY <<<< the FreeRTOS exception.

    ***************************************************************************
    >>!   NOTE: The modification to the GPL is included to allow you to     !<<
    >>!   distribute a combined work that includes FreeRTOS without being   !<<
    >>!   obliged to provide the source code for proprietary components     !<<
    >>!   outside of the FreeRTOS kernel.                                   !<<
    ***************************************************************************

    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.  Full license text is available on the following
    link: http://www.freertos.org/a00114.html

    ***************************************************************************
     *                                                                       *
     *    FreeRTOS provides completely free yet professionally developed,    *
     *    robust, strictly quality controlled, supported, and cross          *
     *    platform software that is more than just the market leader, it     *
     *    is the industry's de facto standard.                               *
     *                                                                       *
     *    Help yourself get started quickly while simultaneously helping     *
     *    to support the FreeRTOS project by purchasing a FreeRTOS           *
     *    tutorial book, reference manual, or both:                          *
     *    http://www.FreeRTOS.org/Documentation                              *
     *                                                                       *
    ***************************************************************************

    http://www.FreeRTOS.org/FAQHelp.html - Having a problem?  Start by reading
    the FAQ page "My application does not run, what could be wrong?".  Have you
    defined configASSERT()?

    http://www.FreeRTOS.org/support - In return for receiving this top quality
    embedded software for free we request you assist our global community by
    participating in the support forum.

    http://www.FreeRTOS.org/training - Investing in training allows your team to
    be as productive as possible as early as possible.  Now you can receive
    FreeRTOS training directly from Richard Barry, CEO of Real Time Engineers
    Ltd, and the world's leading authority on the world's leading RTOS.

    http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
    including FreeRTOS+Trace - an indispensable productivity tool, a DOS
    compatible FAT file system, and our tiny thread aware UDP/IP stack.

    http://www.FreeRTOS.org/labs - Where new FreeRTOS products go to incubate.
    Come and try FreeRTOS+TCP, our new open source TCP/IP stack for FreeRTOS.

    http://www.OpenRTOS.com - Real Time Engineers ltd. license FreeRTOS to High
    Integrity Systems ltd. to sell under the OpenRTOS brand.  Low cost OpenRTOS
    licenses offer ticketed support, indemnification and commercial middleware.

    http://www.SafeRTOS.com - High Integrity Systems also provide a safety
    engineered and independently SIL3 certified version for use in safety and
    mission critical applications that require provable dependability.

    1 tab == 4 spaces!
*/

/* 
	NOTE : Tasks run in system mode and the scheduler runs in Supervisor mode.
	The processor MUST be in supervisor mode when vTaskStartScheduler is 
	called.  The demo applications included in the FreeRTOS.org download switch
	to supervisor mode prior to main being called.  If you are not using one of
	these demo application projects then ensure Supervisor mode is used.
*/


/*
 * Creates all the demo application tasks, then starts the scheduler.  The WEB
 * documentation provides more details of the demo application tasks.
 * 
 * Main.c also creates a task called "Check".  This only executes every three 
 * seconds but has the highest priority so is guaranteed to get processor time.  
 * Its main function is to check that all the other tasks are still operational.
 * Each task (other than the "flash" tasks) maintains a unique count that is 
 * incremented each time the task successfully completes its function.  Should 
 * any error occur within such a task the count is permanently halted.  The 
 * check task inspects the count of each task to ensure it has changed since
 * the last time the check task executed.  If all the count variables have 
 * changed all the tasks are still executing error free, and the check task
 * toggles the onboard LED.  Should any task contain an error at any time 
 * the LED toggle rate will change from 3 seconds to 500ms.
 *
 */

/* Standard includes. */
#include <stdlib.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"

/* Nuvoton includes. */
#include "wblib.h"

/* Demo application includes. */
#include "PollQ.h"
#include "BlockQ.h"
#include "semtest.h"
#include "dynamic.h"

/*-----------------------------------------------------------*/

/* Priorities for the demo application tasks. */
#define mainQUEUE_POLL_PRIORITY		( tskIDLE_PRIORITY + 2 )
#define mainBLOCK_Q_PRIORITY		( tskIDLE_PRIORITY + 2 )
#define mainSEM_TEST_PRIORITY		( tskIDLE_PRIORITY + 1 )
#define mainCHECK_TASK_PRIORITY		( tskIDLE_PRIORITY + 3 )

#define mainBLT_TASK_PRIORITY		( tskIDLE_PRIORITY + 0 )
void vStartBltTasks( UBaseType_t );

/* Constants used by the "check" task.  As described at the head of this file
the check task toggles an LED.  The rate at which the LED flashes is used to
indicate whether an error has been detected or not.  If the LED toggles every
3 seconds then no errors have been detected.  If the rate increases to 500ms
then an error has been detected in at least one of the demo application tasks. */
//#define mainCHECK_LED				( 7 )
#define mainNO_ERROR_FLASH_PERIOD	( ( TickType_t ) 3000 / portTICK_PERIOD_MS  )
#define mainERROR_FLASH_PERIOD		( ( TickType_t ) 500 / portTICK_PERIOD_MS  )

/*-----------------------------------------------------------*/

/*
 * Checks that all the demo application tasks are still executing without error
 * - as described at the top of the file.
 */
static long prvCheckOtherTasksAreStillRunning( void );

/*
 * The task that executes at the highest priority and calls 
 * prvCheckOtherTasksAreStillRunning().  See the description at the top
 * of the file.
 */
static void vErrorChecks( void *pvParameters );

/*
 * Configure the processor for use with the Keil demo board.  This is very
 * minimal as most of the setup is managed by the settings in the project
 * file.
 */
static void prvSetupHardware( void );

/*-----------------------------------------------------------*/



/*
 * Application entry point:
 * Starts all the other tasks, then starts the scheduler. 
 */
int main( void )
{
	/* Setup the hardware for use with the Keil demo board. */
	prvSetupHardware();

	sysprintf("start the FreeRTOS demo\n");

	/* Start the demo/test application tasks. */
	vStartPolledQueueTasks( mainQUEUE_POLL_PRIORITY );
	vStartBlockingQueueTasks( mainBLOCK_Q_PRIORITY );
	vStartSemaphoreTasks( mainSEM_TEST_PRIORITY );
	vStartDynamicPriorityTasks();

	vStartBltTasks( mainBLT_TASK_PRIORITY );

	/* Start the check task - which is defined in this file.  This is the task
	that periodically checks to see that all the other tasks are executing 
	without error. */
	xTaskCreate( vErrorChecks, "Check", configMINIMAL_STACK_SIZE, NULL, mainCHECK_TASK_PRIORITY, NULL );

	/* Now all the tasks have been started - start the scheduler.

	NOTE : Tasks run in system mode and the scheduler runs in Supervisor mode.
	The processor MUST be in supervisor mode when vTaskStartScheduler is 
	called.  The demo applications included in the FreeRTOS.org download switch
	to supervisor mode prior to main being called.  If you are not using one of
	these demo application projects then ensure Supervisor mode is used here. */
	vTaskStartScheduler();

	/* Should never reach here!  If you do then there was not enough heap
	available for the idle task to be created. */
	for( ;; );
}
/*-----------------------------------------------------------*/

static void vErrorChecks( void *pvParameters )
{
TickType_t xDelayPeriod = mainNO_ERROR_FLASH_PERIOD;

	/* Parameters are not used. */
	( void ) pvParameters;

	/* Cycle for ever, delaying then checking all the other tasks are still
	operating without error.  If an error is detected then the delay period
	is decreased from mainNO_ERROR_FLASH_PERIOD to mainERROR_FLASH_PERIOD so
	the on board LED flash rate will increase.

	This task runs at the highest priority. */

	for( ;; )
	{
		/* The period of the delay depends on whether an error has been 
		detected or not.  If an error has been detected then the period
		is reduced to increase the LED flash rate. */
		vTaskDelay( xDelayPeriod );

		if( prvCheckOtherTasksAreStillRunning() != pdPASS )
		{
			/* An error has been detected in one of the tasks - flash faster. */
			xDelayPeriod = mainERROR_FLASH_PERIOD;
		}

		/* Toggle the LED before going back to wait for the next cycle. */
		//vParTestToggleLED( mainCHECK_LED );
	}
}
/*-----------------------------------------------------------*/

static void prvSetupHardware( void )
{
	/* Perform the hardware setup required.  This is minimal as most of the
	setup is managed by the settings in the project file. */

	WB_UART_T uart;

	// initial MMU but disable system cache feature
	//sysEnableCache(CACHE_DISABLE);
	// Cache on
	sysEnableCache(CACHE_WRITE_BACK);

	uart.uart_no = WB_UART_1; 
	uart.uiFreq = EXTERNAL_CRYSTAL_CLOCK;	//use APB clock
	uart.uiBaudrate = 115200;
	uart.uiDataBits = WB_DATA_BITS_8;
	uart.uiStopBits = WB_STOP_BITS_1;
	uart.uiParity = WB_PARITY_NONE;
	uart.uiRxTriggerLevel = LEVEL_1_BYTE;
	sysInitializeUART(&uart);

    /********************************************************************************************** 
     * Clock Constraints: 
     * (a) If Memory Clock > System Clock, the source clock of Memory and System can come from
     *     different clock source. Suggestion MPLL for Memory Clock, UPLL for System Clock   
     * (b) For Memory Clock = System Clock, the source clock of Memory and System must come from 
     *     same clock source	 
     *********************************************************************************************/
#if 0 
    /********************************************************************************************** 
     * Slower down system and memory clock procedures:
     * If current working clock fast than desired working clock, Please follow the procedure below  
     * 1. Change System Clock first
     * 2. Then change Memory Clock
     * 
     * Following example shows the Memory Clock = System Clock case. User can specify different 
     * Memory Clock and System Clock depends on DRAM bandwidth or power consumption requirement. 
     *********************************************************************************************/
    sysSetSystemClock(eSYS_EXT, 12000000, 12000000);
    sysSetDramClock(eSYS_EXT, 12000000, 12000000);
#else 
    /********************************************************************************************** 
     * Speed up system and memory clock procedures:
     * If current working clock slower than desired working clock, Please follow the procedure below  
     * 1. Change Memory Clock first
     * 2. Then change System Clock
     * 
     * Following example shows to speed up clock case. User can specify different 
     * Memory Clock and System Clock depends on DRAM bandwidth or power consumption requirement.
     *********************************************************************************************/
    sysSetDramClock(eSYS_MPLL, 360000000, 360000000);
    sysSetSystemClock(eSYS_UPLL,            //E_SYS_SRC_CLK eSrcClk,
                      240000000,            //UINT32 u32PllKHz,
                      240000000);           //UINT32 u32SysKHz,
    sysSetCPUClock(240000000/2);
#endif

    // Message gets messed up with above.
    sysprintf("\n\n");

    sysSetLocalInterrupt(ENABLE_IRQ);
}
/*-----------------------------------------------------------*/

static long prvCheckOtherTasksAreStillRunning( void )
{
long lReturn = pdPASS;

	/* Check all the demo tasks (other than the flash tasks) to ensure
	that they are all still running, and that none of them have detected
	an error. */
	if( xArePollingQueuesStillRunning() != pdTRUE )
	{
		lReturn = pdFAIL;
	}

	if( xAreBlockingQueuesStillRunning() != pdTRUE )
	{
		lReturn = pdFAIL;
	}

	if( xAreSemaphoreTasksStillRunning() != pdTRUE )
	{
		lReturn = pdFAIL;
	}

	if( xAreDynamicPriorityTasksStillRunning() != pdTRUE )
	{
		lReturn = pdFAIL;
	}

	return lReturn;
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
	( void ) pcTaskName;
	( void ) pxTask;

	/* Run time stack overflow checking is performed if
	configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	function is called if a stack overflow is detected. */
	taskDISABLE_INTERRUPTS();
	for( ;; );
}
/*-----------------------------------------------------------*/

