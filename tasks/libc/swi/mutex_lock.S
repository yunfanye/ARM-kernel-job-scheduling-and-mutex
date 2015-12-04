/** @file time.S
 *
 * @brief time sycall wrapper
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-10-31
 */

#include <asm.h>
#include <bits/swi.h>

	.file "mutex_lock.S"

FUNC(mutex_lock)
	@ Add code here.  Put your name at the top.

    @ store user register
	STMFD	sp!, {r1-r12, lr}

	swi MUTEX_LOCK

	@compare the return value
	cmp r0, #0
	bge .end

	sub r0, #0, r0;
	ldr r3, =errno
	str r0, [r3]
	mov r0, #1
.end:
    ldmfd   sp!, {r1-r12, lr}
	mov pc, lr
