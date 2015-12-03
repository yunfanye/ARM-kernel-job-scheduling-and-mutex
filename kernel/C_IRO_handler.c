#include <arm/interrupt.h>
#include <arm/timer.h>
#include <arm/reg.h>
#include <types.h>
#include <exports.h>
#include "timer.h"

void C_IRQ_handler() {
	uint32_t now_count;
	if(reg_read(OSTMR_OSSR_ADDR) & OSTMR_OSSR_M0) {		
		/* acknowledge the interrupt */
		reg_set(OSTMR_OSSR_ADDR, OSTMR_OSSR_M0);
		/* reset the count */
		now_count = reg_read(OSTMR_OSCR_ADDR);
		/* loop to substract, as count may be several times higher than the match
		 * in high response case */
		while(now_count >= MATCH_COUNT) {
			now_count -= (MATCH_COUNT - EXECUTION_OFFSET);
			_time += INTERVAL;
		}
		reg_write(OSTMR_OSCR_ADDR, now_count);
		dev_update(_time);
	}
}
