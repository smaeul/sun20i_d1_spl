	.equ	zero, 0
	.equ	ra, 1
	.equ	sp, 2
	.equ	gp, 3
	.equ	tp, 4
	.equ	t0, 5
	.equ	t1, 6
	.equ	t2, 7
	.equ	s0, 8
	.equ	s1, 9
	.equ	a0, 10
	.equ	a1, 11
	.equ	a2, 12
	.equ	a3, 13
	.equ	a4, 14
	.equ	a5, 15
	.equ	a6, 16
	.equ	a7, 17
	.equ	s2, 18
	.equ	s3, 19
	.equ	s4, 20
	.equ	s5, 21
	.equ	s6, 22
	.equ	s7, 23
	.equ	s8, 24
	.equ	s9, 25
	.equ	s10, 26
	.equ	s11, 27
	.equ	t3, 28
	.equ	t4, 29
	.equ	t5, 30
	.equ	t6, 31

	.macro	extu rd, rs1, imm1, imm2
	.word	0x0000300b | ((\imm1) << 26) | ((\imm2) << 20) | (\rs1 << 15) | (\rd << 7)
	.endm

	.macro	ldd rd1, rd2, rs1, imm2, junk
	.word	0xf800400b | ((\imm2) << 25) | (\rd2 << 20) | (\rs1 << 15) | (\rd1 << 7)
	.endm

	.macro	lrbu rd, rs1, rs2, imm2
	.word	0x8000400b | ((\imm2) << 25) | (\rs2 << 20) | (\rs1 << 15) | (\rd << 7)
	.endm

	.macro	lrw rd, rs1, rs2, imm2
	.word	0x4000400b | ((\imm2) << 25) | (\rs2 << 20) | (\rs1 << 15) | (\rd << 7)
	.endm

	.macro	lurw rd, rs1, rs2, imm2
	.word	0x5000400b | ((\imm2) << 25) | (\rs2 << 20) | (\rs1 << 15) | (\rd << 7)
	.endm

	.macro	lwd rd1, rd2, rs1, imm2, junk
	.word	0xe000400b | ((\imm2) << 25) | (\rd2 << 20) | (\rs1 << 15) | (\rd1 << 7)
	.endm

	.macro	sdd rd1, rd2, rs1, imm2, junk
	.word	0xf800500b | ((\imm2) << 25) | (\rd2 << 20) | (\rs1 << 15) | (\rd1 << 7)
	.endm

	.macro	srb rd, rs1, rs2, imm2
	.word	0x0000500b | ((\imm2) << 25) | (\rs2 << 20) | (\rs1 << 15) | (\rd << 7)
	.endm

	.macro	srw rd, rs1, rs2, imm2
	.word	0x4000500b | ((\imm2) << 25) | (\rs2 << 20) | (\rs1 << 15) | (\rd << 7)
	.endm

	.macro	swia rs2, rs1, imm5, imm2
	.word	0x5800500b | ((\imm2) << 25) | ((\imm5) << 20) | (\rs1 << 15) | (\rs2 << 7)
	.endm
