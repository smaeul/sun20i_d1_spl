// SPDX-License-Identifier: GPL-2.0+

typedef struct __DRAM_PARA
{
    //normal configuration
    unsigned int  dram_clk;	// 0x00
    unsigned int  dram_type;	// 0x04
    unsigned int  dram_zq;	// 0x08
    unsigned int  dram_odt_en;	// 0x0c

    //control configuration
    unsigned int  dram_para1;	// 0x10
    unsigned int  dram_para2;	// 0x14

    //timing configuration
    unsigned int  dram_mr0;	// 0x18
    unsigned int  dram_mr1;	// 0x1c
    unsigned int  dram_mr2;	// 0x20
    unsigned int  dram_mr3;	// 0x24
    unsigned int  dram_tpr0;	// 0x28 DRAMTMG0
    unsigned int  dram_tpr1;	// 0x2c DRAMTMG1
    unsigned int  dram_tpr2;	// 0x30 DRAMTMG2
    unsigned int  dram_tpr3;	// 0x34 DRAMTMG3
    unsigned int  dram_tpr4;	// 0x38 DRAMTMG4
    unsigned int  dram_tpr5;	// 0x3c DRAMTMG5
    unsigned int  dram_tpr6;	// 0x40 DRAMTMG8

    //reserved for future use
    unsigned int  dram_tpr7;	// 0x44
    unsigned int  dram_tpr8;	// 0x48
    unsigned int  dram_tpr9;	// 0x4c
    unsigned int  dram_tpr10;	// 0x50
    unsigned int  dram_tpr11;	// 0x54
    unsigned int  dram_tpr12;	// 0x58
    unsigned int  dram_tpr13;	// 0x5c

} __dram_para_t;

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
