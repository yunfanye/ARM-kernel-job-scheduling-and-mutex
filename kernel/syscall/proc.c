/** @file proc.c
 * 
 * @brief Implementation of `process' syscalls
 *
 * @author Mike Kasick <mkasick@andrew.cmu.edu>
 * @date   Sun, 14 Oct 2007 00:07:38 -0400
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-11-12
 */

#include <exports.h>
#include <bits/errno.h>
#include <config.h>
#include <kernel.h>
#include <syscall.h>
#include <sched.h>

#include <arm/reg.h>
#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/physmem.h>
#include <device.h>


/**
 * @brief Sort all task using bubble sort
 */
void sort_tasks(task_t* tasks, size_t num_tasks)
{
    size_t i;
    size_t j;
    task_t tmp;
    for(i = 0; i < num_tasks; i++)
    {
        for(j = i + 1; j < num_tasks; j++)
        {
            if(tasks[i].T > tasks[j].T)
            {
            	/* SWAP two tasks */
            	tmp = tasks[i];
            	tasks[i] = tasks[j];
            	tasks[j] = tmp;
            }
        }
    }
}

int task_create(task_t* tasks, size_t num_tasks)
{
	if(num_tasks <= 0 || num_tasks > OS_AVAIL_TASKS)
        return -EINVAL;
	/* init devices and run queue */
	dev_init();
	runqueue_init();
	/* sort tasks, rate monotonic */
	sort_tasks(tasks, num_tasks);
	/* init scheduler */
	sched_init();
	allocate_tasks(tasks, num_tasks);

	/* switch from idle to task */
	dispatch_nosave();

	return 1; /* remove this line after adding your code */
}

int event_wait(unsigned int dev)
{
    tcb_t* cur_tcb = get_cur_tcb();
    if(dev >= NUM_DEVICES)
        return -EINVAL;

    if(cur_tcb->holds_lock >= 1)
        return -EHOLDSLOCK;

    // Puts the calling task to sleep on given device number
    dev_wait(dev);
    dispatch_sleep();
    
    return 0;
}

/* An invalid syscall causes the kernel to exit. */
void invalid_syscall(unsigned int call_num)
{
	printf("Kernel panic: invalid syscall -- 0x%08x\n", call_num);

	disable_interrupts();
	while(1);
}
