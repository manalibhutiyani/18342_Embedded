@ IRQ_Handler.S: irq_handler startup routine
@ Author : Hailun Zhu <hailunz@andrew.cmu.edu>
@ time	 : 10/15/2014

	.file "IRQ_Handler.S"
	.text
	.align 2
	.extern C_IRQ_Handler
	.global IRQ_Handler

IRQ_Handler:
	sub	sp, sp, #4              @ leave room on stack for SPSR
	stmfd	sp!, {r0-r12, lr} 	@ store user's gp registers
	mrs	r2, spsr                @ get SPSR into gp registers
	str	r2, [sp, #56]           @ store SPSR above gp registers
	mov 	r0, sp              	@ pointer to parameters on stack
	mrs	r2, cpsr		@ mask nested IRQ in cpsr
	orr	r2, #0x80
	msr	cpsr, r2
	bl	C_IRQ_Handler           @ goto handler
	mrs	r2, cpsr		@ unmask IRQ bit in cpsr
	bic	r2, #0x80
	msr	cpsr, r2
	ldr	r2, [sp, #56] 	
	msr	spsr, r2                @ restore SPSR from r2
	add	sp, sp, #4              @ remove space used to store SPSR
	ldmfd	sp!, {r1-r12, lr} 	@ unstack user's registers
	add	sp, sp, #4              @ remove space used to store SPSR
	movs	pc, lr              	@ return from handler
