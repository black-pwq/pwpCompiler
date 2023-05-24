	.text
	.attribute	4, 16
	.attribute	5, "rv64i2p0"
	.file	"my module"
	.globl	main
	.p2align	2
	.type	main,@function
main:
	.cfi_startproc
	addi	sp, sp, -448
	.cfi_def_cfa_offset 448
	sd	ra, 440(sp)
	sd	s0, 432(sp)
	sd	s1, 424(sp)
	sd	s2, 416(sp)
	sd	s3, 408(sp)
	.cfi_offset ra, -8
	.cfi_offset s0, -16
	.cfi_offset s1, -24
	.cfi_offset s2, -32
	.cfi_offset s3, -40
	lui	a0, %hi(__unnamed_1)
	addi	a0, a0, %lo(__unnamed_1)
	addi	a1, sp, 404
	call	scanf@plt
	sw	zero, 0(sp)
	addi	s3, sp, 4
	lui	a0, %hi(__unnamed_2)
	addi	s0, a0, %lo(__unnamed_2)
	lui	a0, %hi(__unnamed_3)
	addi	s1, a0, %lo(__unnamed_3)
.LBB0_1:
	lw	a0, 0(sp)
	lw	a1, 404(sp)
	bge	a0, a1, .LBB0_3
	lw	a0, 0(sp)
	slli	a0, a0, 2
	add	s2, s3, a0
	mv	a0, s0
	mv	a1, s2
	call	scanf@plt
	lw	a1, 0(s2)
	mv	a0, s1
	call	printf@plt
	lw	a0, 0(sp)
	addiw	a0, a0, 1
	sw	a0, 0(sp)
	j	.LBB0_1
.LBB0_3:
	li	a0, 0
	ld	ra, 440(sp)
	ld	s0, 432(sp)
	ld	s1, 424(sp)
	ld	s2, 416(sp)
	ld	s3, 408(sp)
	addi	sp, sp, 448
	ret
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc

	.type	__unnamed_1,@object
	.section	.rodata.str1.1,"aMS",@progbits,1
__unnamed_1:
	.asciz	"%d"
	.size	__unnamed_1, 3

	.type	__unnamed_2,@object
__unnamed_2:
	.asciz	"%d"
	.size	__unnamed_2, 3

	.type	__unnamed_3,@object
__unnamed_3:
	.asciz	"%d\n"
	.size	__unnamed_3, 4

	.section	".note.GNU-stack","",@progbits
