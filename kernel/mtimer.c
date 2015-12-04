/**
 * @file timer.c
 *
 * @brief util functions related to timer
 *
 * @author Yunfan Ye <yunfanye@cmu.edu>
 *			Fangda Chen <fangdac@andrew.cmu.edu>
 *
 * @date 2015-11-09
 */

#include <arm/interrupt.h>
#include <arm/timer.h>
#include <arm/reg.h>
#include <types.h>
#include <exports.h>
#include "timer_constants.h"


#define INTV_OSTMR_0     1 << INT_OSTMR_0    /* OS Timer Match Register 0 Interrupt */
#define INTV_OSTMR_1     1 << INT_OSTMR_1    /* OS Timer Match Register 1 Interrupt */
#define INTV_OSTMR_2     1 << INT_OSTMR_2    /* OS Timer Match Register 2 Interrupt */
#define INTV_OSTMR_3     1 << INT_OSTMR_3    /* OS Timer Match Register 3 Interrupt */

void setup_timer() {
	_time = 0;
	/* set match register */
	reg_write(OSTMR_OSMR_ADDR(0), MATCH_COUNT);
	
	/* If an ICMR bit is set, the corresponding interrupt is delivered.
	 * If an ICMR bit is not set, the corresponding interrupt is masked. 
	 */
	reg_set(INT_ICMR_ADDR, INTV_OSTMR_0); /* deliver MR0 */
	reg_clear(INT_ICMR_ADDR, INTV_OSTMR_1 | INTV_OSTMR_2 | INTV_OSTMR_3);
	/* the corresponding interrupt is routed as an IRQ if the ICLR bit is 
	 * low and routed as an FIQ if the ICLR bit is high. */
	reg_clear(INT_ICLR_ADDR, INTV_OSTMR_0); /* use IRQ */
	
	/* only use OSMR0, enable timer 0 */
	reg_write(OSTMR_OIER_ADDR, OSTMR_OIER_E0);
	/* init, start timing */
	reg_write(OSTMR_OSCR_ADDR, 0);
}

