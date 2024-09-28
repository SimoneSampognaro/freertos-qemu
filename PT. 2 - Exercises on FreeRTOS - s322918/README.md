**ExNotifications**  
In this exercise, we simulate a peripheral that sends a periodic notification to a task handler, which will consequently notify one or more tasks, according to the value of the variable event, eventually unblocking them and letting them read and print on the stdout the notification value.
Task Notifications in FreeRTOS serve as a mechanism for tasks to communicate and interact with each other directly, without relying on additional communication channels like queues or semaphores.   
When configUSE_TASK_NOTIFICATIONS is set to 1, each task has two main properties related to notifications: the "Notification State", which can assume two states value, "Pending" or "Not-Pending", and the "Notification Value", a 32-bit unsigned integer.  
When a task receives a notification its notification state is set to "Pending". Regarding the notification value, once read the notification state of the task is set to "Not-Pending".  

``Peripheral send notification to Handler!``  
``Handler!``  
``Task1, event 0``  
``Task2, event 0``  
``Peripheral send notification to Handler!``  
``Handler!``  
``Task1, event 1``  

**ExSemaphores**  
In this exercise, we use three binary semaphore to effectively synchronize three tasks, the order of execution: T1 - T2 - T3. 
The binary semaphore can be considered conceptually as a queue with a length of one, the queue can contain a maximum of one item at any time, so is always either empty or full, hence binary.  
A consequence of the emptiness of the queue is the entrance of the task in the Blocked state. 
Task takes a binary semaphore using the xSemaphoreTake() function. If the semaphore is not available, the task will block until the semaphore becomes available.  
To give a binary semaphore, you use the xSemaphoreGive() function. This makes the semaphore available for another task to take.

``Task1``  
``Task2``  
``Task3``  

**ExMutex**  
In this exercise configUSE_MUTEXES is set to 1, due to the usage of mutexes to control access to a resource that is shared among three tasks, a global variable: static int number_of_task_printing.  
These three tasks with different periods try to acquire the mutex. The task able to access the critical section updates number_of_task_printing, writes it on the stdout and releases the mutex.  
A semaphore that is used for mutual exclusion must always be returned!  
This exercise also demonstrates one of the potential pitfalls of using a mutex to provide mutual exclusion, the priority inversion scenario.
In fact, the higher priority task has to wait for the lower priority task to give up control of the mutex.
FreeRTOS employs a mechanism called priority inheritance to handle priority inversion: by temporarily elevating the priority of the lower-priority task to a value that is at least as high as any task waiting for that resource, ensuring that it can promptly release the resource without being preempted itself.  
In order to demonstrate the priority inversion scenario, it is provided a FreeRTOSConfig.h with several macros for tracing execution flow and task states.  

``Task3 with high prio hold mutex!``  
``Task2 with medium prio hold mutex!``  
``Task1 with low prio hold mutex!``  

if configUSE_TRACE_FACILITY is set to 1:  

``Task1 with low prio hold mutex!``  
``Task moved to ready state: tskMedium, Tick count: 1563``  
``Task switched out: tskLow, Tick count: 1563``  
``Task switched in: tskMedium, Tick count: 1563``  
``Task moved to ready state: tskLow, Tick count: 1563``  
``Task switched out: tskMedium, Tick count: 1563``  
``Task switched in: tskLow, Tick count: 1563``  
``Task switched out: tskLow, Tick count: 1563``  
``The number of items currently in the queue: 1``
