/** @file time.S
 *
 * @brief time sycall wrapper
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-10-31
 */

#include <asm.h>
#include <bits/swi.h>

	.file "task_create.S"

FUNC(task_create)
	@ Add code here.  Put your name at the top.
	STMFD	sp!, {r1-r12, lr}
	@ r0 fd, r1 buf, r2 len
	swi		CREATE_SWI
	cmp 	r0, #0
	bge		.end
	@ if return val less than 0
	ldr		r3, =errno
	neg		r0, r0
	str		r0, [r3]
	mov		r0, #-1
.end:
	@ restore user register
	LDMFD	sp!, {r1-r12, lr}
	@ return to user mode
	mov		pc, lr
