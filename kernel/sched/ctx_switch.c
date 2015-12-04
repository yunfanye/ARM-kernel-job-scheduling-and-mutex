/** @file ctx_switch.c
 * 
 * @brief C wrappers around assembly context switch routines.
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-11-21
 */
 

#include <types.h>
#include <assert.h>

#include <config.h>
#include <kernel.h>
#include "sched_i.h"

#ifdef DEBUG_MUTEX
#include <exports.h>
#endif

static tcb_t* cur_tcb; /* use this if needed */

/**
 * @brief Initialize the current TCB and priority.
 *
 * Set the initialization thread's priority to IDLE so that anything
 * will preempt it when dispatching the first task.
 */
void dispatch_init(tcb_t* idle)
{
	cur_tcb = idle;
	runqueue_remove(idle->cur_prio);
	ctx_switch_half(&idle->context);
}


/**
 * @brief Context switch to the highest priority task while saving off the 
 * current task state. preempted.
 *
 * This function needs to be externally synchronized.
 * We could be switching from the idle task.  The priority searcher has been tuned
 * to return IDLE_PRIO for a completely empty run_queue case.
 */
void dispatch_save(void)
{
    /* Only switch to higher priority task (lower num), actually never 
     * called in current scenario
     */
    if(cur_tcb->cur_prio > highest_prio())
    {
        // Add current task to run queue
        runqueue_add(cur_tcb, cur_tcb->cur_prio);

        // Swap current task and the highest priority task in run queue
        tcb_t* new_tcb = runqueue_remove(highest_prio());
        tcb_t* old_tcb = cur_tcb;
        cur_tcb = new_tcb;

        // Switch to the highest priority task
        ctx_switch_full(&cur_tcb->context, &old_tcb->context);
    }
}

/**
 * @brief Context switch to the highest priority task that is not this task -- 
 * don't save the current task state.
 *
 * There is always an idle task to switch to.
 */
void dispatch_nosave(void)
{
    // Get the highest priority task in run queue
    tcb_t* new_tcb = runqueue_remove(highest_prio());
    cur_tcb = new_tcb;

    // Switch to the highest priority task
    ctx_switch_half(&new_tcb->context);
}


/**
 * @brief Context switch to the highest priority task that is not this task -- 
 * and save the current task but don't mark is runnable. finished.
 *
 * There is always an idle task to switch to.
 */
void dispatch_sleep(void)
{
	// Get the highest priority task in run queue
    tcb_t* new_tcb = runqueue_remove(highest_prio());
    tcb_t* old_tcb = cur_tcb;
    cur_tcb = new_tcb;
    ctx_switch_full(&cur_tcb->context, &old_tcb->context);
}

/**
 * @brief Returns the priority value of the current task.
 */
uint8_t get_cur_prio(void)
{
	return cur_tcb -> cur_prio;
}

/**
 * @brief Returns the TCB of the current task.
 */
tcb_t* get_cur_tcb(void)
{
	return cur_tcb;
}
