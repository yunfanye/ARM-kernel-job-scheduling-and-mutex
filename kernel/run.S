@ run.S: set up user stack and call user application
@
@ Authors: Yunfan Ye <yunfany@cmu.edu>
@ 
@ Date:    Oct. 15th, 2015

#include <bits/swi.h>
#include <arm/addr.h>

	.file	"run.S"
	.text
	.global	run
	.type	run, %function
run:
	@ save all registers except return status, r0
	STMFD	sp!, {r1-r12, lr}
	@ save sp on a custom defined location
	LDR		r2, =SP_SAVED_LOC
	STR		sp, [r2]
	@ Switch to user mode with IRQs & FIQs masked.
	MRS		r2, cpsr
	AND		r2, #0xFFFFFFF0
	ORR		r2, #0x000000D0
	MSR		cpsr, r2
	@ Setup a full descending user mode stack 
	@ (with the stack top at 0xa3000000).
	LDR 	sp, =0xa3000000
	@ Push NULL at the end of envp
	MOV		r2, #0
	@ Push U-Boot's argc & argv on the user stack.
	STMFD 	sp!, {r2}
	MOVS	r2, r0		@ r2 = argc
	ADD		r1, r1, r2, ROR #30 @ r1 += r2 * 4
.push_argv:	
	BEQ		.push_argc	@ jump when count = 0
	LDR		r3, [r1, #-4]!
	STMFD	sp!, {r3}
	SUBS	r2, r2, #1
	B		.push_argv
.push_argc:		
	STMFD	sp!, {r0}
	@ Jump to a loaded user program at address 0xa0000000.
	MOV		lr, pc
	LDR 	pc, =0xa0000000
