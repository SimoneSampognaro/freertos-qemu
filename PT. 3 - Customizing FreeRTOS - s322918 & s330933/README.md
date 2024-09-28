# **Aging mechanism in FreeRTOS**

### Disclaimer

**This customization is thought to work with the Prioritized Pre-emptive Scheduling with Time Slicing. Configuration constants to set:**

1. configUSE_PREEMPTION  1
2. configUSE_TIME_SLICING 1
---

### Implementation

In this Project we will change three files (tasks.c, FreeRTOS.h, FreeRTOSConfig.h):

- Update the TCB structure with two new parameters: *uint8_t age_counter, UBaseType_t uxOldPriority;* (Check line 324 in tasks.c)
- In the function *prvInitialiseNewTask, set age_counter to 0 and uxOldPriority equal to uxPriority;* (Check line 938 in task.c)
- Edit the xSTATIC_TCB structure in FreeRTOS.h to achieve a correct support of eventual runs with static allocations. (Check line 1305 in FreeRTOS.h)
- Add *configUSE_AGING and configRESET_PRIORITY* in FreeRTOSConfig.h and set them to 1 if aging with priority reset has to be used, otherwise it is possible to choose to use just Aging without priority reset mechanism;  (Check line 65 in FreeRTOSConfig.h)
- To gradually increase the priority of a task over time, two parameters are used: TICK_RATE_UPDATE (number of tick needed to increase by one *age_counter* ) and TICK_THRESHOLD (increase priority by one if *age_counter* is greater than this parameter);
- A new function is added: **xTaskPrioritySet**, it is in charge of modifying the priority of the task whose TCB is passed as a parameter to the function;
(Check line 2741 in tasks.c)
- In the function ***xTaskIncrementTick**,* if xTickCount is a multiple of TICK_RATE_UPDATE, increment *age_counter,* than if *age_counter >* TICK_THRESHOLD, set *age_counter* to zero and evaluate if the priority of the task can be increased; (Check line 2930 in tasks.c)
- In the function ***vTaskSwitchContext**,* set to zero the *age_counter* of the task that has been  switched out and the one that has been switched in, in order to avoid inconsistent priority and age_counter values (these two tasks had the opportunity to execute); 
Moreover, if configRESET_PRIORITY is equal to 1 in this function it is compulsory to perform the priority reset of the task that will be switched out. To do so, we save the pointer of the TCB of the task that will leave the CPU at the beginning of the function and right after the context switch we perform the reset of the priority. (Check lines 3137 and 3189 in tasks.c)

---

### Testing assessment and Considerations

These customizations are tested using the file main_aging_test.c in which there are four tasks:

- Task 1: it has the highest priority among the tasks.
- Task 2: it has a medium priority.
- Task 3-4: these last two have the same lowest priority.

Without aging, since the default scheduler behavior is priority based, we would expect only the task with the highest priority to run.
With aging, we ensure that the scheduler is preemptive and applies a time-slicing based scheduling algorithm (Round Robin), as the priority changes have been applied by the aging mechanism and all the tasks have the same priority level. 
It is also possible to use this mechanism with priority reset, which implies to reset the priority of a dispatched task which underwent a priority increase.
