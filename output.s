	.text
	.attribute	4, 16
	.attribute	5, "rv64i2p0"
	.file	"my module"
	.globl	main
	.p2align	2
	.type	main,@function
main:
	.cfi_startproc
	addi	sp, sp, -16
	.cfi_def_cfa_offset 16
	sd	ra, 8(sp)
	.cfi_offset ra, -8
	lui	a0, %hi(__unnamed_1)
	addi	a0, a0, %lo(__unnamed_1)
	addi	a1, sp, 4
	call	scanf@plt
	lw	a2, 4(sp)
	lui	a0, %hi(__unnamed_2)
	addi	a0, a0, %lo(__unnamed_2)
	lui	a1, 8209
	slli	a1, a1, 37
	call	printf@plt
	li	a0, 0
	ld	ra, 8(sp)
	addi	sp, sp, 16
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
	.asciz	"\n%f\n%d"
	.size	__unnamed_2, 7

	.section	".note.GNU-stack","",@progbits
