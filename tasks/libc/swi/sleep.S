/** @file sleep.S
 *
 * @brief sleep sycall wrapper
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-10-31
 */

#include <asm.h>
#include <bits/swi.h>

	.file "sleep.S"

FUNC(sleep)
	@ Add code here.  Put your names at the top.
	STMFD	sp!, {r0-r12, lr}
	swi		SLEEP_SWI
	LDMFD	sp!, {r0-r12, lr}
	mov		pc, lr
