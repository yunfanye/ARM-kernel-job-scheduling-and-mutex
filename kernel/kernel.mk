KERNEL = $(KDIR)/kernel
KSTART = $(KDIR)/start.o

# All core kernel objects go here.  Add objects here if you need to.
KOBJS := arm/reg.o arm/psr.o assert.o run.o mtimer.o IRQ_handler.o SWI_handler.o hanlder_installer.o C_SWI_handler.o C_IRO_handler.o main.o math.o memcheck.o raise.o ctype.o hexdump.o device.o
KOBJS := $(KOBJS:%=$(KDIR)/%)

-include $(KDIR)/arm/kernel.mk
-include $(KDIR)/syscall/kernel.mk
-include $(KDIR)/sched/kernel.mk
-include $(KDIR)/lock/kernel.mk

ALL_OBJS += $(KOBJS) $(KSTART)
ALL_CLOBBERS += $(KERNEL) $(KERNEL).bin

# Put everything needed by the kernel into the final binary.
# KOBJS contains core kernel objects.
# AOBJS contains objects that are ARM dependent.
# UOBJS contains objects that are U-boot dependent.

$(KERNEL): $(KSTART) $(KOBJS) $(UOBJS)
	@echo LD $(notdir $@)
	@$(LD) -static $(LDFLAGS) -o $@ $^ $(LIBGCC)

