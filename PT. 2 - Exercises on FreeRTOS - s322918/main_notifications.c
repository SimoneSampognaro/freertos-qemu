/*
Task notifications are a very powerful feature that can often be used in place of a binary
semaphore, a counting semaphore, an event group, and sometimes even a queue. This wide
range of usage scenarios can be achieved by using the xTaskNotify() API function to send a
task notification, and the xTaskNotifyWait() API function to receive a task notification.

In this exercise, we simulate a peripheral that sends a periodic notification to a task handler, which will consequently notify one or more tasks, according to the value of the variable event, 
eventually unblocking them and letting them read and print on the stdout the notification value. 
Task Notifications in FreeRTOS serve as a mechanism for tasks to communicate and interact with each other directly,
without relying on additional communication channels like queues or semaphores. 
When configUSE_TASK_NOTIFICATIONS is set to 1, each task has two main properties related to notifications: the "Notification State", 
which can assume two states value, "Pending" or "Not-Pending", and the "Notification Value", a 32-bit unsigned integer. 
When a task receives a notification its notification state is set to "Pending". Regarding the notification value, once read the notification state of the task is set to "Not-Pending".

If a task attempts to send a task notification to a task that already has a notification
pending, then it is not possible for the sending task to wait in the Blocked state for the
receiving task to reset its notification state.

Each task has a 32-bit notification value that is initialized to zero when the task is created.
xTaskNotify() is used to send an event directly to and potentially unblock a task, and optionally
update the receiving task’s notification value in one of the following ways:
- Write a 32-bit number to the notification value
- Add one (increment) the notification value
- Set one or more bits in the notification value
- Leave the notification value unchanged

*/
 

/* Standard includes. */
#include <stdio.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"


/* Priorities at which the tasks are created. */
#define high_prio_task	(tskIDLE_PRIORITY + 3)
#define medium_prio_task	(tskIDLE_PRIORITY + 2)
#define low_prio_task	(tskIDLE_PRIORITY + 1)

TaskHandle_t xPeripheral;
TaskHandle_t xTaskHandler;
TaskHandle_t xTask1;
TaskHandle_t xTask2;


static void fPeripheral(void *p_arg);
static void fHandler(void *p_arg);
static void fTask1(void *p_arg);
static void fTask2(void *p_arg);

void main_blinky( void )
{	
	xTaskCreate(fPeripheral,"peripheral",configMINIMAL_STACK_SIZE,NULL,medium_prio_task,&xPeripheral);
	xTaskCreate(fHandler,"handler",configMINIMAL_STACK_SIZE,NULL,high_prio_task,&xTaskHandler);
	xTaskCreate(fTask1,"tsk1",configMINIMAL_STACK_SIZE,NULL,low_prio_task,&xTask1);
	xTaskCreate(fTask2,"tsk2",configMINIMAL_STACK_SIZE,NULL,low_prio_task,&xTask2);

	vTaskStartScheduler();

	for( ;; );
}
/*-----------------------------------------------------------*/

static void fPeripheral(void *p_arg){

	(void) p_arg;
	TickType_t xLastWakeTime;
    const TickType_t xPeriod = pdMS_TO_TICKS(3000UL); // Every 3 seconds the peripheral send a notification to the handler
    xLastWakeTime = xTaskGetTickCount();

	for (; ; ){
		printf("Peripheral send notification to Handler! \r\n");
		xTaskNotifyGive(xTaskHandler);
		vTaskDelayUntil(&xLastWakeTime, xPeriod);
    }
}

static void fHandler(void *p_arg){

	(void) p_arg;

	int event=0;

	for ( ; ; ){ // Handler waits the peripheral notification
		xTaskNotifyWait(0,0,NULL,portMAX_DELAY);
		printf("Handler! \r\n");

		/*	
			eSetValueWithOverwrite:
			The receiving task’s notification value is set to the value
			passed in the xTaskNotify() ulValue parameter, regardless of
			whether the receiving task had a notification pending before
			xTaskNotify() was called or not.
		*/
		switch (event)
		{
		case 0: 
			xTaskNotify( xTask1,0,eSetValueWithOverwrite);
			xTaskNotify( xTask2,0,eSetValueWithOverwrite);
			break;
		case 1:
			xTaskNotify( xTask1,1,eSetValueWithOverwrite);
			break;
		case 2:
			xTaskNotify( xTask2,2,eSetValueWithOverwrite);
			break;
		default:
			break;
		}

		event++;
		if(event==3){
			event=0;
		}
    }

}

static void fTask1(void *p_arg){

	(void) p_arg;

	int *pNotificationValue;
	for ( ; ; ){ // Task wait untils the notification from handler
		xTaskNotifyWait(0,0,&pNotificationValue,portMAX_DELAY);
		printf("Task1, event %d \r\n",pNotificationValue);
		
    }

}

static void fTask2(void *p_arg){

	(void) p_arg;

	int *pNotificationValue;
	for ( ; ; ){ // Task wait untils the notification from handler
		xTaskNotifyWait(0,0,&pNotificationValue,portMAX_DELAY);
		printf("Task2, event %d \r\n",pNotificationValue);
		
    }

}

/*-----------------------------------------------------------*/
