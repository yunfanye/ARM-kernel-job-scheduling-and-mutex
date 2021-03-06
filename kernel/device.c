/**
 * @file device.c
 *
 * @brief Implements simulated devices.
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-12-01
 */


#include <types.h>
#include <assert.h>

#include <task.h>
#include <sched.h>
#include <device.h>
#include <arm/reg.h>
#include <arm/psr.h>
#include <arm/exception.h>

/**
 * @brief Fake device maintainence structure.
 * Since our tasks are periodic, we can represent 
 * tasks with logical devices. 
 * These logical devices should be signalled periodically 
 * so that you can instantiate a new job every time period.
 * Devices are signaled by calling dev_update 
 * on every timer interrupt. In dev_update check if it is 
 * time to create a tasks new job. If so, make the task runnable.
 * There is a wait queue for every device which contains the tcbs of
 * all tasks waiting on the device event to occur.
 */

extern unsigned long _time;

#define NULL	((void *)0)

struct dev
{
	tcb_t* sleep_queue;
	unsigned long   next_match;
};
typedef struct dev dev_t;

/* devices will be periodically signaled at the following frequencies */
const unsigned long dev_freq[NUM_DEVICES] = { 100, 200, 300, 400, 500, 600, 700, 800, 900, 1000, 1100, 1200, 1300, 1400, 1500, 1600, 1700, 1800, 1900, 2000, 2100, 2200, 2300, 2400, 2500, 2600, 2700, 2800, 2900, 3000, 3100, 3200, 3300, 3400, 3500, 3600, 3700, 3800, 3900, 4000, 4100, 4200, 4300, 4400, 4500, 4600, 4700, 4800, 4900, 5000, 5100, 5200, 5300, 5400, 5500, 5600, 5700, 5800, 5900, 9000};

static dev_t devices[NUM_DEVICES];

/**
 * @brief Initialize the sleep queues and match values for all devices.
 */
void dev_init(void)
{
	int i = 0;
	for(; i < NUM_DEVICES; i++)
	{
		devices[i].sleep_queue = NULL;
		devices[i].next_match = dev_freq[i] + _time;
	}
}


/**
 * @brief Puts a task to sleep on the sleep queue until the next
 * event is signalled for the device.
 *
 * @param dev  Device number.
 */
void dev_wait(unsigned int dev __attribute__((unused)))
{
	//??disable interrupt
	tcb_t * sleep_queue_current = devices[dev].sleep_queue;
	tcb_t * run_current = get_cur_tcb();
	run_current -> sleep_queue = NULL;
	if(devices[dev].sleep_queue == NULL)
	{
		devices[dev].sleep_queue = run_current;
	}
	else 
	{
		while(sleep_queue_current -> sleep_queue != NULL)
		{
			sleep_queue_current = sleep_queue_current -> sleep_queue;
		}
		sleep_queue_current -> sleep_queue = run_current;
	}
}
/*
dev_wait is called by tasks (through event_wait) when they want to wait for an event
-  E.g. when have finished execution of their current period and want to suspend themselves until the next period
*/
/**
 * @brief Signals the occurrence of an event on all applicable devices. 
 * This function should be called on timer interrupts to determine that 
 * the interrupt corresponds to the event frequency of a device. If the 
 * interrupt corresponded to the interrupt frequency of a device, this 
 * function should ensure that the task is made ready to run 
 */
void dev_update(unsigned long millis __attribute__((unused)))
{
	int i = 0;
	for(; i < NUM_DEVICES; i ++)
	{
		if(millis >= devices[i].next_match)
		{
			devices[i].next_match += dev_freq[i];
			tcb_t * sleep_queue_current = devices[i].sleep_queue;
			tcb_t * tmp;
			// put all tasks in the sleep queue to the run queue
			while(sleep_queue_current != NULL)
			{
				tmp = sleep_queue_current;
				runqueue_add(sleep_queue_current, sleep_queue_current -> cur_prio);
				sleep_queue_current = sleep_queue_current -> sleep_queue;
				tmp -> sleep_queue = NULL;
			}
			devices[i].sleep_queue = NULL;
		}
	}
}

/*dev_update should be called by your timer interrupt handler code
-  dev update should check whether the next event for every device has occurred
-  If the next event has occurred
!  Wake up all the tasks on this device s sleep_queue
!  Make these tasks ready to run
*/
