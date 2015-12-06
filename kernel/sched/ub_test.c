/** @file ub_test.c
 * 
 * @brief The UB Test for basic schedulability
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-11-20
 */

//#define DEBUG 0

#include <sched.h>
#ifdef DEBUG
#include <exports.h>
#endif


static double ub_table[] = 
{
	1.0,
	1.0, 0.8284, 0.7798, 0.7568, 0.7435, 0.7348, 0.7286, 0.7241, 
	0.7205, 0.7177, 0.7155, 0.7136, 0.712, 0.7106, 0.7094, 0.7084, 
	0.7075, 0.7067, 0.7059, 0.7053, 0.7047, 0.7042, 0.7037, 0.7033, 
	0.7028, 0.7025, 0.7021, 0.7018, 0.7015, 0.7012, 0.701, 0.7007, 
	0.7005, 0.7003, 0.7001, 0.6999, 0.6997, 0.6995, 0.6993, 0.6992, 
	0.699, 0.6989, 0.6988, 0.6986, 0.6985, 0.6984, 0.6983, 0.6982, 
	0.6981, 0.698, 0.6979, 0.6978, 0.6977, 0.6976, 0.6975, 0.6975, 
	0.6974, 0.6973, 0.6972, 0.6972, 0.6971, 0.697, 0.697, 0.6969
};
 
/**
 * @brief Perform UB Test and reorder the task list.
 *
 * The task list at the end of this method will be sorted in order is priority
 * -- from highest priority (lowest priority number) to lowest priority
 * (highest priority number).
 *
 * @param tasks  An array of task pointers containing the task set to schedule.
 * @param num_tasks  The number of tasks in the array.
 *
 * @return 0  The test failed.
 * @return 1  Test succeeded.  The tasks are now in order.
 */
int assign_schedule(task_t** tasks, size_t num_tasks)
{
	size_t i;
	sched_context_t* context;
	task_t * task_list = *tasks;/* array */
	task_t * task; /* pointer */
	double util = 0.0;

	for(i = 0; i < num_tasks; i++) {
		task = &task_list[i];
		util += (double)task -> C / (double)task -> T;
	}
	if(util <= ub_table[num_tasks])
		return 1;
	else
		return 0;	
}
	


