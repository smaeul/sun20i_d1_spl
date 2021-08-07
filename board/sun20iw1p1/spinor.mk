#
#config file for sun50iw11
#

include $(TOPDIR)/board/$(PLATFORM)/common.mk

CFG_SUNXI_SPI =y
CFG_SUNXI_DMA =y
CFG_SPI_USE_DMA =y
CFG_SUNXI_SPINOR =y
CFG_SPINOR_UBOOT_OFFSET=128
