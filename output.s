	.text
	.attribute	4, 16
	.attribute	5, "rv64i2p0"
	.file	"my module"
	.globl	main
	.p2align	2
	.type	main,@function
main:
	.cfi_startproc
	addi	sp, sp, -416
	.cfi_def_cfa_offset 416
	sd	ra, 408(sp)
	.cfi_offset ra, -8
	lui	a0, %hi(__unnamed_1)
	addi	a0, a0, %lo(__unnamed_1)
	addi	a1, sp, 404
	call	scanf@plt
	lw	a1, 404(sp)
	lui	a0, %hi(__unnamed_2)
	addi	a0, a0, %lo(__unnamed_2)
	call	printf@plt
	sw	zero, 0(sp)
	li	a0, 0
	ld	ra, 408(sp)
	addi	sp, sp, 416
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
	.asciz	"\n%d"
	.size	__unnamed_2, 4

	.section	".note.GNU-stack","",@progbits
