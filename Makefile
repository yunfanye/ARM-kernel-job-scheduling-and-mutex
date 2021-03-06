# Root 18-349 Makefile
# Borrowed from 15410 Makefile -- thanks nwf and co.
#

# Change this to blank if you are on the gumstix
#CCPREFIX = arm-softfloat-linux-gnu-
CCPREFIX = arm-linux-

MAKEFLAGS += -rR

############### RULE HEADER #####################

# The packages are all phony targets.  They produce files in the bin directory.
# Make sure there are no name clashes.  Add new ones here if you make your own
# tests (which I recommend you do).

PACKAGES = dagger sys_err simple_mutex cyclone mutex_chaser stress

.PHONY: all package clean clobber $(PACKAGES)
all: package kernel


############### ENVIRONMENT #####################

CC = $(CCPREFIX)gcc
LD = $(CCPREFIX)ld
AR = $(CCPREFIX)ar
OBJCOPY = $(CCPREFIX)objcopy
RM = rm -f

ROOT = $(PWD)
KDIR = $(ROOT)/kernel
UDIR = $(ROOT)/uboot
TDIR = $(ROOT)/tasks
TLIBCDIR = $(TDIR)/libc

LIBGCC = `$(CC) -print-libgcc-file-name`

############## CONFIGURATION ####################

# Entry point addresses
KLOAD_ADDR = 0xa3000000
TLOAD_ADDR = 0xa0000000

# These are extra warning that you may find helpful.  These may give spurious
# warnings but are good for debugging code.
CWARNINGS_NOISY = -Wformat=2 -Wstrict-aliasing=2 -Wshadow -Wcast-qual \
	-Wstrict-prototypes -Wold-style-definition
CWARNINGS_SAFE = -Wall -Wno-unused-parameter -Wextra -Wpointer-arith \
	-Wwrite-strings -Wsystem-headers

CWARNINGS =  $(CWARNINGS_SAFE)
CWARNINGS1 = $(CWARNINGS_SAFE) $(CWARNINGS_NOISY)

KCFLAGS = -Os -ffreestanding -ffixed-r8 -nostdinc $(CWARNINGS)
TCFLAGS = -Os -ffreestanding -nostdinc $(CWARNINGS)
ASFLAGS = -nostdinc -Wall -Wextra -Werror -DASSEMBLER
KLDFLAGS = -nostdlib -N --fatal-warnings --warn-common -Ttext $(KLOAD_ADDR)
TLDFLAGS = -nostdlib -N --fatal-warnings --warn-common -Ttext $(TLOAD_ADDR)

KINCLUDES = -I$(UDIR)/include -I$(KDIR)/include
TINCLUDES = -I$(TLIBCDIR)/include

########### SUBFOLDER INCLUDES ##################

include $(UDIR)/uboot.mk
include $(KDIR)/kernel.mk
include $(TDIR)/tasks.mk


########### PATTERNED VARIABLES #################

$(TDIR)/%: INCLUDES=$(TINCLUDES)
$(KDIR)/%: INCLUDES=$(KINCLUDES)
$(UDIR)/%: INCLUDES=$(KINCLUDES)

$(TDIR)/%: CFLAGS=$(TCFLAGS)
$(KDIR)/%: CFLAGS=$(KCFLAGS)
$(UDIR)/%: CFLAGS=$(KCFLAGS)

$(TDIR)/%: LDFLAGS=$(TLDFLAGS)
$(KDIR)/%: LDFLAGS=$(KLDFLAGS)
$(UDIR)/%: LDFLAGS=$(KLDFLAGS)


############### PACKAGE RULES ###################

package: $(PACKAGES)
kernel : $(KERNEL).bin

############### GENERIC RULES ###################

%.dep: %.S
	@echo DEP $(notdir $<)
	@$(CC) $(ASFLAGS) -DASSEMBLER $(INCLUDES) -M -MP -MF $@ -MT $(<:.S=.o) $<

%.dep: %.s
	@echo "You should use the .S file extension rather than .s"
	@echo ".s does not support precompiler directives (like #include)"
	@false

%.dep: %.c
	@echo DEP $(notdir $<)
	@$(CC) $(CFLAGS) $(INCLUDES) -M -MP -MF $@ -MT $(<:.c=.o) $<

%.o: %.S
	@echo CC $(notdir $<)
	@$(CC) $(ASFLAGS) $(INCLUDES) -c -o $@ $<

%.o: %.s
	@echo "You should use the .S file extension rather than .s"
	@echo ".s does not support precompiler directives (like #include)"
	@false

%.o: %.c
	@echo CC $(notdir $<)
	@$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

%.a:
	@echo AR $(notdir $@)
	@$(AR) rcs $@ $^

%.bin: %
	@echo OBJCOPY $(notdir $<) $(notdir $@)
	@$(OBJCOPY) -O binary $< $@


############### CLEANING RULES ##################

clean:
	@echo CLEAN objects
	@$(RM) $(ALL_OBJS)
	@echo CLEAN deps
	@$(RM) $(ALL_OBJS:%.o=%.dep)
	@echo CLEAN archives/misc
	@$(RM) $(ALL_CLEANS)

clobber: clean
	@echo CLEAN packages
	@$(RM) $(PACKAGE_TARGETS)
	@echo CLEAN package bins
	@$(RM) $(PACKAGE_TARGETS:%=%.bin)
	@echo CLEAN others
	@$(RM) $(ALL_CLOBBERS)

debug: all
		(rm -f kernel_debug.s; rm -f dagger_debug.s;						\
       	sudo cp kernel/kernel.bin /media/bootfs/kernel.bin; 				\
       	sudo cp tasks/bin/dagger.bin /media/bootfs/dagger.bin; 				\
       	sudo cp tasks/bin/simple_mutex.bin /media/bootfs/simple_mutex.bin; 	\
       	sudo cp tasks/bin/mutex_chaser.bin /media/bootfs/mutex_chaser.bin; 	\
       	sudo cp tasks/bin/sys_err.bin /media/bootfs/sys_err.bin; 			\
       	sudo cp tasks/bin/cyclone.bin /media/bootfs/cyclone.bin; 			\
       	sudo cp tasks/bin/stress.bin /media/bootfs/stress.bin; 				\
       	arm-linux-objdump -d kernel/kernel >> kernel_debug.s; 				\
       	arm-linux-objdump -d tasks/bin/dagger >> dagger_debug.s)


########### DEPENDENCY FILE INCLUSION ############

ifeq (0,$(words $(findstring clean,$(MAKECMDGOALS))))
ifeq (0,$(words $(findstring clobber,$(MAKECMDGOALS))))
ifneq (,$(ALL_OBJS))
-include $(ALL_OBJS:.o=.dep)
endif
endif
endif

