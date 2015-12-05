ARM_OBJS := reg.o psr.o run.o mtimer.o IRQ_handler.o SWI_handler.o hanlder_installer.o C_SWI_handler.o C_IRO_handler.o
ARM_OBJS := $(ARM_OBJS:%=$(KDIR)/arm/%)

KOBJS += $(ARM_OBJS)
