// * SPDX-License-Identifier:	GPL-2.0+

#include <common.h>
#include <sbi/fw_dynamic.h>

void boot0_jmp(phys_addr_t addr)
{
    asm volatile("jr a0");
}

void boot0_jmp_optee(phys_addr_t optee, phys_addr_t uboot)
{
	phys_addr_t dtb_entry = uboot + 2 * 1024 * 1024;

	asm volatile ("mv s1, %0" : : "r" (optee) : "memory");
	asm volatile ("mv a0, %0" : : "r" (dtb_entry) : "memory");
	asm volatile ("mv ra, %0" : : "r" (uboot) : "memory");
	asm volatile ("jr     s1");
}

void boot0_jmp_monitor(phys_addr_t monitor_base)
{
#ifdef CONFIG_MONITOR
	asm volatile("jr a0");
__LOOP:
	asm volatile("WFI");
	goto __LOOP;
#endif
}

static struct fw_dynamic_info fw_info;

void boot0_jmp_opensbi(phys_addr_t opensbi_base, phys_addr_t dtb, phys_addr_t uboot_base)
{
	void (*fn)(ulong, phys_addr_t, phys_addr_t) = (void *)opensbi_base;

	fw_info.magic		= FW_DYNAMIC_INFO_MAGIC_VALUE;
	fw_info.version		= FW_DYNAMIC_INFO_VERSION_2;
	fw_info.next_addr	= uboot_base;
	fw_info.next_mode	= FW_DYNAMIC_INFO_NEXT_MODE_S;
	fw_info.options		= 0;

	fn(0, dtb, (phys_addr_t)&fw_info);
}
