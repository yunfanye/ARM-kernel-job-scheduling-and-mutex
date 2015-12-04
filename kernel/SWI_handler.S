@ SWI_handler.S: SWI handler wired in
@
@ Authors: Yunfan Ye <yunfany@cmu.edu>
@					 Fangda Chen <fangdac@andrew.cmu.edu>
@ 
@ Date:    Oct. 15th, 2015

#include "addr.h"

	.file	"SWI_handler.S"
	.text
	.global	SWI_handler
	.type	SWI_handler, %function
SWI_handler:
	@ Save registers except return value, r0
	STMFD	sp!, {r1-r12, lr}
	@ restore r8 to be able to jump to export functions
	LDR		r2, =global_data
	LDR		r8, [r2]
	@ enable IRQ AND FIQ
	MRS		r2, cpsr
	BIC		r2, #0x000000C0
	MSR		cpsr, r2

	@ make old first para the second para
	MOV 	r1, r0
	@ obtain other paras
	MOV 	r2, sp
	@ retrieve swi_num
	LDR 	r0, [lr, #-4]
	BIC 	r0, r0, #0xff000000

	@ Save spsr
	MRS   r2, spsr
	STMFD sp!, {r2}
	@ Store user mode stack and link register
	STMFD sp!, {sp, lr}^

	@ call C_SWI_hanlder(swi_num, old_r0, paras)
	BL 		C_SWI_handler
	
	@ Restore stack and link register
    LDMFD sp!, {sp, lr}^
    @ Restore spsr
    LDMFD sp!, {r2}
    MSR   spsr, r2

	@ Restore registers, return value in r0
	LDMFD	sp!, {r1-r12, lr}
	@ return to user application
	MOVS	pc, lr
