/*
Binary semaphores and mutexes are very similar, but do have some subtle differences.
Mutexes include a priority inheritance mechanism, binary semaphores do not. This makes
binary semaphores the better choice for implementing synchronization (between tasks or
between tasks and an interrupt), and mutexes the better choice for implementing simple
mutual exclusion.
*/
/*In this exercise, we use three binary semaphore to effectively synchronize three tasks, the order of execution: Repeat(T1 - T2 - T3).
The binary semaphore can be considered conceptually as a queue with a length of one, 
the queue can contain a maximum of one item at any time, so is always either empty or full (hence, binary). 
A consequence of the emptiness of the queue is the entrance of the task in the Blocked state. 
Task takes a binary semaphore using the xSemaphoreTake() function. If the semaphore is not available,
the task will block until the semaphore becomes available.
To give a binary semaphore, you use the xSemaphoreGive() function. This makes the semaphore available for another task to take. 
*/

/* Standard includes. */
#include <stdio.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/* Priorities at which the tasks are created. */
#define prio_task	(tskIDLE_PRIORITY + 2)

static SemaphoreHandle_t xSemaphoreBinary12 = NULL;
static SemaphoreHandle_t xSemaphoreBinary23 = NULL;
static SemaphoreHandle_t xSemaphoreBinary31 = NULL;

static void f1(void *p_arg);
static void f2(void *p_arg);
static void f3(void *p_arg);
static void taskJob();

void main_blinky( void )
{	
	xTaskCreate(f2,"Task1",configMINIMAL_STACK_SIZE,NULL,prio_task,NULL);
	xTaskCreate(f1,"Task2",configMINIMAL_STACK_SIZE,NULL,prio_task,NULL);
	xTaskCreate(f3,"Task3",configMINIMAL_STACK_SIZE,NULL,prio_task,NULL);

	xSemaphoreBinary12 = xSemaphoreCreateBinary();
	xSemaphoreBinary23 = xSemaphoreCreateBinary();
	xSemaphoreBinary31 = xSemaphoreCreateBinary();
	// Some Semaphore API functions are actually macros, not functions.

	xSemaphoreGive(xSemaphoreBinary31);
	/* The semaphore is created in the ‘empty’ state, meaning the semaphore must first be given
       before it can be taken (obtained) using the xSemaphoreTake() function. */

	vTaskStartScheduler();

	for( ;; );
}
/*-----------------------------------------------------------*/

static void f1(void *p_arg){

	(void) p_arg;

	while(1){
	if (xSemaphoreTake(xSemaphoreBinary31, portMAX_DELAY))
        {
            // Semaphore taken, perform Task1 operation
			printf("Task1\r\n");
			taskJob();
			xSemaphoreGive(xSemaphoreBinary12);
      		// Task1 will take the semaphore, perform its operations, give the semaphore to
			// Task2 and then may enter the Blocked state waiting for Task3 to give the semaphore again
        }
	}
}

static void f2(void *p_arg){

	(void) p_arg;

	while(1){
	if (xSemaphoreTake(xSemaphoreBinary12, portMAX_DELAY))
        {
            // Semaphore taken, perform Task2 operation
			printf("Task2\r\n");
			taskJob();
			xSemaphoreGive(xSemaphoreBinary23);
     		// Task2 will take the semaphore, perform its operations, give the semaphore to
			// Task3 and then may enter the Blocked state waiting for Task1 to give the semaphore again
        }
	}

}

static void f3(void *p_arg){

	(void) p_arg;

	while(1){
	if (xSemaphoreTake(xSemaphoreBinary23, portMAX_DELAY))
        {
            // Semaphore taken, perform Task3 operation
			printf("Task3\r\n");
			taskJob();
			xSemaphoreGive(xSemaphoreBinary31);
            // Task3 will take the semaphore, perform its operations, give the semaphore to
			// Task1 and then may enter the Blocked state waiting for Task2 to give the semaphore again
        }
	}

}

static void taskJob(){
	for(int i=0;i<100000000;i++);
}


/*-----------------------------------------------------------*/
