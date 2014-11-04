@ switch_mode.S: set the mode, stack, jump to user program
@ Author: Hailun Zhu <hailunz@andrew.cmu.edu>
@ Date: 10/14/2014

        .file	"switch_mode.S"
        .text
        .align 2
        .global	switch_mode

switch_mode:

        @ save supervisor mode
        stmfd sp!,{r1-r12,lr}
        ldr r2, =savesp @save old sp to savesp
        str sp, [r2]
	
        @ Switch to user mode with IRQs and FIQs masked
        mrs r2, cpsr
        bic r2, r2, #0x0f
        orr r2, r2, #0xd0
        msr cpsr, r2
        
        @ Setup a full descending user mode stack
        ldr sp, =0xa3000000
        
        @ push argc & argv
        mov r2, #0
        stmfd sp!, {r2}
        mov r2, r0 @ count the argv num
        subs r2, r2, #1
        blt .pushArgc
        add r4, r1, r2, lsl #2

.pushArgv:
        ldr r3, [r4], #-4
        stmfd sp!, {r3}
        subs r2, r2, #1
        bge .pushArgv

.pushArgc:
        stmfd sp!, {r0}

.jumptoUser:
        @ jump to user program
        ldr pc, =0xa0000000
        
	
