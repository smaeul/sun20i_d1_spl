// SPDX-License-Identifier: GPL-2.0+

/*
 * dram_para1 bits:
 *  16-19 = page size
 *  20-27 = row count
 *     28 = banks 4 or 8
 *
 * dram_para2 bits:
 *      0 = DQ width
 *      4 = CS1 control
 *   8-11 = rank flags? bit 8 = ranks differ in config
 *  12-13 = rank
 */

/* MC_WORK_MODE bits
 *   0- 1 = ranks code
 *   2- 3 = banks, log2 - 2		 2	 3	 2
 *   4- 7 = row width, log2 - 1		 16	11	11
 *   8-11 = page size, log2 - 3		 9	 9	13
 *  12-15 = DQ width (or 12-14?)
 *  16-18 = dram type (2=DDR2, 3=DDR3, 6=LPDDR2, 7=LPDDR3)
 *     19 = 2T or 1T
 *  23-24 = ranks code (again?)
 */

#define DRAM_MR0	((void*)0x3103030)
#define DRAM_MR1	((void*)0x3103034)
#define DRAM_MR2	((void*)0x3103038)
#define DRAM_MR3	((void*)0x310303c)

#define DRAMTMG0	((void*)0x3103058)
#define DRAMTMG1	((void*)0x310305c)
#define DRAMTMG2	((void*)0x3103060)
#define DRAMTMG3	((void*)0x3103064)
#define DRAMTMG4	((void*)0x3103068)
#define DRAMTMG5	((void*)0x310306c)
#define DRAMTMG6	((void*)0x3103070)
#define DRAMTMG7	((void*)0x3103074)
#define DRAMTMG8	((void*)0x3103078)

#define PITMG0		((void*)0x3103080)
#define PTR3           	((void*)0x3103050)
#define PTR4           	((void*)0x3103054)
#define RFSHTMG        	((void*)0x3103090)
#define RFSHCTL1       	((void*)0x3103094)
