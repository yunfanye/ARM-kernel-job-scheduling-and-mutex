/** @file sched.c
 * 
 * @brief Top level implementation of the scheduler.
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-11-20
 */

#include <types.h>
#include <assert.h>

#include <kernel.h>
#include <config.h>
#include "sched_i.h"

#include <arm/reg.h>
#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/physmem.h>

tcb_t system_tcb[OS_MAX_TASKS]; /*allocate memory for system TCBs */

void sched_init(task_t* main_task  __attribute__((unused)))
{
	sched_context_t* context;

	system_tcb[IDLE_PRIO].native_prio = IDLE_PRIO;
	system_tcb[IDLE_PRIO].cur_prio = IDLE_PRIO;
	system_tcb[IDLE_PRIO].holds_lock = 0;
	system_tcb[IDLE_PRIO].sleep_queue = NULL;

	context = &(system_tcb[IDLE_PRIO].context);
    context->r4 = (size_t)idle;
    context->r5 = 0;
    context->r6 = 0;
    context->r7 = 0;
    context->r8 = global_data;
    context->r9 = 0;
    context->r10 = 0;
    context->r11 = 0;
    context->sp = system_tcb[IDLE_PRIO].kstack_high;
    context->lr = launch_task;
}

/**
 * @brief This is the idle task that the system runs when no other task is runnable
 */
 
static void idle(void)
{
	 enable_interrupts();
	 while(1);
}

/**
 * @brief Allocate user-stacks and initializes the kernel contexts of the
 * given threads.
 *
 * This function assumes that:
 * - num_tasks < number of tasks allowed on the system.
 * - the tasks have already been deemed schedulable and have been appropriately
 *   scheduled.  In particular, this means that the task list is sorted in order
 *   of priority -- higher priority tasks come first.
 *
 * @param tasks  A list of scheduled task descriptors.
 * @param size   The number of tasks is the list.
 */
void allocate_tasks(task_t** tasks, size_t num_tasks)
{
	int i;
	sched_context_t* context;
	for(i = 0; i < OS_MAX_TASKS; i++) {
		system_tcb[i].native_prio = i;
		system_tcb[i].cur_prio = i;
		system_tcb[i].holds_lock = 0;
		system_tcb[i].sleep_queue = NULL;

		context = &(system_tcb[i].context);
	    context->r4 = (size_t)task->lambda;
	    context->r5 = (size_t)task->data;
	    context->r6 = (size_t)task->stack_pos;
	    context->r7 = 0;
	    context->r8 = global_data;
	    context->r9 = 0;
	    context->r10 = 0;
	    context->r11 = 0;
	    context->sp = system_tcb[i].kstack_high;
	    context->lr = launch_task;

		runqueue_add(&system_tcb[i], i);
	}
}

