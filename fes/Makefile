#
# (C) Copyright 2000-2011
# SPDX-License-Identifier: GPL-2.0+
# Wolfgang Denk, DENX Software Engineering, wd@denx.de.
#
# (C) Copyright 2011
# Daniel Schwierzeck, daniel.schwierzeck@googlemail.com.
#
# (C) Copyright 2011
# Texas Instruments Incorporated - http://www.ti.com/
# Aneesh V <aneesh@ti.com>
#
# See the file COPYING in the root directory of the source tree for details.
#
# Based on top-level Makefile.
#

SKIP_AUTO_CONF:=yes

include $(TOPDIR)/mk/config.mk
include $(TOPDIR)/board/$(PLATFORM)/$(MAKECMDGOALS).mk

CPU ?= armv7

CPUDIR := $(TOPDIR)/arch/$(ARCH)/cpu/$(CPU)
FES_LDSCRIPT := $(CPUDIR)/fes1.lds

# We want the final binaries in this directory
FES1_DIR := $(TOPDIR)/fes/
obj := $(TOPDIR)/fes/

LIBS-y += arch/$(ARCH)/cpu/$(CPU)/libarch.o
LIBS-y += fes/main/libmain.o
LIBS-y += drivers/libdrivers.o
LIBS-y += common/libcommon.o
LIBS-y += board/$(PLATFORM)/libboard.o

LIBS := $(addprefix $(TOPDIR)/,$(sort $(LIBS-y)))

COBJS-y += $(TOPDIR)/fes/main/fes1_main.o

OBJS = $(sort $(COBJS-y))

# Special flags for CPP when processing the linker script.
# Pass the version down so we can handle backwards compatibility
# on the fly.
LDPPFLAGS += \
	-DFES1ADDR=$(CFG_FES1_RUN_ADDR)	 \
	-DCPUDIR=$(CPUDIR) \
	$(shell $(LD) --version | \
	  sed -ne 's/GNU ld version \([0-9][0-9]*\)\.\([0-9][0-9]*\).*/-DLD_MAJOR=\1 -DLD_MINOR=\2/p')

fes: $(LIBS) fes1.lds
	$(LD) -r -o lib$(PLATFORM)_fes.o $(LIBS)
	$(LD) lib$(PLATFORM)_fes.o $(OBJS) $(PLATFORM_LIBGCC) $(LDFLAGS) -Tfes1.lds -o fes1.elf -Map fes1.map
	$(OBJCOPY) $(OBJCFLAGS) -O binary fes1.elf fes1.bin
	python3 $(TOPDIR)/mk/gen_check_sum fes1.bin fes1_$(PLATFORM)$(DRAM_TYPE_NAME).bin
	$(STRIP) -g $(TOPDIR)/fes/lib$(PLATFORM)_fes.o

$(LIBS): depend
	$(MAKE) -C $(dir $@)

fes1.lds: $(FES_LDSCRIPT) depend
	$(Q)$(CPP) $(ALL_CFLAGS) $(LDPPFLAGS) -ansi -D__ASSEMBLY__ -P - <$(FES_LDSCRIPT) >$@

sinclude $(TOPDIR)/mk/target_for_conf.mk
depend: .depend build-confs
#########################################################################

# defines $(NBOOT_DIR).depend target
include $(TOPDIR)/mk/rules.mk

sinclude .depend

#########################################################################
