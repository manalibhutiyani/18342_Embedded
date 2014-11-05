@ exit.S
@ Author : Hailun Zhu <hailunz@andrew.cmu.edu>
@ time	 : 10/17/2014

	.file "exit.S"
	.text
	.align 2
	.global exit
exit:
	@restore registers and go back to kernel and exit
	ldr r2, =savesp
	ldr sp, [r2]
	ldr r2, [sp, #56]
	msr cpsr, r2
	ldr r2, [sp, #52] 
	msr spsr, r2
	ldmfd sp!,{r1-r12,lr}
	add sp, sp, #8
	mov pc, lr
