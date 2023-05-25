	.text
	.attribute	4, 16
	.attribute	5, "rv64i2p0"
	.file	"my module"
	.globl	main
	.p2align	2
	.type	main,@function
main:
	.cfi_startproc
	li	a0, 0
	ret
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc

	.section	".note.GNU-stack","",@progbits
