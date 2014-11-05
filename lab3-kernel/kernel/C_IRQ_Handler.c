/*
 * SWI handler
 * Author: Hailun Zhu <hailunz@andrew.cmu.edu>
 * Date 10/13/2014
 */
#include <exports.h>
#include <bits/swi.h>
#include <arm/reg.h>
#include <arm/timer.h>

extern volatile uint32_t system_time;

int C_IRQ_Handler()
{
    // get the content of OSCR and OSSR
    volatile uint32_t oscr_val = reg_read(OSTMR_OSCR_ADDR);
    volatile uint32_t ossr_val = reg_read(OSTMR_OSSR_ADDR);

    uint32_t next_match_time;

    // If interrupt on OSMR0 happened
    if (ossr_val == 1) {

        // clear OSSR bit
        reg_set(OSTMR_OSSR_ADDR, OSTMR_OSSR_M0);       

        // increment time interval by 10ms
        global_time += TIME_INTERVAL;

        // increment match register to next match value
        next_match_time += oscr_val + OSTMR_FREQ / OSTMR_DIVISOR;

        // store value to OSMR0
        reg_write(OSTMR_OSMR_ADDR(0), next_match_time);

    }

    int value=0;
  //  value=write(regs[0],(void *)regs[1],regs[2]);
    return value;
}

