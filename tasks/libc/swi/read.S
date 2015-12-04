/** @file read.S
 *
 * @brief read sycall wrapper
 *
 * @author Mike Kasick <mkasick@andrew.cmu.edu>
 * @date   Sun, 07 Oct 2007 01:53:16 -0400
 * 
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-10-29
 */

#include <asm.h>
#include <bits/swi.h>

	.file "read.S"

FUNC(read)
	@ Add code here.  Put your names at the top.
	@ store user register
	STMFD	sp!, {r1-r12, lr}
	@ r0 fd, r1 buf, r2 len
	swi		READ_SWI
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
