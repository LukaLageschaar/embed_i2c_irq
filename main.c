/********************************************************************
 * Main file inspired by the armc-013.c file from the Raspberry-Pi Bare
 * Metal Tutorials by Brian Sidebotham and modified to implement an
 * interrupt-driven device driver for the Raspberry Pi 1 Model b+.
 ********************************************************************/

#include "rpi-interrupts.h" // other exception handlers
#include "rpi-gpio.h"
#include "rpi-uart.h"
#include "rpi-i2c.h"
#include "defines.h"
#include <stdio.h>

// initialize global constants

const int I2C_SDA_BSC_0_pin = 0;
const int I2C_SCL_BSC_0_pin = 1;
const int I2C_SDA_BSC_1_pin = 2;
const int I2C_SCL_BSC_1_pin = 3;

const int UART_TX_pin = 14;
const int UART_RX_pin = 15;

const int MPR121_ADR_1 = 0x5A;
const int MPR121_ADR_2 = 0x5B;
const int MPR121_ADR_3 = 0x5C;
const int MPR121_ADR_4 = 0x5D;

// const int I2C_BSC_0_MPR_1_IRQ_pin;
// const int I2C_BSC_0_MPR_2_IRQ_pin;
// const int I2C_BSC_0_MPR_3_IRQ_pin;
// const int I2C_BSC_0_MPR_4_IRQ_pin;
const int I2C_BSC_1_MPR_1_IRQ_pin = 17;
// const int I2C_BSC_1_MPR_2_IRQ_pin;
// const int I2C_BSC_1_MPR_3_IRQ_pin;
// const int I2C_BSC_1_MPR_4_IRQ_pin;


void kernel_main( unsigned int r0, unsigned int r1, unsigned int atags )
{
    _lock();
    gpio_init();
    
    //MPR121 05xA op bus 1
    set_GPIO_input(&gpio[I2C_BSC_1_MPR_1_IRQ_pin]);
    
    set_GPIO_output(&gpio[22]);
    set_GPIO_output(&gpio[24]);
    set_GPIO_output(&gpio[25]);
    
    enable_edge_detect(&gpio[I2C_BSC_1_MPR_1_IRQ_pin]);
    
    //UART
    set_GPIO_alterfunc(&gpio[UART_TX_pin], 4);
    set_GPIO_alterfunc(&gpio[UART_RX_pin], 4);
    
    uart_init();
    
    //I2C
    //BSC 1
    
    //don't use internal pullups, use external >4.7kohm pullups
    set_GPIO_alterfunc(&gpio[I2C_SDA_BSC_1_pin], 4);
    set_GPIO_alterfunc(&gpio[I2C_SCL_BSC_1_pin], 4);
    
    I2C *i2c_BSC_1 = i2c_init_BSC_1();
    mpr121_init(MPR121_ADR_1, i2c_BSC_1);
    //interrupts
    interrupt_init();
    IRQ *interrupt;
    interrupt = &irq;
    *(interrupt->Enable_Basic_IRQs) |= BASIC_PENDING_REGISTER_2_IRQ;	//enable basic interrupt sources
    *(interrupt->Enable_IRQs_2) |= IRQ_2_ARM_UART_IRQ;	//enable UART interrupt sources
    *(interrupt->Enable_IRQs_2) |= IRQ_2_ARM_I2C_IRQ;	//enable I2C interrupt sources
    *(interrupt->Enable_IRQs_2) |= (1 << 17);	//enable gpio0 interrupt sources
    *(interrupt->Enable_IRQs_2) |= (1 << 18);	//enable gpio1 interrupt sources
    *(interrupt->Enable_IRQs_2) |= (1 << 19);	//enable gpio2 interrupt sources
    *(interrupt->Enable_IRQs_2) |= (1 << 20);	//enable gpio3 interrupt sources
    
    /* Enable interrupts */
	//start interrupt handling 
    _unlock();

    int tim;
    char bytes[12];
    char *byte = &bytes;
    int commando = 0x00;
    int * lees_p = &commando;
    int commando2 = 0x01;
    int * lees_p2 = &commando2;
    CHANGE* mpr_c;
    mpr_c = &mpr_change;
    while(1){
	i2c_get_change(i2c_BSC_1);
// 	for(tim = 0; tim < 5000000; tim++);
//         int bytes_out[1];
// 	bytes_out[1] = i2c_get_byte(i2c_BSC_1);
//         uprintf("READING 0x00: %x\n\r", *(bytes_out) & 0xFF);
// 	for(tim = 0; tim < 5000000; tim++);
// 	bytes_out[1] = i2c_get_byte(i2c_BSC_1);
//         uprintf("READING 0x01: %x\n\r", *(bytes_out) & 0xFF);
//         for(tim = 0; tim < 5000000; tim++);
// 
//         uprintf("--------------------------------\n\r");
//         for(tim = 0; tim < 5000000; tim++);
    }
}
    
    /*
    int tim;
    while(1){
	// restart a new game, 
	init_board();
	int winner = 0;
	
	while( winner == 0){
	    //Show the board
	    uprintf("\n\rBoard Updated:\n\r");
	    for(tim = 0; tim < 500000; tim++); // de delay is nodig om de buffer hier te laten leeglopen. 
	    display_board_color();
	    for(tim = 0; tim < 5000000; tim++); // de delay is nodig om de buffer hier te laten leeglopen. 
	    
	    int move_rejected = 0;
	    while(move_rejected == 0){
		//Make a move
		uprintf("Make a move: (ex. A2 A4)\n\r");
		for(tim = 0; tim < 500000; tim++);
		line_size = ugets(up, line);
		while (line_size != 5){
		    uprintf("Wrong format. Try writing it like this: A2 B7\ngetting data\r");
		    for(tim = 0; tim < 500000; tim++);
		    line_size = ugets(up, line);
		}        
		
		
		//Parse the move that was accepted:
		char *line_pointer;
		line_pointer = &line;
		char *from_x = line_pointer;
		char *from_y = line_pointer+1;
		char *to_x = line_pointer+3;
		char *to_y = line_pointer+4;
		uprintf("Making move from %c%c to %c%c\n\r", *from_x, *from_y, *to_x, *to_y);
		for(tim = 0; tim < 500000; tim++);
		
		
		//Execute the move
		// TODO: only execute if it is a legal move
		if (check_move_char(*from_x, *from_y, *to_x, *to_y)){
		    for(tim = 0; tim < 500000; tim++);
		    move_rejected =1;
		    move_char(*from_x, *from_y, *to_x, *to_y);
		    for(tim = 0; tim < 500000; tim++);
		} else {
		    uprintf("Illegal move. It is rejected.\n\r");
		    for(tim = 0; tim < 500000; tim++);
		}
	    }
	    winner = checkGameOver();
	}
	if (winner ==1) {
	    uprintf("\n\rThe winner is WHITE!\n\r\n\rPress enter to play again\n\r");
	} else {
	    uprintf("\n\rThe winner is BLACK!\n\r\n\rPress enter to play again\n\r");
	}
	for(tim = 0; tim < 500000; tim++);
	line_size = ugets(up, line);
	
	
    }
    */