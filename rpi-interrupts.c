
#include <stdint.h>
#include <stdbool.h>

#include "rpi-interrupts.h"

/**
 * @brief Initialize the pointers to the interrupt registers
 */
void interrupt_init(){
    IRQ *interrupt;
    interrupt = &irq;
    
    interrupt->IRQ_basic_pending = (uint32_t *)(IRQ_BASIC);
    interrupt->IRQ_pending_1= (uint32_t *)(IRQ_PEND1);
    interrupt->IRQ_pending_2= (uint32_t *)(IRQ_PEND2);
    interrupt->FIQ_control= (uint32_t *)(IRQ_FIQ_CONTROL);
    interrupt->Enable_IRQs_1= (uint32_t *)(IRQ_ENABLE1);
    interrupt->Enable_IRQs_2= (uint32_t *)(IRQ_ENABLE2);
    interrupt->Enable_Basic_IRQs= (uint32_t *)(IRQ_ENABLE_BASIC);
    interrupt->Disable_IRQs_1= (uint32_t *)(IRQ_DISABLE1);
    interrupt->Disable_IRQs_2= (uint32_t *)(IRQ_DISABLE2);
    interrupt->Disable_Basic_IRQs= (uint32_t *)(IRQ_DISABLE_BASIC); 
}


/**
 *	@brief The Reset vector interrupt handler
 * 
 *	This can never be called, since an ARM core reset would also reset the
 *	GPU and therefore cause the GPU to start running code again until
 *	the ARM is handed control at the end of boot loading
 */
void __attribute__((interrupt("ABORT"))) reset_vector(void)
{
    
}

/**
 *	@brief The undefined instruction interrupt handler
 * 
 *	If an undefined intstruction is encountered, the CPU will start
 *	executing this function. Just trap here as a debug solution.
 */
void __attribute__((interrupt("UNDEF"))) undefined_instruction_vector(void)
{
    while( 1 )
    {
	/* Do Nothing! */
    }
}


/**
 *	@brief The supervisor call interrupt handler
 * 
 *	The CPU will start executing this function. Just trap here as a debug
 *	solution.
 */
void __attribute__((interrupt("SWI"))) software_interrupt_vector(void)
{
    while( 1 )
    {
	/* Do Nothing! */
    }
}


/**
 *	@brief The prefetch abort interrupt handler
 * 
 *	The CPU will start executing this function. Just trap here as a debug
 *	solution.
 */
void __attribute__((interrupt("ABORT"))) prefetch_abort_vector(void)
{
    
}


/**
 *	@brief The Data Abort interrupt handler
 * 
 *	The CPU will start executing this function. Just trap here as a debug
 *	solution.
 */
void __attribute__((interrupt("ABORT"))) data_abort_vector(void)
{
    
}


/**
 *	@brief The IRQ Interrupt handler
 * 
 *	This handler is run every time an interrupt source is triggered. It's
 *	up to the handler to determine the source of the interrupt and most
 *	importantly clear the interrupt flag so that the interrupt won't
 *	immediately put us back into the start of the handler again.
 */
void __attribute__((interrupt("IRQ"))) interrupt_vector(void)
{
    _lock();
    
    
    //Access the memory space for interrupt registers
    IRQ *interrupt;
    interrupt = &irq;    
    /* go to all interrupt sources registers enabled in pendingIRQs
     * 
     *		
     *		
     *		
     *	//TODO: Add all the others 
     *	//bit 	10-20: GPU pending IRQs 
     *	//		8: pending reg 1
     *	//pending_reg_1 = IRQ_pending (1 << 9);
     *	//		7: Illegal acces type 0
     *	//		6: Illegal acces type 1
     *	//		5: GPU1 halted
     *	//		4: GPU0 halted (or GPU1 if bit 10 of control reg 1 is set)
     *	//		3: ARM doorbell 1
     *	//		2: ARM doorbell 2
     *	//		1: Mailbox
     *	//		0: Timer
     *	//we could _unlock() here but that would mean that handling of the interrupts can be interrupted...
     *	//TODO: add check to see which interrupt source is triggered and thus which handler to call.
     *	
     *	//if(pending_reg_1){
     *		//handle Aux interrupt
     *	//}
     */
    //uint32_t pending_reg_2_status = (*(interrupt->IRQ_basic_pending) >> 9) & 0x1;// this check returns a 0. Why?
    if(1){//if(pending_reg_2_status){ //it goes wrong here.
	uint32_t* pendingReg2 = (uint32_t *) interrupt->IRQ_pending_2;
	/* see which source triggered interupt
	 *		//
	 *		//TODO: Clear interrupt flag for handled interrupts
	 *		/*
	 *		//TODO: add the others
	 *		//11 = I2c_spi_slv
	 *		if(pending_IRQ2 & (1 <<  11)){
	 *		
    }
    //13 pwa0
    if(pending_IRQ2 & (1 <<  13)){
	
    }
    //14 pwa1
    if(pending_IRQ2 & (1 <<  14)){
	
    }
    //16 smi
    if(pending_IRQ2 & (1 <<  16)){
	
    }
    */
    //17-20 gpio0-gpio3
    
	
    
	if((*pendingReg2 >> 17) & 0x1){
	    gpio_handler(0);
	}
	if((*pendingReg2 >> 18) & 0x1){
	    gpio_handler(1);
	}
	if((*pendingReg2 >> 19) & 0x1){
	    gpio_handler(2);
	}
	if((*pendingReg2 >> 20) & 0x1){
	    gpio_handler(3);
	}
    //21 I2c
	if((*pendingReg2 >> 21) & 0x1){
	    i2c_handler();
	}
    //22 spi
     	//if(pending_IRQ2 & (1 <<  22)){
	//spi_handler();
	//}
    /* 
    //23 pcm
    if(pending_IRQ2 & (1 <<  23)){
	
    }
    */
    //25 uart
	if( (*pendingReg2 >> 25) & 0x1){
	    uart_handler(&uart); //CORRECT
	    //clear UART interrupt:
	    //disable then enable to clear (should be implemented differently to get more efficient code, probably
	    //RPI_GetIrqController() -> Disable_IRQs_2 = IRQ_2_ARM_UART_IRQ;
	    //RPI_GetIrqController() -> Enable_IRQs_2 = IRQ_2_ARM_UART_IRQ;
	}
    }
    _unlock();
}


/**
 *	@brief The FIQ Interrupt Handler
 * 
 *	The FIQ handler can only be allocated to one interrupt source. The FIQ has
 *	a full CPU shadow register set. Upon entry to this function the CPU
 *	switches to the shadow register set so that there is no need to save
 *	registers before using them in the interrupt.
 * 
 *	In C you can't see the difference between the IRQ and the FIQ interrupt
 *	handlers except for the FIQ knowing it's source of interrupt as there can
 *	only be one source, but the prologue and epilogue code is quite different.
 *	It's much faster on the FIQ interrupt handler.
 * 
 *	The prologue is the code that the compiler inserts at the start of the
 *	function, if you like, think of the opening curly brace of the function as
 *	being the prologue code. For the FIQ interrupt handler this is nearly
 *	empty because the CPU has switched to a fresh set of registers, there's
 *	nothing we need to save.
 * 
 *	The epilogue is the code that the compiler inserts at the end of the
 *	function, if you like, think of the closing curly brace of the function as
 *	being the epilogue code. For the FIQ interrupt handler this is nearly
 *	empty because the CPU has switched to a fresh set of registers and so has
 *	not altered the main set of registers.
 */
void __attribute__((interrupt("FIQ"))) fast_interrupt_vector(void)
{
    
}
