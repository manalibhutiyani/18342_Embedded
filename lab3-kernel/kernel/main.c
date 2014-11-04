#include <exports.h>

#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/interrupt.h>
#include <arm/timer.h>

#include <bits/fileno.h>
#include <bits/errno.h>

uint32_t global_data;

#define INS_ERROR 0x0badc0de
#define SYSCALLS 0x08
#define IRQ_NUM 0x18

extern void S_Handler();
extern int switch_mode(int argc, char *argv[]);
extern void exit(int status);

size_t savesp;
size_t install_handler(size_t *storeUboot,size_t num);
void restore_handler(size_t handler_addrs, size_t *storeUboot);


int kmain(int argc, char** argv, uint32_t table)
{
	app_startup(); /* bss is valid after this point */
	global_data = table;
	
	/* Add your code here */
	int status=0; 
    	size_t storeOld[2], handler_address;
	savesp=0;

	/* Set up interrupt controller */
	*INT_ICMR_ADDR |= (1 << INT_OSTMR_0);  // set OSTMR0 bit in ICMR to 1 to unmask OSTMR0
	*INT_ICLR_ADDR &= (~(1 << INT_OSTMR_0));  // set OSTMR0 bit in ICLR to 0, handle Timer0 with IRQ

	/* Timer setup */
	*OSTMR_OIER_ADDR &= 0; // mask all interrup at first
	*OSTMR_OIER_ADDR |= OSTMR_OIER_E0; // enable interrupt of timer 0
	*OSTMR_OSMR_ADDR(0) = OSTMR_FREQ / 100; // 10 ms after 36864 clock cycles

	 // install my handler
    	handler_address= install_handler(storeOld,SYSCALLS);
	if (handler_address == INS_ERROR )
       		return INS_ERROR;

	size_t storeOldIRQ[2], IRQ_handler_address;
	saveIRQsp = 0;
	IRQ_handler_address = install_handler(storeOldIRQ, IRQ_NUM);
	if (IRQ_handler_address == INS_ERROR)
		return INS_ERROR;
    
    	// check the arguments
    	// printf("kernel:argc:%d,argv:%s,%s,%s\n",argc,argv[0],argv[1],argv[2]) ;
 	
    	// switch to user mode
    	status=switch_mode(argc, argv);
   	//printf("switch over:%d\n",status);
   	
    	// restore original handler
    	restore_handler(handler_address,storeOld);
    	//puts("retore over!\n");

	// restore IRQ original handler
	restore_handler(IRQ_handler_address, storeOldIRQ);
    
    return status;
}



// install my handler
size_t install_handler(size_t *storeUboot, size_t num){
    size_t *addr= (size_t *)num;
    size_t instruction = *(addr);
    size_t signs = (instruction >> 23) &1;
    size_t offset;
    size_t addr_in_vec;	
    size_t handler_addr;
	
    // check LDR
    if ((instruction & 0xfffff000) != 0xe59ff000){
        puts("Unrecognized instruction\n");
        return INS_ERROR;
    }
    
    offset = instruction & 0xfff;
	
    // get address
    addr_in_vec = (signs==1)? (offset+0x10):(0x10-offset);
	//printf("addr_in_vec:%x\n",addr_in_vec);

    handler_addr=*((size_t *)addr_in_vec);
    
	//printf("handler:%x\n",handler_addr);

    // save the original handler
    storeUboot[0]= * ((size_t *) handler_addr);
    storeUboot[1]= * ((size_t *) (handler_addr+4));
    
    // set my handler
    // ldr pc, [pc, #-4]
    * ((size_t *) handler_addr) = 0xe51ff004;
    if (num == SYSCALLS)
        * ((size_t *) (handler_addr + 4)) = (size_t) &S_Handler;
    if (num == IRQ_NUM)
        * ((size_t *) (handler_addr + 4)) = (size_t) &IRQ_Handler;
    
    return handler_addr;
}

// restore the Uboot handler
void restore_handler(size_t handler_addrs, size_t *storeUboot){
    
    * ((size_t *) handler_addrs) = storeUboot[0];
    * ((size_t *) (handler_addrs + 4)) = storeUboot[1];

}

