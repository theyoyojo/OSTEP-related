	.file	"direct_write.c"
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movabsq	$8583909746840200520, %rax
	movq	%rax, -23(%rbp)
	movl	$1684828783, -15(%rbp)
	movw	$2593, -11(%rbp)
	movb	$0, -9(%rbp)
	movl	$0, -44(%rbp)
	movl	$4, -40(%rbp)
	movl	$0, -36(%rbp)
	leaq	-23(%rbp), %rax
	movq	%rax, -32(%rbp)
	jmp	.L2
.L3:
	addl	$1, -44(%rbp)
.L2:
	movq	-32(%rbp), %rax
	leaq	1(%rax), %rdx
	movq	%rdx, -32(%rbp)
	movzbl	(%rax), %eax
	testb	%al, %al
	jne	.L3
	movl	-36(%rbp), %ecx
	leaq	-23(%rbp), %r8
	movl	-44(%rbp), %r9d
	movl	-40(%rbp), %r10d
#APP
# 12 "direct_write.c" 1
	movl %ecx, %edi
	movq %r8, %rsi
	movl %r9d, %edx
	movl %r10d, %eax
	syscall
# 0 "" 2
#NO_APP
	movl	$0, %eax
	movq	-8(%rbp), %rsi
	xorq	%fs:40, %rsi
	je	.L5
	call	__stack_chk_fail@PLT
.L5:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (GNU) 8.2.1 20181127"
	.section	.note.GNU-stack,"",@progbits
