	.file	"measure.c"
	.text
	.comm	timer_state,4,4
	.comm	timer_buffer,8,8
	.globl	rdtsc
	.type	rdtsc, @function
rdtsc:
.LFB6:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
#APP
# 17 "measure.c" 1
	rdtsc
# 0 "" 2
#NO_APP
	movq	%rax, -16(%rbp)
	movq	%rdx, -8(%rbp)
	movq	-8(%rbp), %rax
	salq	$32, %rax
	orq	-16(%rbp), %rax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	rdtsc, .-rdtsc
	.globl	toggle_timer
	.type	toggle_timer, @function
toggle_timer:
.LFB7:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$8, %rsp
	movq	%rdi, -8(%rbp)
	cmpq	$0, -8(%rbp)
	je	.L8
	movl	timer_state(%rip), %eax
	testl	%eax, %eax
	je	.L6
	cmpl	$1, %eax
	je	.L7
	jmp	.L3
.L6:
	call	rdtsc
	movq	%rax, timer_buffer(%rip)
	movl	$1, timer_state(%rip)
	movq	-8(%rbp), %rax
	movq	$0, (%rax)
	jmp	.L3
.L7:
	call	rdtsc
	movq	%rax, %rdx
	movq	timer_buffer(%rip), %rax
	subq	%rax, %rdx
	movq	-8(%rbp), %rax
	movq	%rdx, (%rax)
	movl	$0, timer_state(%rip)
	nop
	jmp	.L3
.L8:
	nop
.L3:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7:
	.size	toggle_timer, .-toggle_timer
	.section	.rodata
.LC0:
	.string	"%lu\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB8:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movl	%edi, -36(%rbp)
	movq	%rsi, -48(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movl	$100, -28(%rbp)
	cmpl	$1, -36(%rbp)
	jle	.L10
	movq	-48(%rbp), %rax
	addq	$8, %rax
	movq	(%rax), %rax
	movq	%rax, %rdi
	call	atoi@PLT
	movl	%eax, -28(%rbp)
.L10:
	movl	$0, timer_state(%rip)
	movq	stdin(%rip), %rax
	movq	%rax, -16(%rbp)
	movl	$0, -32(%rbp)
	jmp	.L11
.L12:
	leaq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	toggle_timer
	movl	$0, %edi
	call	time@PLT
	leaq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	toggle_timer
	movq	-24(%rbp), %rax
	movq	%rax, %rsi
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	addl	$1, -32(%rbp)
.L11:
	movl	-32(%rbp), %eax
	cmpl	-28(%rbp), %eax
	jl	.L12
	movl	$0, %eax
	movq	-8(%rbp), %rdx
	xorq	%fs:40, %rdx
	je	.L14
	call	__stack_chk_fail@PLT
.L14:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE8:
	.size	main, .-main
	.ident	"GCC: (GNU) 8.2.1 20181127"
	.section	.note.GNU-stack,"",@progbits
