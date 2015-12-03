@ exit.S: exit sycall wrapper
@
@ Authors: Yunfan Ye <yunfany@cmu.edu>
@ 
@ Date:    Oct. 29th, 2015

#include <arm/addr.h>

	.file	"IRQ_handler.S"
	.text
	.global	IRQ_handler
	.type	IRQ_handler, %function
IRQ_handler:
	@ set IRQ stack to IRQ_STACK, full desc
	LDR		sp, =IRQ_SP_LOC
	@ Save all registers	
	STMFD	sp!, {r0-r12, lr}
	@ restore r8 to be able to jump to export functions
	LDR		r2, =global_data
	LDR		r8, [r2]
	@ call void C_IRQ_hanlder()
	BL 		C_IRQ_handler
	LDMFD	sp!, {r0-r12, lr}
	@ return to user application
	SUBS	pc, lr, #4
