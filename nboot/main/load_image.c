/*
 * (C) Copyright 2018-2020
* SPDX-License-Identifier:	GPL-2.0+
 * Allwinner Technology Co., Ltd. <www.allwinnertech.com>
 * wangwei <wangwei@allwinnertech.com>
 *
 */

#include <common.h>
#include <spare_head.h>
#include <nand_boot0.h>
#include <private_toc.h>
#include <private_boot0.h>
#include <private_uboot.h>
#include <private_tee.h>
#include <private_atf.h>
#include <u-boot/zlib.h>
#include <lzma/LzmaTools.h>
#include <u-boot/lz4.h>

extern const boot0_file_head_t  BT0_head;

int toc1_flash_read(u32 start_sector, u32 blkcnt, void *buff)
{
	void __iomem *addr = sunxi_get_iobase(CONFIG_BOOTPKG_BASE + 512 * start_sector);
	memcpy(buff, (addr), 512 * blkcnt);

	return blkcnt;
}

int load_image(phys_addr_t *uboot_base, phys_addr_t *optee_base, \
		phys_addr_t *monitor_base, phys_addr_t *rtos_base, phys_addr_t *opensbi_base, phys_addr_t *dtb_base)
{
	int i;
	//int len;
	__maybe_unused void *dram_para_addr = (void *)BT0_head.prvt_head.dram_para;
	phys_addr_t image_base;
	void __iomem *bootpkg_base = sunxi_get_iobase(CONFIG_BOOTPKG_BASE);

	struct sbrom_toc1_head_info  *toc1_head = NULL;
	struct sbrom_toc1_item_info  *item_head = NULL;

	struct sbrom_toc1_item_info  *toc1_item = NULL;

	toc1_head = (struct sbrom_toc1_head_info *)bootpkg_base;
	item_head = (struct sbrom_toc1_item_info *)(bootpkg_base + sizeof(struct sbrom_toc1_head_info));

#ifdef BOOT_DEBUG
	printf("*******************TOC1 Head Message*************************\n");
	printf("Toc_name          = %s\n",   toc1_head->name);
	printf("Toc_magic         = 0x%x\n", toc1_head->magic);
	printf("Toc_add_sum       = 0x%x\n", toc1_head->add_sum);

	printf("Toc_serial_num    = 0x%x\n", toc1_head->serial_num);
	printf("Toc_status        = 0x%x\n", toc1_head->status);

	printf("Toc_items_nr      = 0x%x\n", toc1_head->items_nr);
	printf("Toc_valid_len     = 0x%x\n", toc1_head->valid_len);
	printf("TOC_MAIN_END      = 0x%x\n", toc1_head->end);
	printf("*************************************************************\n");
#endif
	//init
	toc1_item = item_head;
	for(i=0;i<toc1_head->items_nr;i++,toc1_item++)
	{
#ifdef BOOT_DEBUG
		printf("*******************TOC1 Item Message*************************\n");
		printf("Entry_name        = %s\n",   toc1_item->name);
		printf("Entry_data_offset = 0x%x\n", toc1_item->data_offset);
		printf("Entry_data_len    = 0x%x\n", toc1_item->data_len);

		printf("encrypt           = 0x%x\n", toc1_item->encrypt);
		printf("Entry_type        = 0x%x\n", toc1_item->type);
		printf("run_addr          = 0x%x\n", toc1_item->run_addr);
		printf("index             = 0x%x\n", toc1_item->index);
		printf("Entry_end         = 0x%x\n", toc1_item->end);
		printf("*************************************************************\n");
#else
		printf("Entry_name        = %s\n",   toc1_item->name);
#endif

		image_base = toc1_item->run_addr;
		if (strncmp(toc1_item->name, ITEM_UBOOT_NAME, sizeof(ITEM_UBOOT_NAME)) == 0) {
			*uboot_base = image_base;
		} else if (strncmp(toc1_item->name, ITEM_OPTEE_NAME, sizeof(ITEM_OPTEE_NAME)) == 0) {
			*optee_base = image_base;
		} else if (strncmp(toc1_item->name, ITEM_MONITOR_NAME, sizeof(ITEM_MONITOR_NAME)) == 0) {
			*monitor_base = image_base;
		} else if (strncmp(toc1_item->name, ITEM_OPENSBI_NAME, sizeof(ITEM_OPENSBI_NAME)) == 0) {
			*opensbi_base = image_base;
		} else if (strncmp(toc1_item->name, ITEM_RTOS_NAME, sizeof(ITEM_RTOS_NAME)) == 0) {
			*rtos_base = image_base;
		} else if (strncmp(toc1_item->name, ITEM_DTB_NAME, sizeof(ITEM_DTB_NAME)) == 0) {
			*dtb_base = image_base;
		}
		toc1_flash_read(toc1_item->data_offset/512, (toc1_item->data_len+511)/512, (void *)image_base);
	}

	return 0;
}
