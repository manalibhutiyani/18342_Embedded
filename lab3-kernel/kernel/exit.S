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
	ldmfd sp!,{r1-r12,lr}
	mov pc, lr
