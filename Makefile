# SPDX-License-Identifier: GPL-2.0+

#
# Makefile for sunxi bootloader
# wangwei@allwinnertech.com
#

TOPDIR=$(CURDIR)
SRCTREE=$(TOPDIR)

#Q: quit for compile
Q = @
ifeq ("$(origin V)", "command line")
VERBOSE=$(V)
endif
ifndef VERBOSE
VERBOSE=0
endif

ifeq ($(VERBOSE),1)
Q=
CMD_ECHO_SILENT := true
else
CMD_ECHO_SILENT := echo
endif

#P: platform: sun50iw3p1/sun8iw15p1 etc.
PLATFORM=NULL
ifeq ("$(origin p)", "command line")
PLATFORM=$(p)
endif

#ddr: compile module: dd3/lpddr3/ddr4/lpddr4
DRAM_TYPE=NULL
ifeq ("$(origin ddr)", "command line")
DRAM_TYPE=$(ddr)
DRAM_TYPE_NAME=_$(DRAM_TYPE)
endif

ifeq ("$(origin b)", "command line")
BOARD=$(b)
endif

CPU ?= armv7

#include config file
include $(TOPDIR)/mk/config.mk
include $(TOPDIR)/mk/checkconf.mk
include $(TOPDIR)/board/$(p)/common.mk

export Q TOPDIR SRCTREE CMD_ECHO_SILENT PLATFORM DRAM_TYPE_NAME CPU

define update-commit-info
	sh $(SRCTREE)/tools/generate_hash_header_file.sh > \
		$(SRCTREE)/include/commit_info.h.tmp
	$(call mv-if-changed,$(SRCTREE)/include/commit_info.h.tmp,$(SRCTREE)/include/commit_info.h)
endef

MAKEFLAGS += --no-print-directory

all:
	$(MAKE) boot0
	$(MAKE) fes

boot0:
ifneq ($(wildcard $(TOPDIR)/board/$(PLATFORM)/mmc.mk),)
	$(MAKE) mmc
endif
ifneq ($(wildcard $(TOPDIR)/board/$(PLATFORM)/nand.mk),)
	$(MAKE) nand
endif
ifneq ($(wildcard $(TOPDIR)/board/$(PLATFORM)/spinor.mk),)
	$(MAKE) spinor
endif

ifneq ($(wildcard $(TOPDIR)/board/$(PLATFORM)/sboot.mk),)
all: sboot
endif

fes: mkdepend
	$(MAKE) -C $(SRCTREE)/fes $@
mmc nand spinor: mkdepend
	$(MAKE) -C $(SRCTREE)/nboot $@
offline_secure_mmc offline_secure_nand offline_secure_spinor: mkdepend
	$(MAKE) -C $(SRCTREE)/nboot $@
simulate: mkdepend
	$(MAKE) -C $(SRCTREE)/nboot $@
sboot: mkdepend
	$(MAKE) -C $(SRCTREE)/sboot $@

clean:
	@find $(TOPDIR) -type f \
		\( -name 'core' -o -name '*.bak' -o -name '*~' \
		-o -name '*.o'	-o -name '*.exe' -o -name '*.axf' \
		-o -name '*.elf' \
		-o -name '*.depend' \
		-o -name '*.bin' \
		-o -name '*.log' \
		-o -name '*.map' \) \
		-exec rm {} +
	@rm -f $(TOPDIR)/fes/fes1.lds
	@rm -f $(TOPDIR)/nboot/boot0.lds
	@rm -f $(TOPDIR)/sboot/sboot.lds

distclean: clean

mkdepend:
	$(call update-commit-info)

PHONY += FORCE
FORCE:
.PHONY: $(PHONY)
