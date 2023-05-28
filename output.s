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
	addi	sp, sp, -48
	.cfi_def_cfa_offset 48
	sd	ra, 40(sp)
	sd	s0, 32(sp)
	sd	s1, 24(sp)
	sd	s2, 16(sp)
	sd	s3, 8(sp)
	.cfi_offset ra, -8
	.cfi_offset s0, -16
	.cfi_offset s1, -24
	.cfi_offset s2, -32
	.cfi_offset s3, -40
	addi	s0, sp, 48
	.cfi_def_cfa s0, 0
	mv	s1, a2
	sext.w	a2, a2
	sext.w	a3, a1
	bge	a3, a2, .LBB2_12
	mv	s2, a0
	mv	a0, sp
	addi	s3, a0, -16
	mv	sp, s3
	sw	a1, -16(a0)
	mv	a2, sp
	addi	a0, a2, -16
	mv	sp, a0
	sw	s1, -16(a2)
	mv	a4, sp
	addi	a2, a4, -16
	mv	sp, a2
	slli	a3, a3, 2
	add	a3, s2, a3
	lw	a3, 0(a3)
	sw	a3, -16(a4)
.LBB2_2:
	lw	a3, 0(s3)
	lw	a4, 0(a0)
	bge	a3, a4, .LBB2_11
.LBB2_3:
	lw	a3, 0(a0)
	lw	a4, 0(s3)
	slli	a5, a3, 2
	add	a5, s2, a5
	lw	a5, 0(a5)
	lw	a6, 0(a2)
	slt	a3, a4, a3
	xori	a3, a3, 1
	slt	a4, a5, a6
	or	a3, a3, a4
	bnez	a3, .LBB2_5
	lw	a3, 0(a0)
	addiw	a3, a3, -1
	sw	a3, 0(a0)
	j	.LBB2_3
.LBB2_5:
	lw	a3, 0(s3)
	lw	a4, 0(a0)
	bge	a3, a4, .LBB2_8
	lw	a3, 0(a0)
	lw	a4, 0(s3)
	slli	a3, a3, 2
	add	a3, s2, a3
	lw	a3, 0(a3)
	slli	a4, a4, 2
	add	a4, s2, a4
	sw	a3, 0(a4)
.LBB2_7:
	lw	a3, 0(s3)
	addiw	a3, a3, 1
	sw	a3, 0(s3)
.LBB2_8:
	lw	a3, 0(s3)
	lw	a4, 0(a0)
	slli	a5, a3, 2
	add	a5, s2, a5
	lw	a5, 0(a5)
	lw	a6, 0(a2)
	slt	a3, a3, a4
	xori	a3, a3, 1
	slt	a4, a5, a6
	xori	a4, a4, 1
	or	a3, a3, a4
	beqz	a3, .LBB2_7
	lw	a3, 0(s3)
	lw	a4, 0(a0)
	bge	a3, a4, .LBB2_2
	lw	a3, 0(s3)
	lw	a4, 0(a0)
	slli	a3, a3, 2
	add	a3, s2, a3
	lw	a3, 0(a3)
	slli	a4, a4, 2
	add	a4, s2, a4
	sw	a3, 0(a4)
	lw	a3, 0(a0)
	addiw	a3, a3, -1
	sw	a3, 0(a0)
	j	.LBB2_2
.LBB2_11:
	lw	a0, 0(s3)
	lw	a2, 0(a2)
	slli	a0, a0, 2
	add	a0, s2, a0
	sw	a2, 0(a0)
	lw	a0, 0(s3)
	addiw	a2, a0, -1
	mv	a0, s2
	call	QuickSort@plt
	lw	a0, 0(s3)
	addiw	a1, a0, 1
	mv	a0, s2
	mv	a2, s1
	call	QuickSort@plt
.LBB2_12:
	addi	sp, s0, -48
	ld	ra, 40(sp)
	ld	s0, 32(sp)
	ld	s1, 24(sp)
	ld	s2, 16(sp)
	ld	s3, 8(sp)
	addi	sp, sp, 48
	ret
.Lfunc_end2:
	.size	QuickSort, .Lfunc_end2-QuickSort
	.cfi_endproc

	.globl	main
	.p2align	2
	.type	main,@function
main:
	.cfi_startproc
	addi	sp, sp, -2032
	.cfi_def_cfa_offset 2032
	sd	ra, 2024(sp)
	sd	s0, 2016(sp)
	sd	s1, 2008(sp)
	.cfi_offset ra, -8
	.cfi_offset s0, -16
	.cfi_offset s1, -24
	lui	a0, 9
	addiw	a0, a0, 1152
	sub	sp, sp, a0
	.cfi_def_cfa_offset 40048
	sw	zero, 20(sp)
	call	getint@plt
	sw	a0, 16(sp)
	addi	s0, sp, 24
.LBB3_1:
	lw	a0, 20(sp)
	lw	a1, 16(sp)
	bge	a0, a1, .LBB3_3
	lw	a0, 20(sp)
	slli	a0, a0, 2
	add	s1, s0, a0
	call	getint@plt
	lw	a1, 20(sp)
	sw	a0, 0(s1)
	addiw	a0, a1, 1
	sw	a0, 20(sp)
	j	.LBB3_1
.LBB3_3:
	lw	a0, 16(sp)
	addiw	a2, a0, -1
	addi	a0, sp, 24
	addi	s0, sp, 24
	li	a1, 0
	call	QuickSort@plt
	sw	zero, 20(sp)
.LBB3_4:
	lw	a0, 20(sp)
	lw	a1, 16(sp)
	bge	a0, a1, .LBB3_6
	lw	a0, 20(sp)
	slli	a0, a0, 2
	add	a0, s0, a0
	lw	a0, 0(a0)
	call	putint@plt
	lw	a0, 20(sp)
	addiw	a0, a0, 1
	sw	a0, 20(sp)
	j	.LBB3_4
.LBB3_6:
	li	a0, 0
	lui	a1, 9
	addiw	a1, a1, 1152
	add	sp, sp, a1
	ld	ra, 2024(sp)
	ld	s0, 2016(sp)
	ld	s1, 2008(sp)
	addi	sp, sp, 2032
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
