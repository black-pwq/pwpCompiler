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

	.globl	main
	.p2align	2
	.type	main,@function
main:
	.cfi_startproc
	lui	a0, 10
	addiw	a0, a0, -944
	sub	sp, sp, a0
	.cfi_def_cfa_offset 40016
	li	a0, 1
	sw	a0, 20(sp)
	li	a0, 0
	lui	a1, 10
	addiw	a1, a1, -944
	add	sp, sp, a1
	ret
.Lfunc_end2:
	.size	main, .Lfunc_end2-main
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
