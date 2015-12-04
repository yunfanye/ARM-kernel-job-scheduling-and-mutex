/**
 * @file mutex.c
 *
 * @brief Implements mutices.
 *
 * @author Harry Q Bovik < PUT YOUR NAMES HERE
 *
 * 
 * @date  
 */

//#define DEBUG_MUTEX

#include <lock.h>
#include <task.h>
#include <sched.h>
#include <bits/errno.h>
#include <arm/psr.h>
#include <arm/exception.h>
#ifdef DEBUG_MUTEX
#include <exports.h> // temp
#endif

mutex_t gtMutex[OS_NUM_MUTEX];

void mutex_init()
{
	int i = 0;
	for(; i < OS_NUM_MUTEX; i++)
	{
		gtMutex[i].bAvailable = TRUE;
		gtMutex[i].pHolding_Tcb = NULL;
		gtMutex[i].bLock = FALSE;
		gtMutex[i].pSleep_queue = NULL;

	}
}

int mutex_create(void)
{
	//disable_interrupt;
	disable_interrupts();
	int position = find_available_mutex();
	if(position != -1)
	{
		gtMutex[position].bAvailable = FALSE;
		enable_interrupts();
		return position;
	}
	else//do nothing
	{
		enable_interrupts();
		return -ENOMEM;
	}

	//enable_interrupts; // fix this to return the correct value
}

int mutex_lock(int mutex  __attribute__((unused)))
{
	disable_interrupts();
	tcb_t * current = get_cur_tcb();

	if(mutex < 0 || mutex > OS_NUM_MUTEX)
	{
		enable_interrupts();
		return -EINVAL;
	}

	else if (gtMutex[mutex].bAvailable == TRUE)
	{
		enable_interrupts();
		return -EINVAL;
	}

	else if (gtMutex[mutex].bLock == TRUE || 
		(gtMutex[mutex].pHolding_Tcb != NULL && gtMutex[mutex].pHolding_Tcb != current))
	{
		current -> sleep_queue = NULL;
		if(gtMutex[mutex].pSleep_queue == NULL)
		{
			gtMutex[mutex].pSleep_queue = current;
		}
		else {
			tcb_t * next = gtMutex[mutex].pSleep_queue;
			
			while(next -> sleep_queue != NULL)
			{
				next = next -> sleep_queue;
			}
			next -> sleep_queue = current;
		}
		dispatch_sleep();
	}

	gtMutex[mutex].bLock = TRUE;
	gtMutex[mutex].pHolding_Tcb = current;
	current.holds_lock ++ ;
	enable_interrupts();
	return 0;
}

int mutex_unlock(int mutex  __attribute__((unused)))
{
	disable_interrupts();
	tcb_t * current = get_cur_tcb();

	if(mutex < 0 || mutex > OS_NUM_MUTEX)
	{
		enable_interrupts();
		return -EINVAL;
	}

	else if (gtMutex[mutex].bAvailable == TRUE)
	{
		enable_interrupts();
		return -EINVAL;
	}

	else if (gtMutex[mutex].pHolding_Tcb != current)
	{
		enable_interrupts();
		return -EPERM;
	}
	else
	{
		gtMutex[mutex].pHolding_Tcb -> holds_lock --;
		gtMutex[mutex].bLock = FALSE;
		if(gtMutex[mutex].pSleep_queue != NULL)
		{
			gtMutex[mutex].pHolding_Tcb = gtMutex[mutex].pSleep_queue;
			runqueue_add(gtMutex[mutex].pSleep_queue, gtMutex[mutex].pSleep_queue -> cur_prio);
			gtMutex[mutex].pSleep_queue = gtMutex[mutex].pSleep_queue -> sleep_queue;
		}
		else
		{
    		gtMutex[mutex].pHolding_Tcb = NULL;
		}
		enable_interrupts();
		return 0;
	}
}

int find_available_mutex()
{
	int i = 0;
	for(; i < OS_NUM_MUTEX; i++)
	{
		if(gtMutex[i].bAvailable == TRUE)
			return i;
	}
	return -1;
}

