/*
 * (C) Copyright 2018
* SPDX-License-Identifier:	GPL-2.0+
 * wangwei <wangwei@allwinnertech.com>
 */

#include <common.h>
#include <libfdt.h>
#include <private_boot0.h>
#include <private_uboot.h>
#include <private_toc.h>
#include <arch/clock.h>
#include <arch/uart.h>
#include <arch/dram.h>
#include <arch/rtc.h>
#include <arch/gpio.h>
#ifdef CFG_DDR_SOFT_TRAIN
#include <arch/efuse.h>
#endif

static int boot0_clear_env(void);

void main(void)
{
	int dram_size;
	int status;
	phys_addr_t  uboot_base = 0, optee_base = 0, monitor_base = 0, \
				rtos_base = 0, opensbi_base = 0, dtb_base = 0;

	sunxi_serial_init(BT0_head.prvt_head.uart_port, (void *)BT0_head.prvt_head.uart_ctrl, 6);
	printf("HELLO! BOOT0 is starting!\n");
	printf("BOOT0 commit : %s\n", BT0_head.hash);
	sunxi_set_printf_debug_mode(BT0_head.prvt_head.debug_mode);

	status = sunxi_board_init();
	if(status)
		goto _BOOT_ERROR;

	if (rtc_probe_fel_flag()) {
		rtc_clear_fel_flag();
		goto _BOOT_ERROR;
#ifdef CFG_SUNXI_PHY_KEY
	} else if (check_update_key(&key_input)) {
		goto _BOOT_ERROR;
#endif
	} else if (BT0_head.prvt_head.enable_jtag) {
		printf("enable_jtag\n");
		boot_set_gpio((normal_gpio_cfg *)BT0_head.prvt_head.jtag_gpio, 5, 1);
	}

#ifdef FPGA_PLATFORM
	dram_size = mctl_init((void *)BT0_head.prvt_head.dram_para);
#else
#ifdef CFG_DDR_SOFT_TRAIN
	if (BT0_head.prvt_head.dram_para[30] & (1 << 11))
		neon_enable();
#endif
	dram_size = init_DRAM(0, (void *)BT0_head.prvt_head.dram_para);
#endif
	if(!dram_size)
		goto _BOOT_ERROR;
	else {
		printf("dram size =%d\n", dram_size);
	}

	char uart_input_value = get_uart_input();

	if (uart_input_value == '2') {
		sunxi_set_printf_debug_mode(3);
		printf("detected user input 2\n");
		goto _BOOT_ERROR;
	} else if (uart_input_value == 'd') {
		sunxi_set_printf_debug_mode(8);
		printf("detected user input d\n");
	}

	mmu_enable(dram_size);
	malloc_init(CONFIG_HEAP_BASE, CONFIG_HEAP_SIZE);
	status = sunxi_board_late_init();
	if (status)
		goto _BOOT_ERROR;

	status = load_package();
	if(status == 0 )
		load_image(&uboot_base, &optee_base, &monitor_base, &rtos_base, &opensbi_base, &dtb_base);
	else
		goto _BOOT_ERROR;

	if (dtb_base) {
		void *fdt = (void *)dtb_base;
		unsigned int i = 0;
		uint32_t reg[4];
		int offs;

		printf("Adding DRAM info to DTB.\n");
		status = fdt_open_into(fdt, fdt, SZ_1M);
		if (status)
			goto _BOOT_ERROR;

		offs = fdt_subnode_offset(fdt, 0, "memory");
		if (offs < 0)
			offs = fdt_add_subnode(fdt, 0, "memory");
		if (offs < 0)
			goto _BOOT_ERROR;
		if (fdt_address_cells(fdt, 0) > 1)
			reg[i++] = 0;
		reg[i++] = cpu_to_fdt32(SDRAM_OFFSET(0));
		if (fdt_size_cells(fdt, 0) > 1)
			reg[i++] = 0;
		reg[i++] = cpu_to_fdt32(dram_size * SZ_1M);
		if (fdt_setprop_string(fdt, offs, "device_type", "memory") < 0)
			goto _BOOT_ERROR;
		if (fdt_setprop(fdt, offs, "reg", reg, i * sizeof(*reg)) < 0)
			goto _BOOT_ERROR;
		if (fdt_pack(fdt) < 0)
			goto _BOOT_ERROR;
	}

	mmu_disable( );

	printf("Jump to second Boot.\n");
	if (opensbi_base) {
			boot0_jmp_opensbi(opensbi_base, dtb_base, uboot_base);
	} else if (monitor_base) {
		struct spare_monitor_head *monitor_head =
			(struct spare_monitor_head *)((phys_addr_t)monitor_base);
		monitor_head->secureos_base = optee_base;
		monitor_head->nboot_base = uboot_base;
		boot0_jmp_monitor(monitor_base);
	} else if (optee_base)
		boot0_jmp_optee(optee_base, uboot_base);
	else if (rtos_base) {
		printf("jump to rtos\n");
		boot0_jmp(rtos_base);
	}
	else
		boot0_jmp(uboot_base);

	while(1);
_BOOT_ERROR:
	boot0_clear_env();
	boot0_jmp(FEL_BASE);

}

static int boot0_clear_env(void)
{
	sunxi_board_exit();
	sunxi_board_clock_reset();
	mmu_disable();
	mdelay(10);

	return 0;
}
