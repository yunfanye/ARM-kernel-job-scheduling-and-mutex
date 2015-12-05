#include <exports.h>

#include <arm/addr.h>
#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/interrupt.h>
#include <arm/timer.h>
#include <bits/fileno.h>
#include <bits/errno.h>
#include <bits/swi.h>


/* Helper functinos */
#define GET(addr)					(*((size_t *)(addr)))
#define SET(addr, val)		(*((size_t *)(addr)) = (val))

/* store the corrupted handler addr in handler_addr,
 * the old instruction in saved1, saved2, separately */
void install_handler(size_t vector, size_t new_addr, 
	size_t * saved_handler_addr, size_t * saved1, size_t * saved2) {
	
	size_t handler_addr = * saved_handler_addr; 
	size_t vec_instr = GET(vector);
	int imm;
	/* make sure the instruction is LDR PC, [PC, #imm12] 
	 * 0xe59FF shows that #imm12 is a positive, while
	 * 0xe51FF shows that #imm12 is a negative */
	if((vec_instr & 0xe51FF000) != 0xe51FF000) {
		puts("The instruction in vector is unrecognized!\n");
		return;
	}
	/* retrieve #imm12 */
	imm = vec_instr & 0xFFF;
	/* deal with the case imm is negative, which only
	 * has remote possibility though, i.e. if the the neg bit is set,
	 * imm *= -1 */
	if((vec_instr & 0x00800000) == 0)
		imm = -imm;
	/* locate the addr of the handler by imm12; pc + #8 + #imm12 */	
	handler_addr = GET(vector + imm + 0x8);
	*saved_handler_addr = handler_addr;
	/* save original instruction */
	*saved1 = GET(handler_addr);
	*saved2 = GET(handler_addr + sizeof(size_t));
	/* wire in new instructions */
	SET(handler_addr, 0xe51ff004); /* LDR PC, [PC, #-4]*/
	SET(handler_addr + sizeof(size_t), new_addr);	
}

void recover_handler(size_t handler_addr, size_t saved1, size_t saved2) {
	/* recover */
	SET(handler_addr, saved1); 
	SET(handler_addr + sizeof(size_t), saved2);	
}
