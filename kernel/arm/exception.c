#include <kernel.h>
#include <task.h>
#include <sched.h>
#include <device.h>
#include <assert.h>
#include <arm/psr.h>
#include <arm/exception.h>

void enable_interrupts(void)
{
	uint32_t cpsr;
	asm volatile ("mrs %0, cpsr" : "=r" (cpsr));
	cpsr &= ~(PSR_IRQ | PSR_FIQ);
	asm volatile ("msr cpsr_c, %0" : : "r" (cpsr) : "memory", "cc");
}

void disable_interrupts(void)
{
	uint32_t cpsr;
	asm volatile ("mrs %0, cpsr" : "=r" (cpsr));
	cpsr |= PSR_IRQ | PSR_FIQ;
	asm volatile ("msr cpsr_c, %0" : : "r" (cpsr) : "memory", "cc");
}
