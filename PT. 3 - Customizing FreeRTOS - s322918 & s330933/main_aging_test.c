/*
    Test file to ensure the correctness of the aging mechanism
*/

/* Standard includes. */
#include <stdio.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"

/* Priorities at which the tasks are created. */
#define mainTASK1_PRIORITY		( tskIDLE_PRIORITY + 3 )
#define	mainTASK2_PRIORITY		( tskIDLE_PRIORITY + 2 )
#define	mainTASK3_PRIORITY		( tskIDLE_PRIORITY + 1 )

static TaskHandle_t tsk1;

static void task1( void *pvParameters );
static void task2( void *pvParameters );
static void task3( void *pvParameters );
static void task4( void *pvParameters );

/*** SEE THE COMMENTS AT THE TOP OF THIS FILE ***/
void main_aging_test( void )
{
	/* Start the three tasks as described in the comments at the top of this file. */
	xTaskCreate( task1,			                /* The function that implements the task. */
				"TSK1", 						/* The text name assigned to the task - for debug only as it is not used by the kernel. */
				configMINIMAL_STACK_SIZE, 		/* The size of the stack to allocate to the task. */
				NULL, 							/* The parameter passed to the task - not used in this simple case. */
				mainTASK1_PRIORITY,             /* The priority assigned to the task. */
				&tsk1 );						/* The task handle is not required, so NULL is passed. */

	xTaskCreate( task2, "TSK2", configMINIMAL_STACK_SIZE, NULL, mainTASK2_PRIORITY, NULL );

	xTaskCreate( task3, "TSK3", configMINIMAL_STACK_SIZE, NULL, mainTASK3_PRIORITY, NULL );

	xTaskCreate( task4, "TSK4", configMINIMAL_STACK_SIZE, NULL, mainTASK3_PRIORITY, NULL );

	/* Start the tasks and timer running. */
	vTaskStartScheduler();

	/* If all is well, the scheduler will now be running, and the following
	line will never be reached.  If the following line does execute, then
	there was insufficient FreeRTOS heap memory available for the idle and/or
	timer tasks	to be created.  See the memory management section on the
	FreeRTOS web site for more details.  NOTE: This demo uses static allocation
	for the idle and timer tasks so this line should never execute. */
	for( ;; );
}
/*-----------------------------------------------------------*/


static void taskJob(){
	for(int i=0;i<100000000;i++);
}

static void task1( void *pvParameters )
{

	/* Prevent the compiler warning about the unused parameter. */
	( void ) pvParameters;

	for(;;){
		printf("t1\r\n");
		taskJob();
	}

}
/*-----------------------------------------------------------*/

static void task2( void *pvParameters )
{

	/* Prevent the compiler warning about the unused parameter. */
	( void ) pvParameters;

	for(;;){
		printf("t2\r\n");
		taskJob();
	}

}

static void task3( void *pvParameters )
{

	/* Prevent the compiler warning about the unused parameter. */
	( void ) pvParameters;

	for(;;){
		printf("t3\r\n");
		taskJob();
	}

}

static void task4( void *pvParameters )
{

	/* Prevent the compiler warning about the unused parameter. */
	( void ) pvParameters;

	for(;;){
		printf("t4\r\n");
		taskJob();
	}

}
