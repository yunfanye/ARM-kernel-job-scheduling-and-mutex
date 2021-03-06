@ SWI_handler.S: SWI handler wired in
@
@ Authors: Yunfan Ye <yunfany@cmu.edu>
@					 Fangda Chen <fangdac@andrew.cmu.edu>
@ 
@ Date:    Oct. 15th, 2015


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
	
	@ Save spsr, SVC
	MRS   r3, spsr
	STMFD sp!, {r3}
	@ Store user mode stack and link register, usr
	STMFD sp, {sp, lr}^
	SUB   sp, sp, #8

	@ enable IRQ AND FIQ
	MRS		r2, cpsr
	BIC		r2, #0x000000C0
	MSR		cpsr, r2

	@ make old first para the second para
	MOV 	r1, r0
	@ obtain other paras
	ADD 	r2, sp, #12
	@ retrieve swi_num
	LDR 	r0, [lr, #-4]
	BIC 	r0, r0, #0xff000000

	@ call C_SWI_hanlder(swi_num, old_r0, paras)
	BL 		C_SWI_handler
	
	@ Restore stack and link register, usr
    LDMFD sp, {sp, lr}^
    ADD   sp, sp, #8
    @ Restore spsr, SVC
    LDMFD sp!, {r3}
    MSR   spsr, r3

	@ Restore registers, return value in r0
	LDMFD	sp!, {r1-r12, lr}

	@ return to user application
	MOVS	pc, lr
