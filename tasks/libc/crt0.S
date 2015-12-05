/** @file crt0.S
 *
 * @brief Execution startup routine
 *
 * @author Mike Kasick <mkasick@andrew.cmu.edu>
 * @date   Sun, 07 Oct 2007 01:51:29 -0400
 */

#include <asm.h>

	.file "crt0.S"

FUNC(_start)
	@ needs code.  Put your names at the top.
	@ retrieve envp
	@ mov argc to r0
	ldr		r0, [sp]
	@ mov argv to r1
	add 	r1, sp, #4
	@ save pc
	mov		lr, pc	
	@ call main, return value in r0
	ldr		pc, =main
	@ never exit
