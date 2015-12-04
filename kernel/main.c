/**
 * @file main.c
 *
 * @brief main function of the Gravel2 kernel
 *
 * @author Yunfan Ye <yunfanye@cmu.edu>
 *				 Fangda Chen <fangdac@andrew.cmu.edu>
 *
 * @date 2015-12-03
 */
 
#include <kernel.h>
#include <task.h>
#include <sched.h>
#include <device.h>
#include <assert.h>
#include <arm/exception.h>

/* constants */
#define SWI_VECTOR				EX_SWI * 4
#define IRQ_VECTOR				EX_IRQ * 4

uint32_t global_data;

/* IRO_handler written in assembly, call C_IRO_handler */
extern size_t IRQ_handler();

/* SWI_handler written in assembly, call C_SWI_handler */
extern size_t SWI_handler();

/* run - save svc state before calling user program 
 * set up the user stack in assembly and 
 * call user application at 0xa2000000 
 * return exit_status */
extern int run(int argc, char *argv[]);

/* set initial value of the timer */
extern void setup_timer();

/* init mutex */
extern void mutex_init();

/* recover the change */
extern void install_handler(size_t vector, size_t new_addr, 
	size_t * handler_addr, size_t * saved1, size_t * saved2);

int kmain(int argc, char** argv, uint32_t table)
{
	int exit_status;
	size_t vector, new_addr;
	
	size_t swi_saved1, swi_saved2, swi_handler_addr;
	size_t irq_saved1, irq_saved2, irq_handler_addr;

	app_startup();
	global_data = table;
	/* add your code up to assert statement */
	
	/* hijack the original IRQ handler */
	new_addr = (size_t) &IRQ_handler;
	vector = IRQ_VECTOR;
	install_handler(vector, new_addr, &irq_handler_addr, &irq_saved1, &irq_saved2);
	/* hijack the original SWI handler */
	new_addr = (size_t) &SWI_handler;
	vector = SWI_VECTOR;
	install_handler(vector, new_addr, &swi_handler_addr, &swi_saved1, &swi_saved2);
	/* set up timer, begin timing */
	setup_timer();

	/* init mutex */
	mutex_init();

	/* save processor state, set up user stack, and call user program 
	 * after running the program, recover the uboot code */
	exit_status = run(argc, argv);
	
	assert(0);        /* should never get here */
	return exit_status;
}
