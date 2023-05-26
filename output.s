	.text
	.attribute	4, 16
	.attribute	5, "rv64i2p0"
	.file	"my module"
	.globl	putint
	.p2align	2
	.type	putint,@function
putint:
	.cfi_startproc
	addi	sp, sp, -16
	.cfi_def_cfa_offset 16
	sd	ra, 8(sp)
	.cfi_offset ra, -8
	mv	a1, a0
	lui	a0, %hi(__unnamed_1)
	addi	a0, a0, %lo(__unnamed_1)
	call	printf@plt
	ld	ra, 8(sp)
	addi	sp, sp, 16
	ret
.Lfunc_end0:
	.size	putint, .Lfunc_end0-putint
	.cfi_endproc

	.globl	main
	.p2align	2
	.type	main,@function
main:
	.cfi_startproc
	addi	sp, sp, -16
	.cfi_def_cfa_offset 16
	sd	ra, 8(sp)
	.cfi_offset ra, -8
	li	a0, 1
	sw	a0, 4(sp)
	li	a0, 111
	sw	a0, 0(sp)
.LBB1_1:
	lw	a0, 4(sp)
	lw	a1, 0(sp)
	slt	a1, a0, a1
	xori	a1, a1, 1
	addi	a0, a0, -10
	seqz	a0, a0
	or	a0, a1, a0
	bnez	a0, .LBB1_3
	lw	a0, 4(sp)
	call	putint@plt
	lw	a0, 4(sp)
	addiw	a0, a0, 1
	sw	a0, 4(sp)
	j	.LBB1_1
.LBB1_3:
	li	a0, 0
	ld	ra, 8(sp)
	addi	sp, sp, 16
	ret
.Lfunc_end1:
	.size	main, .Lfunc_end1-main
	.cfi_endproc

	.type	__unnamed_1,@object
	.section	.rodata.str1.1,"aMS",@progbits,1
__unnamed_1:
	.asciz	"%d\n"
	.size	__unnamed_1, 4

	.section	".note.GNU-stack","",@progbits
