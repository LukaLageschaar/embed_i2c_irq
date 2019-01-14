/********************************************************************
This c file is based on the original implementation of gpio.c
as described by K.C. Wang, <kwang@eecs.wsu.edu> in the book:
"Embedded and Real-Time Operation Systems" and modified for use 
on the Raspberry Pi 1 Model b+.

Additional comments/functions are added for a better explanation/
more functionality. 
********************************************************************/
#include "rpi-gpio.h"
#include "rpi-i2c.h"


/**
 * @brief activate the pull up circuit on a pin
 * 
 * @param GPIO* address of the gpio pin. format: &gpio[pin_number]
 */
void set_GPIO_pullup(GPIO *gp) {
    gpio_reg[GPIO_GPPUD] = 0b10;
    int i;
    for (i = 0; i<150; i++);
    gpio_reg[GPIO_GPPUDCLK0] = (1 << gp->gpio_bit);
    for (i = 0; i<150; i++);
    gpio_reg[GPIO_GPPUD] = 0;
    gpio_reg[GPIO_GPPUDCLK0] = (0 << gp->gpio_bit);
}

/**
 * @brief clear the pull up or pull down circuit on all pins
 */
void clear_GPIO_pullup_down() {
    gpio_reg[GPIO_GPPUD] = 0b00;
    int i;
    for (i = 0; i<150; i++);
    gpio_reg[GPIO_GPPUDCLK0] = -1;
    for (i = 0; i<150; i++);
    gpio_reg[GPIO_GPPUDCLK0] = 0;
    gpio_reg[GPIO_GPPUD] = 0b00;
}

/** @brief GPIO initialization. For each gpio the correct registers are stored 
 * 	   in variables declaired in the gpio struct 
 */
void gpio_init(){
	gpio_reg = (unsigned int*)GPIO_BASE;
	int i,j;
	int tiental;
	GPIO *gp;
	for(i = 0; i<=25;i++){
		j = i+2;
		gp = &gpio[j];
		gp->bit = (j%10)*3;
		gp->set = GPIO_GPSET0;
		gp->clr = GPIO_GPCLR0;
		gp->enable_falling_edge = GPIO_GPFEN0;
		gp->a_enable_falling_edge = GPIO_GPAFEN0;
		gp->detect_edge = GPIO_GPEDS0;
		gp->gpio_bit = j;
		
		tiental = j/10;
		switch (tiental){
			case 0:
				gp->sel = GPIO_GPFSEL0;
				break;
			case 1:
				gp->sel = GPIO_GPFSEL1;
				break;
			case 2:
				gp->sel = GPIO_GPFSEL2;
				break;
			default: break;
		}
	}

	clear_GPIO_pullup_down();
}

/**
 * @brief Alter the gpio its functionality to an alternate function 
 * 
 * @param GPIO* address of the gpio pin. format: &gpio[pin_number]
 * @param int Alternate function 0 tot 5
 */
void set_GPIO_alterfunc(GPIO *gp, int n){
	gpio_reg[gp->sel] |= (n << gp->bit);
}

/**
 * @brief Set the gpio as an output 
 * 
 * @param GPIO* address of the gpio pin. format: &gpio[pin_number]
 */
void set_GPIO_output(GPIO *gp){	
	gpio_reg[gp->sel] |= (1 << gp->bit);
}

/**
 * @brief Set the gpio as an input 
 * 
 * @param GPIO* address of the gpio pin. format: &gpio[pin_number]
 */
void set_GPIO_input(GPIO *gp){	
	gpio_reg[gp->sel] &= ~( 7<< gp->bit);
}

/**
 * @brief Set gpio output HIGH 
 * 
 * @param GPIO* address of the gpio pin. format: &gpio[pin_number]
 */
void set_pin(GPIO *gp){
	gpio_reg[gp->set] = (1 << gp->gpio_bit);
}

/**
 * @brief Set gpio output LOW 
 * 
 * @param GPIO* address of the gpio pin. format: &gpio[pin_number]
 */
void clear_pin(GPIO *gp){
	gpio_reg[gp->clr] = (1 << gp->gpio_bit);
}

/**
 * @brief Help function to clear the edge detect event 
 * 
 * @param GPIO* address of the gpio pin. format: &gpio[pin_number]
 */
void clear_detect(GPIO *gp){
	gpio_reg[gp->detect_edge] = (1 << gp->gpio_bit);
}

/**
 * @brief Help function to enable the edge detect event
 * 
 * @param GPIO* address of the gpio pin. format: &gpio[pin_number]
 */
void enable_falling_edge_async(GPIO *gp){
	gpio_reg[gp->a_enable_falling_edge] |= (1 << gp->gpio_bit);
}

/**
 * @brief Help function to enable the synchronous edge detect event
 * 
 * @param GPIO* address of the gpio pin. format: &gpio[pin_number]
 */
void enable_falling_edge_sync(GPIO *gp){
	gpio_reg[gp->enable_falling_edge] |= (1 << gp->gpio_bit);
}

/**
 * @brief Enable edge detect event 
 * 
 * @param GPIO* address of the gpio pin. format: &gpio[pin_number]
 */
void enable_edge_detect(GPIO *gp){
    enable_falling_edge_sync(gp);
    enable_falling_edge_async(gp);
}

/**
 * @brief  Checks bit value for a specific register and corresponding bit number 
 * 
 * @param int the bit number you want to read from the register data, 
 * 	      usually 0 to 31 for 32 bit registers
 * @param int the register data
 * @return Returns status of the requested bit 0-1
 */
int getBit(int bitNumber, int reg){	
	int bit = (reg >> bitNumber);
	return bit &= 1;
}

/**
 * @brief Edge detect event check 
 * 
 * @param GPIO* address of the gpio pin. format: &gpio[pin_number]
 * @return Returns a one on a edge detect
 */
int edge_detect(GPIO *gp){
	if(getBit(gp->gpio_bit, gpio_reg[gp->detect_edge])){
		clear_detect(gp);		
		return 1;
	}
	else{return 0;}
}

/**
 * @brief Interrupt handler for gpio interrupts from MPR121 sources
 * 
 * @param int number representing which gpio bank has send the interrupt 0-3 
 */
void gpio_handler(int int_reg){
	u32 *addr; 
	if (int_reg == 0){
		addr = (u32*)&gpio_reg[GPIO_GPEDS0];
	}
	else if (int_reg == 1){
		addr = (u32*)&gpio_reg[GPIO_GPEDS1];
	}
	else{uprintf("error gpio_handler \n\r");}
	CHANGE* mpr_c;
	mpr_c = &mpr_change;
// 	if((*addr >> I2C_BSC_0_MPR_1_IRQ_pin) & 0x1){
// 	    mpr_c->bsc0_1 = 1;
// 	}
// 	if((*addr >> I2C_BSC_0_MPR_2_IRQ_pin) & 0x1){
//		mpr_c->bsc0_2 = 1;
// 	}
// 	if((*addr >> I2C_BSC_0_MPR_3_IRQ_pin) & 0x1){
//		mpr_c->bsc0_3 = 1;
// 	}
// 	if((*addr >> I2C_BSC_0_MPR_4_IRQ_pin) & 0x1){
//		mpr_c->bsc0_4 = 1;
// 	}
	if((*addr >> I2C_BSC_1_MPR_1_IRQ_pin) & 0x1){
	    mpr_c->bsc1_1 = 1;
	}
// 	if((*addr >> I2C_BSC_1_MPR_2_IRQ_pin) & 0x1){
//		mpr_c->bsc1_2 = 1;
// 	}
// 	if((*addr >> I2C_BSC_1_MPR_3_IRQ_pin) & 0x1){
//		mpr_c->bsc1_3 = 1;
// 	}
// 	if((*addr >> I2C_BSC_1_MPR_4_IRQ_pin) & 0x1){
//		mpr_c->bsc1_4 = 1;
// 	}
	
	
	/*else if (int_reg == 2){
		addr = 5;
	}
	else if (int_reg == 3){
		addr = 5;
	}
	*/
	//clear gpio event detect status interrupts!
	if (int_reg == 0){
	    gpio_reg[GPIO_GPEDS0] &= (*addr);
	}
	else if (int_reg == 1){
	    gpio_reg[GPIO_GPEDS1] &= (*addr);
	}
}