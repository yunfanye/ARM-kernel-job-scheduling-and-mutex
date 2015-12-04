@ exit.S: exit sycall wrapper
@
@ Authors: Yunfan Ye <yunfany@cmu.edu>
@ 
@ Date:    Oct. 29th, 2015

#include <arm/psr.h>

	.file	"IRQ_handler.S"
	.text
	.global	IRQ_handler
	.type	IRQ_handler, %function
IRQ_handler:
	
	@ save spsr and lr, IRQ
	MRS   r2, spsr
	STMFD sp!, {r2, lr}

	@ Switch to SVC with IRQ and FIQ enabled
	msr      cpsr_c, #(PSR_MODE_SVC)

	@ Save all registers, SVC	
	STMFD	sp!, {r0-r12, lr}

	@ restore r8 to be able to jump to export functions
	LDR		r2, =global_data
	LDR		r8, [r2]

	@ Save spsr, SVC
	MRS   r2, spsr
	STMFD sp!, {r2}
	@ Store user mode stack and link register, usr
	STMFD sp, {sp, lr}^
	SUB   sp, sp, #8

	@ call void C_IRQ_hanlder()
	BL 		C_IRQ_handler

	@ Restore stack and link register, usr
    LDMFD sp, {sp, lr}^
    ADD   sp, sp, #8
    @ Restore spsr, SVC
    LDMFD sp!, {r2}
    MSR   spsr, r2


	@ Restore all registers, SVC
	LDMFD	sp!, {r0-r12, lr}

	@ Return to IRQ mode
	msr      cpsr_c, #(PSR_MODE_IRQ)

	@ restore spsr and lr, IRQ
	
	LDMFD sp!, {r2, lr}
	MSR   spsr, r2

	SUBS	pc, lr, #4
