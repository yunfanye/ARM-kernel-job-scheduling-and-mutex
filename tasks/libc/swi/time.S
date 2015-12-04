/** @file time.S
 *
 * @brief time sycall wrapper
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-10-31
 */

#include <asm.h>

	.file "time.S"

FUNC(time)
	@ Add code here.  Put your names at the top.
	STMFD	sp!, {r1-r12, lr}
	swi		TIME_SWI
	LDMFD	sp!, {r1-r12, lr}
	mov		pc, lr
