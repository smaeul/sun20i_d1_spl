# SPDX-License-Identifier: GPL-2.0+

include $(TOPDIR)/mk/config.mk
LIB = libfdt.o

all: $(LIB)

include Makefile.libfdt
COBJS   += $(filter-out fdt_overlay.o,$(LIBFDT_OBJS))

SRCS	:= $(COBJS:.o=.c) $(SOBJS:.o=.S)
OBJS	:= $(addprefix $(obj),$(COBJS) $(SOBJS))

$(LIB):	$(OBJS)
	$(call cmd_link_o_target, $(OBJS))

#########################################################################

# defines $(obj).depend target
include $(TOPDIR)/mk/rules.mk

sinclude .depend

#########################################################################
