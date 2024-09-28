/* 
In this exercise configUSE_MUTEXES is set to 1, due to the usage of mutexes to control access to a resource that is shared among three tasks,
a global variable: static int number_of_task_printing. 
These three tasks with different periods tries to acquire the mutex. The task able to access the critical section updates number_of_task_printing,
writes it on the stdout and releases the mutex. 
A semaphore that is used for mutual exclusion must always be returned!
This exercise also demonstrates one of the potential pitfalls of using a mutex to provide mutual exclusion,
the priority inversion scenario. In fact, the higher priority task has to wait for the lower priority task to give up control of the mutex.
FreeRTOS employs a mechanism called priority inheritance to handle priority inversion: by temporarily elevating the priority of the lower-priority task to a value that is at least as high as any task waiting for that resource, ensuring that it can promptly release the resource without being preempted itself.

xTaskPriorityInherit(), line 4209 task.c
*/

/* Standard includes. */
#include <stdio.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"

/* Priorities at which the tasks are created. */
#define high_prio_task	(tskIDLE_PRIORITY + 3)
#define medium_prio_task	(tskIDLE_PRIORITY + 2)
#define low_prio_task	(tskIDLE_PRIORITY + 1)

static int number_of_task_printing=0;

static SemaphoreHandle_t xMutex = NULL;

static void fHigh(void *p_arg);
static void fMedium(void *p_arg);
static void fLow(void *p_arg);
static void taskJob();

void main_blinky( void )
{	
	xTaskCreate(fHigh,"tskHigh",configMINIMAL_STACK_SIZE,NULL,high_prio_task,NULL);
	xTaskCreate(fMedium,"tskMedium",configMINIMAL_STACK_SIZE,NULL,medium_prio_task,NULL);
	xTaskCreate(fLow,"tskLow",configMINIMAL_STACK_SIZE,NULL,low_prio_task,NULL);

	xMutex = xSemaphoreCreateMutex();

	vTaskStartScheduler();

	for( ;; );
}
/*-----------------------------------------------------------*/

static void fHigh(void *p_arg){

	(void) p_arg;
	TickType_t xLastWakeTime;
    const TickType_t xPeriod = pdMS_TO_TICKS(2000UL);
    xLastWakeTime = xTaskGetTickCount();


	for ( ; ; ){
		xSemaphoreTake(xMutex,portMAX_DELAY);
		number_of_task_printing=3;
		printf("task %d with high prio hold mutex! \r\n",number_of_task_printing);
		taskJob();
		xSemaphoreGive(xMutex);
		vTaskDelayUntil(&xLastWakeTime, xPeriod);
    }
}

static void fMedium(void *p_arg){

	(void) p_arg;

	TickType_t xLastWakeTime;
    const TickType_t xPeriod = pdMS_TO_TICKS(3000UL);
    xLastWakeTime = xTaskGetTickCount();

	for ( ; ; ){
		xSemaphoreTake(xMutex,portMAX_DELAY);
		number_of_task_printing=2;
		printf("task %d with medium prio hold mutex! \r\n",number_of_task_printing);
		taskJob();
		xSemaphoreGive(xMutex);
		vTaskDelayUntil(&xLastWakeTime, xPeriod);
    }

}

static void fLow(void *p_arg){

	(void) p_arg;

	TickType_t xLastWakeTime;
    const TickType_t xPeriod = pdMS_TO_TICKS(1000UL);
    xLastWakeTime = xTaskGetTickCount();

	for ( ; ; ){
		xSemaphoreTake(xMutex,portMAX_DELAY);
		number_of_task_printing=1;
		printf("task %d with low prio hold mutex! \r\n",number_of_task_printing);
		taskJob();
		xSemaphoreGive(xMutex);
		vTaskDelayUntil(&xLastWakeTime, xPeriod);
    }

}

static void taskJob(){
	for(int i=0;i<100000000;i++);
}


/*-----------------------------------------------------------*/
