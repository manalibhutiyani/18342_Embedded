/*
 * SWI handler
 * Author: Hailun Zhu <hailunz@andrew.cmu.edu>
 * Date 10/13/2014
 */
#include <exports.h>
#include <bits/swi.h>

extern void exit(int status);
extern ssize_t read(int fd, void *buf, size_t count);
extern ssize_t write(int fd, const void *buf, size_t count);

int C_SWI_Handler(unsigned swi_num, unsigned *regs)
{

    int value=0;
    switch (swi_num) {
            case EXIT_SWI:
                exit(regs[0]);
                break;
            case READ_SWI:
               value= read(regs[0],(void *)regs[1],regs[2]);
                break;
            case WRITE_SWI:
                value=write(regs[0],(void *)regs[1],regs[2]);
                break;
            default:
                puts("Wrong SWI Number!");
                exit(0x0badc0de);
	} /* end switch */
    return value;
}

