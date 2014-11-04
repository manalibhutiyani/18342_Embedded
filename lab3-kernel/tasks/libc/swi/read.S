/** @file read.S
 *
 * @brief read sycall wrapper
 *
 * @author Mike Kasick <mkasick@andrew.cmu.edu>
 * @date   Sun, 07 Oct 2007 01:53:16 -0400
 * 
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-10-29
 */

#include <asm.h>
#include <bits/swi.h>
#include <bits/errno.h>
	.file "read.S"

FUNC(read)
	@ Add code here.  Put your names at the top.
	swi READ_SWI
	cmp r0, #0
	bge .L2
.L1:
	neg r1,r0
	ldr r0, =errno
	str r1, [r0]
	mov r0,#-1 

.L2:
	mov pc, lr
	
