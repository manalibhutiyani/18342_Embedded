@ S_Handler.S: swi_handler startup routine
@ Author : Hailun Zhu <hailunz@andrew.cmu.edu>
@ time	 : 10/15/2014

	.file "S_Handler.S"
	.text
	.align 2
	.extern C_SWI_Handler
	.global S_Handler

S_Handler:
	sub	sp, sp, #4              @ leave room on stack for SPSR
	stmfd	sp!, {r0-r12, lr} 	@ store user's gp registers
	mrs	r2, spsr                @ get SPSR into gp registers
	str	r2, [sp, #56]           @ store SPSR above gp registers
	mov 	r1, sp              	@ pointer to parameters on stack
	ldr	r0, [lr, #-4]           @ extract the SWI number
	bic	r0,r0,#0xff000000       @ get SWI # by bit-masking
	bl	C_SWI_Handler           @ goto handler
	ldr	r2, [sp, #56] 	
	msr	spsr, r2                @ restore SPSR from r2
	add	sp, sp, #4              @ remove space used to store SPSR
	ldmfd	sp!, {r1-r12, lr} 	@ unstack user's registers
	add	sp, sp, #4              @ remove space used to store SPSR
	movs	pc, lr              	@ return from handler
