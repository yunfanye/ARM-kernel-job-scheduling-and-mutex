ARM_OBJS := reg.o psr.o run.o mtimer.o int_asm.o SWI_handler.o hanlder_installer.o C_SWI_handler.o C_IRQ_handler.o
ARM_OBJS := $(ARM_OBJS:%=$(KDIR)/arm/%)

KOBJS += $(ARM_OBJS)
