	.text
	.attribute	4, 16
	.attribute	5, "rv64i2p0"
	.file	"my module"
	.globl	getint
	.p2align	2
	.type	getint,@function
getint:
	.cfi_startproc
	addi	sp, sp, -16
	.cfi_def_cfa_offset 16
	sd	ra, 8(sp)
	.cfi_offset ra, -8
	lui	a0, %hi(__unnamed_1)
	addi	a0, a0, %lo(__unnamed_1)
	addi	a1, sp, 4
	call	scanf@plt
	lw	a0, 4(sp)
	ld	ra, 8(sp)
	addi	sp, sp, 16
	ret
.Lfunc_end0:
	.size	getint, .Lfunc_end0-getint
	.cfi_endproc

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
	lui	a0, %hi(__unnamed_2)
	addi	a0, a0, %lo(__unnamed_2)
	call	printf@plt
	ld	ra, 8(sp)
	addi	sp, sp, 16
	ret
.Lfunc_end1:
	.size	putint, .Lfunc_end1-putint
	.cfi_endproc

	.globl	QuickSort
	.p2align	2
	.type	QuickSort,@function
QuickSort:
	.cfi_startproc
	addi	sp, sp, -16
	.cfi_def_cfa_offset 16
	sd	ra, 8(sp)
	sd	s0, 0(sp)
	.cfi_offset ra, -8
	.cfi_offset s0, -16
	addi	s0, sp, 16
	.cfi_def_cfa s0, 0
	sext.w	a3, a2
	sext.w	a4, a1
	bge	a4, a3, .LBB2_4
	mv	a5, sp
	addi	a3, a5, -16
	mv	sp, a3
	sw	a1, -16(a5)
	mv	a5, sp
	addi	a1, a5, -16
	mv	sp, a1
	sw	a2, -16(a5)
	mv	a5, sp
	addi	a2, a5, -16
	mv	sp, a2
	slli	a4, a4, 2
	add	a4, a0, a4
	lw	a4, 0(a4)
	sw	a4, -16(a5)
.LBB2_2:
	lw	a4, 0(a3)
	lw	a5, 0(a1)
	blt	a4, a5, .LBB2_2
	lw	a1, 0(a2)
	sw	a1, 4(a0)
.LBB2_4:
	addi	sp, s0, -16
	ld	ra, 8(sp)
	ld	s0, 0(sp)
	addi	sp, sp, 16
	ret
.Lfunc_end2:
	.size	QuickSort, .Lfunc_end2-QuickSort
	.cfi_endproc

	.globl	main
	.p2align	2
	.type	main,@function
main:
	.cfi_startproc
	li	a0, 0
	ret
.Lfunc_end3:
	.size	main, .Lfunc_end3-main
	.cfi_endproc

	.type	__unnamed_1,@object
	.section	.rodata.str1.1,"aMS",@progbits,1
__unnamed_1:
	.asciz	"%d"
	.size	__unnamed_1, 3

	.type	__unnamed_2,@object
__unnamed_2:
	.asciz	"%d\n"
	.size	__unnamed_2, 4

	.section	".note.GNU-stack","",@progbits
