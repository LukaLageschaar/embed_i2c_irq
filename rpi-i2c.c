#include "rpi-i2c.h"

/**
 * @brief Initialize the BSC_0 I2C master and the allocated memory structure to allow
 * 	  automated reading and writing without loss of data
 * 
 * @return A pointer to the addressspace used for the I2C i2c_0 BSC struct
 */
I2C * i2c_init_BSC_0() {
    I2C *i2c_mem;
    i2c_mem = &i2c_0;
    i2c_mem->base_addr = (unsigned int*) I2C_BASE_BSC_0;
    set_clock_divider(750, i2c_mem); // 150MHz /200khz = 150 000 000/ 200 000= 1 500/ 2 
    enable_int_DONE(i2c_mem);
    
    CHANGE* mpr_c;
    mpr_c = &mpr_change;
    mpr_c->bsc0_1 = 0;
    mpr_c->bsc0_2 = 0;
    mpr_c->bsc0_3 = 0;
    mpr_c->bsc0_4 = 0;
    
    return i2c_mem;
}


/**
 * @brief Initialize the BSC_1 I2C master and the allocated memory structure to allow
 * 	  automated reading and writing without loss of data
 * 
 * @return A pointer to the addressspace used for the I2C i2c_1 BSC struct
 */
I2C * i2c_init_BSC_1() {
    I2C *i2c_mem;
    i2c_mem = &i2c_1;
    i2c_mem->base_addr = (unsigned int*) I2C_BASE_BSC_1;
    set_clock_divider(750, i2c_mem);
    enable_int_DONE(i2c_mem); // transfer done interrupts. 
    
    CHANGE* mpr_c;
    mpr_c = &mpr_change;
    mpr_c->bsc1_1 = 0;
    mpr_c->bsc1_2 = 0;
    mpr_c->bsc1_3 = 0;
    mpr_c->bsc1_4 = 0;
    
    return i2c_mem;
    
    
}

void i2c_get_change(I2C* i2c_mem){
    CHANGE* mpr_c;
    mpr_c = &mpr_change;
    REG *reg;
    reg = &last_reg;
    /*debug code*/
    BYTE *byte;
    byte = &last_byte;
    byte->b = 0;
    int bus = -1;
    while(!(mpr_c->bsc0_1) & !(mpr_c->bsc0_2) & !(mpr_c->bsc0_3) & !(mpr_c->bsc0_4) & !(mpr_c->bsc1_1) & !(mpr_c->bsc1_2) & !(mpr_c->bsc1_3) & !(mpr_c->bsc1_4)); //wait for change
    int reg0 = 0x00;
    int reg1 = 0x01;
    int* reg0_p = &reg0;
    int* reg1_p = &reg1;
    if(mpr_c->bsc0_1){
	reg->addr = reg0;
	read_bytes2(&i2c_0, MPR121_ADR_1, reg0_p, 1);
	reg->addr = reg1;
	read_bytes2(&i2c_0, MPR121_ADR_1, reg1_p, 1);
	
	mpr_c->bsc0_1 = 0;
	bus = 0;
    }
    if(mpr_c->bsc0_2){
	reg->addr = reg0;
	read_bytes2(&i2c_0, MPR121_ADR_2, reg0_p, 1);
	reg->addr = reg1;
	read_bytes2(&i2c_0, MPR121_ADR_2, reg1_p, 1);
	
	mpr_c->bsc0_2 = 0;
	bus = 0;
    }
    if(mpr_c->bsc0_3){
	reg->addr = reg0;
	read_bytes2(&i2c_0, MPR121_ADR_3, reg0_p, 1);
	reg->addr = reg1;
	read_bytes2(&i2c_0, MPR121_ADR_3, reg1_p, 1);
	
	mpr_c->bsc0_3 = 0;
	bus = 0;
    }
    if(mpr_c->bsc0_4){
	reg->addr = reg0;
	read_bytes2(&i2c_0, MPR121_ADR_4, reg0_p, 1);
	reg->addr = reg1;
	read_bytes2(&i2c_0, MPR121_ADR_4, reg1_p, 1);
	
	mpr_c->bsc0_4 = 0;
	bus = 0;
    }
    if(mpr_c->bsc1_1){
	reg->addr = reg0;
	read_bytes2(&i2c_1, MPR121_ADR_1, reg0_p, 1);
// 	reg->addr = reg1;
// 	read_bytes2(&i2c_1, MPR121_ADR_1, reg1_p, 1);

	mpr_c->bsc1_1 = 0;
	bus = 1;
    }
    if(mpr_c->bsc1_2){
	reg->addr = reg0;
	read_bytes2(&i2c_1, MPR121_ADR_2, reg0_p, 1);
	reg->addr = reg1;
	read_bytes2(&i2c_1, MPR121_ADR_2, reg1_p, 1);

	mpr_c->bsc1_2 = 0;
	bus = 1;
    }
    if(mpr_c->bsc1_3){
	reg->addr = reg0;
	read_bytes2(&i2c_1, MPR121_ADR_3, reg0_p, 1);
	reg->addr = reg1;
	read_bytes2(&i2c_1, MPR121_ADR_3, reg1_p, 1);

	mpr_c->bsc1_3 = 0;
	bus = 1;
    }
    if(mpr_c->bsc1_4){
	reg->addr = reg0;
	read_bytes2(&i2c_1, MPR121_ADR_4, reg0_p, 1);
	reg->addr = reg1;
	read_bytes2(&i2c_1, MPR121_ADR_4, reg1_p, 1);

	mpr_c->bsc1_4 = 0;
	bus = 1;
    }
    uprintf("Bus: %d, Reg: %d, data: %x \n\r", bus, reg->addr,byte->b);
    int i;
    for( i = 0; i<500000;i++);
    clear_pin(&gpio[22]);
    clear_pin(&gpio[24]);
    reg->addr = 0;
}

/**
 * @brief Handle a i2c interrupt(currently only INTD done interrupt enabled and handled)
 * 	  Currently coded so that data in one transfer can not be more than 16 bytes.
 */
int i2c_handler(){
    I2C* i2c_mem;
    REG *reg;
    reg = &last_reg;
    BYTE *byte;
    byte = &last_byte;
    int tim = 0;
    //Check which bus has a DONE interrupt
    int done_0 = is_transfer_done(&i2c_0);
    int done_1 = is_transfer_done(&i2c_1);
    
    if(done_0){	
	i2c_mem =  &i2c_0;
	unsigned int no_bytes = get_data_length(i2c_mem);
	if(no_bytes >0 || 0 > no_bytes){
	    set_pin(&gpio[25]);
	    //ERROR: transfer done but there are still bytes to transmit or read 
	    //(no_bytes > fifo length)
	    //Can happen at the same time as INTT and INTR interrupts but these are disabled
	    while(1); // catch in endless loop,
	}
	if(no_bytes == 0){
	    /*clear transfer done interrupt*/
	    reset_transfer_done(i2c_mem);
	    //get last set DLEN value
	    no_bytes = get_data_length(i2c_mem) - 1;
	}
	int gtt_0 = get_transfer_type(i2c_mem);
	int bytes[no_bytes];
	if(gtt_0){ //read
	    //find IRQ source:
	    int addr = get_slave_address(i2c_mem);
	    //read fifo
	    while (does_RX_contain_DATA(i2c_mem)) {
		//bytes[tim] = read_byte_from_FIFO(i2c_mem);
		byte->b = read_byte_from_FIFO(i2c_mem);
                //tim ++;
	    } 
	    tim = 0;
	    //TODO: program data transfer local mem to program 
	    /*switch case based on addr?*/
	    
	}
	else if(!gtt_0){//write
	    //No write interrupt handling necessary for our project
	}
    }
    if(done_1){
	i2c_mem =  &i2c_1;
	int no_bytes = get_data_length(i2c_mem);
	if(no_bytes >0){
	    set_pin(&gpio[25]);
	    //ERROR: transfer done but there are still bytes to transmit or read 
	    //(no_bytes > fifo length)
	    //Can happen at the same time as INTT and INTR interrupts but these are disabled
	    //In our project
	    while(1); // catch in endless loop,
	}
	if(no_bytes == 0){
	    /*clear transfer done interrupt*/
	    reset_transfer_done(i2c_mem);
	    no_bytes = get_data_length(i2c_mem) - 1;// -1 door het spelen met Start repeat
	}
	int bytes[no_bytes];
	int gtt_1 = get_transfer_type(i2c_mem);
	if(gtt_1){ //read
	    //find IRQ source:
	    int addr = get_slave_address(i2c_mem);
	    //read fifo
	    while (does_RX_contain_DATA(i2c_mem)) {
		byte->b = read_byte_from_FIFO(i2c_mem);
		//bytes[tim] = read_byte_from_FIFO(i2c_mem);
                //tim ++;
		set_pin(&gpio[22]);
	    } 
	    tim = 0;
	    //TODO: program data transfer local mem to program 
	    /*switch case based on addr?*/
	    
	}
	else if(!gtt_1){//write
	    //No write interrupt handling necessary for our project
	}
    }
}

/**
 * @brief Initialize a MPR121 breakout bord capacitive touch board
 * 
 * @param int The address of the MPR121 on the I2C bus (0x5A, 0x5B, 0x5C, 0x5D)
 * @param I2C* A pointer to the initalized I2C bus where the mpr121 is installed
 */

void mpr121_init(int address, I2C *i2c_mem){
    // first is command byte, second is data byte
    int i;
    // soft reset
    int bytes[2] = {0x80, 0x63};
    write_bytes(address, &bytes, 2, i2c_mem);

    // input op niet actief zetten
    int bytes2[2] = {0x5E, 0x00};
    int bytes4[2] = {0x5E, 0x00};
    write_bytes(address, &bytes2, 2, i2c_mem);

    int bytes3 = 0x5D;
    int bytes_out[2];
    //read_bytes(address, &bytes3, 1);
    //uprintf("READING: %x\n\r", bytes3 & 0xFF);
    read_bytes_init(i2c_mem, address, &bytes3, &bytes_out, 1);
    
    if(bytes_out[0] != 0x24){
	uprintf("Error initializing MPR121 at addr %c", address);
	while(1);
    }
    
    for ( i=0 ; i<12 ; i++){
        int bytes4[2] = {0x41 + 2*i, 0x30}; // 12
        int bytes5[2] = {0x42 + 2*i, 0x10}; // 6
        write_bytes(address, &bytes4, 2, i2c_mem);
        write_bytes(address, &bytes5, 2, i2c_mem);
    }


    bytes2[0] = 0x2B;
    bytes2[1] = 0x01;
    write_bytes(address, &bytes2, 2, i2c_mem);
    bytes2[0] = 0x2C;
    bytes2[1] = 0x01;
    write_bytes(address, &bytes2, 2, i2c_mem);
    bytes2[0] = 0x2D;
    bytes2[1] = 0x0E;
    write_bytes(address, &bytes2, 2, i2c_mem);
    bytes2[0] = 0x2E;
    bytes2[1] = 0x00;
    write_bytes(address, &bytes2, 2, i2c_mem);
    bytes2[0] = 0x2F;
    bytes2[1] = 0x01;
    write_bytes(address, &bytes2, 2, i2c_mem);
    bytes2[0] = 0x30;
    bytes2[1] = 0x05;
    write_bytes(address, &bytes2, 2, i2c_mem);
    bytes2[0] = 0x31;
    bytes2[1] = 0x01;
    write_bytes(address, &bytes2, 2, i2c_mem);
    bytes2[0] = 0x32;
    bytes2[1] = 0x00;
    write_bytes(address, &bytes2, 2, i2c_mem);
    bytes2[0] = 0x33;
    bytes2[1] = 0x00;
    write_bytes(address, &bytes2, 2, i2c_mem);
    bytes2[0] = 0x34;
    bytes2[1] = 0x00;
    write_bytes(address, &bytes2, 2, i2c_mem);
    bytes2[0] = 0x35;
    bytes2[1] = 0x00;
    write_bytes(address, &bytes2, 2, i2c_mem);
    bytes2[0] = 0x5B;
    bytes2[1] = 0x00;
    write_bytes(address, &bytes2, 2, i2c_mem);
    bytes2[0] = 0x5C;
    bytes2[1] = 0x10;
    write_bytes(address, &bytes2, 2, i2c_mem);
    bytes2[0] = 0x5D;
    bytes2[1] = 0x20;
    write_bytes(address, &bytes2, 2, i2c_mem);
    bytes2[0] = 0x5E;
    bytes2[1] = 0x8F;
    write_bytes(address, &bytes2, 2, i2c_mem);
    
    reset_transfer_done(i2c_mem);
}

/**
 * @brief Enables the I2C bus by setting Control register bit 15
 * 
 * @param I2C* A pointer to the I2C bus 
 */
void i2c_enable(I2C *i2c_mem) {
    i2c_mem->base_addr[I2C_CONTROL] |= (1 << 15);
}

/**
 * @brief Disables the I2C bus by clearing Control register bit 15
 * 
 * @param I2C* A pointer to the I2C bus 
 */
void i2c_disable(I2C *i2c_mem) {
    i2c_mem->base_addr[I2C_CONTROL] &= ~(1 << 15);
}

/**
 * @brief Enables the read interrupt on a I2C bus by setting Control register bit 10
 * 
 * @param I2C* A pointer to the I2C bus 
 */
void enable_int_RX(I2C *i2c_mem) {
    i2c_mem->base_addr[I2C_CONTROL] |= (1 << 10);
}

/**
 * @brief Disables the read interrupt on a I2C bus by clearing Control register bit 10
 * 
 * @param I2C* A pointer to the I2C bus 
 */
void disable_int_RX(I2C *i2c_mem) {
    i2c_mem->base_addr[I2C_CONTROL] &= ~(1 << 10);
}

/**
 * @brief Enables the write interrupt on a I2C bus by setting Control register bit 9
 * 
 * @param I2C* A pointer to the I2C bus 
 */
void enable_int_TX(I2C *i2c_mem) {
    i2c_mem->base_addr[I2C_CONTROL] |= (1 << 9);
}

/**
 * @brief Disables the write interrupt on a I2C bus by clearing Control register bit 9
 * 
 * @param I2C* A pointer to the I2C bus 
 */
void disable_int_TX(I2C *i2c_mem) {
    i2c_mem->base_addr[I2C_CONTROL] &= ~(1 << 9);
}

/**
 * @brief enable the communication done interrupt on a I2C bus by setting Control register bit 8
 * 
 * @param I2C* A pointer to the I2C bus 
 */
void enable_int_DONE(I2C *i2c_mem) {
    i2c_mem->base_addr[I2C_CONTROL] |= (1 << 8);
}

/**
 * @brief Disable the communication done interrupt on a I2C bus by setting Control register bit 8
 * 
 * @param I2C* A pointer to the I2C bus 
 */
void disable_int_DONE(I2C *i2c_mem) {
    i2c_mem->base_addr[I2C_CONTROL] &= ~(1 << 8);
}

/**
 * @brief Start a prepared communication on a I2C bus by setting Control register bit 7
 * 	  Calls function i2c_enable(I2C*)
 * 
 * @param I2C* A pointer to the I2C bus 
 */
void start_transfer(I2C *i2c_mem) {
    i2c_enable(i2c_mem);
    i2c_mem->base_addr[I2C_CONTROL] |= (1 << 7);
}

/**
 * @brief Clear data still in the FIFO on a I2C bus by setting Control register bit 4
 * 
 * @param I2C* A pointer to the I2C bus 
 */
void clear_FIFO_data(I2C *i2c_mem) {
    i2c_mem->base_addr[I2C_CONTROL] |= (1 << 4);
}

/**
 * @brief Set the type of communication of the next communication to a read communication
 * 	  by setting Control register bit 0
 * 
 * @param I2C* A pointer to the I2C bus 
 */
void set_read_transfer(I2C *i2c_mem) {
    i2c_mem->base_addr[I2C_CONTROL] |= (1 << 0);
}

/**
 * @brief Set the type of communication of the next communication to a write communication
 * 	  by clearing Control register bit 0
 * 
 * @param I2C* A pointer to the I2C bus 
 */
void set_write_transfer(I2C *i2c_mem) {
    i2c_mem->base_addr[I2C_CONTROL] &= ~(1 << 0);
}

/**
 * @brief returns the type of communication that has last been programmed
 * 
 * @param I2C* A pointer to the I2C bus 
 */
int get_transfer_type(I2C *i2c_mem) {
    return i2c_mem->base_addr[I2C_CONTROL] & 0x1;
}

/**
 * @brief Checks the 9th bit of the Status register for a Clock stretch timeout
 * 	  This means that the slave has held the clock line low 
 * 	  longer than specified in the I2CCLKT register
 * 
 * @param I2C* A pointer to the I2C bus 
 * @return One when the CLKT bit is set
 */

int is_CLKT(I2C *i2c_mem) {
    return (i2c_mem->base_addr[I2C_STATUS] >> 9) & 1;
}

/**
 * @brief Clear the Clock stretch timout check on bit 9 of the Status register
 * 
 * @param I2C* A pointer to the I2C bus 
 */
void reset_CLKT(I2C *i2c_mem) {
    i2c_mem->base_addr[I2C_STATUS] |= (1 << 9);
}

/**
 * @brief Checks if a slave has not acknowledged its address. Found in Status register bit 8
 * 
 * @param I2C* A pointer to the I2C bus 
 * @return A one when the slave does not acknoledges its address
 */
int is_ERR(I2C *i2c_mem) {
    return ((i2c_mem->base_addr[I2C_STATUS] >> 8) & 1);
}

/**
 * @brief Resets the slave has not acknowledged its address error at Status register bit 8
 * 
 * @param I2C* A pointer to the I2C bus 
 */
void reset_ERR(I2C *i2c_mem) {
    i2c_mem->base_addr[I2C_STATUS] |= (1 << 8);
}

/**
 * @brief Checks if the FIFO is full. Found in Status register bit 7
 * 
 * @param I2C* A pointer to the I2C bus 
 * @return A one when the FIFO is full. No more data can be received until data is read.
 */
int is_RX_FIFO_full(I2C *i2c_mem) {
    return (i2c_mem->base_addr[I2C_STATUS] >> 7) & 1;
}

/**
 * @brief Checks if the FIFO is empty. Found in Status register bit 6
 * 
 * @param I2C* A pointer to the I2C bus 
 * @return A one when the FIFO is empty. 
 * 	   No more data can be transmitted until data is written to the FIFO.
 */
int is_TX_FIFO_empty(I2C *i2c_mem) {
    return (i2c_mem->base_addr[I2C_STATUS] >> 6) & 1;
}

/**
 * @brief Checks if the FIFO contains data. Found in Status register bit 5
 * 
 * @param I2C* A pointer to the I2C bus 
 * @return A one when the FIFO has at least one byte of data. 
 * 	   Cleared by reading sufficient data
 */
int does_RX_contain_DATA(I2C *i2c_mem) {
    return (i2c_mem->base_addr[I2C_STATUS] >> 5) & 1;
}

/**
 * @brief Checks if the FIFO can accept more data. Found in Status register bit 4
 * 
 * @param I2C* A pointer to the I2C bus 
 * @return A one when the FIFO has space for at least one byte of data. 
 * 	   A zero when the FIFO is full
 */
int does_TX_accept_DATA(I2C *i2c_mem) {
    return (i2c_mem->base_addr[I2C_STATUS] >> 4) & 1;
}

/**
 * @brief Checks if the FIFO is full and a read is underway. Found in Status register bit 3
 * 
 * @param I2C* A pointer to the I2C bus 
 * @return A one when the FIFO is full and a read is underway
 * 	   Cleared by reading sufficient data
 */
int does_RX_need_reading(I2C *i2c_mem) {
    return (i2c_mem->base_addr[I2C_STATUS] >> 3) & 1;
}

/**
 * @brief Checks if the FIFO is full and a read is write. Found in Status register bit 2
 * 
 * @param I2C* A pointer to the I2C bus 
 * @return A one when the FIFO has less than sufficient data for a transmit.
 * 	   Cleared by writing sufficient data to the FIFO
 */
int does_TX_need_writing(I2C *i2c_mem) {
    return (i2c_mem->base_addr[I2C_STATUS] >> 2) & 1;
}

/**
 * @brief Checks to see if the current communication is still ongoing. Found in Status register bit 1
 * 
 * @param I2C* A pointer to the I2C bus 
 * @return A one when the current communication has finished
 */
int is_transfer_done(I2C *i2c_mem) {
    return (i2c_mem->base_addr[I2C_STATUS] >> 1) & 1;
}

/**
 * @brief Writes a one to Status register bit 1 to clear the DONE flag.
 * 
 * @param I2C* A pointer to the I2C bus 
 */
void reset_transfer_done(I2C *i2c_mem) {
    i2c_mem->base_addr[I2C_STATUS] |= (1 << 1);
}

/**
 * @brief Checks to see if the current communication is still ongoing. Found in Status register bit 1
 * 
 * @param I2C* A pointer to the I2C bus 
 * @return A one when the current communication is active
 */
int is_transfer_active(I2C *i2c_mem) {
    return (i2c_mem->base_addr[I2C_STATUS] >> 0) & 1;
}

/**
 * @brief Resets the DONE, CLKT and ERR flag
 * 	  Calls function reset_CLKT(I2C*)
 * 			 reset_ERR(I2C*)
 * 			 reset_transfer_done(I2C*)
 * @param I2C* A pointer to the I2C bus 
 */
void reset_status_register(I2C *i2c_mem) {
    reset_CLKT(i2c_mem);
    reset_ERR(i2c_mem);
    reset_transfer_done(i2c_mem);
}

/**
 * @brief Sets the number of bytes to be transmitted or received
 * 
 * @param int number of bytes
 * @param I2C* A pointer to the I2C bus 
 */
void set_data_length(int num,I2C *i2c_mem) {
    i2c_mem->base_addr[I2C_DLEN] = num;
}

/**
 * @brief Requests the amount of data still to be transmitted/received 
 * 		when Bit 0 or Bit 1 of Status register is one
 *	  Or the last value it was set to
 * 		when Bit 0 and Bit 1 of Status register are zero
 *  	  
 * @param I2C* A pointer to the I2C bus 
 * @return The amount of data to still be transmitted or the last value DLEN was set to
 */
int get_data_length(I2C *i2c_mem) {
    return (i2c_mem->base_addr[I2C_DLEN]);
}

/**
 * @brief Sets the address of the Slave that will be communicated with
 * 
 * @param int slave address
 * @param I2C* A pointer to the I2C bus 
 */
void set_slave_address(int address,I2C *i2c_mem) {
    i2c_mem->base_addr[I2C_SLAVE_ADDRESS] = address; 
}

/**
 * @brief Requests the last address that was written
 * 
 * @param I2C* A pointer to the I2C bus 
 * @return slave address
 */
int get_slave_address(I2C *i2c_mem) {
    return (i2c_mem->base_addr[I2C_SLAVE_ADDRESS] << 25); 
}

/**
 * @brief Sets the Clock divider to change the I2C bus clock 150MHz / devisor
 *        Default speed is 100kHz
 * 
 * @param int divisor value
 * @param I2C* A pointer to the I2C bus 
 */
void set_clock_divider(int divisor,I2C *i2c_mem) {
    i2c_mem->base_addr[CLOCK_DIVIDER] ^= divisor;
}

/**
 * @brief Reads the divisor value
 * 
 * @param I2C* A pointer to the I2C bus 
 * @return current divisor value
 */
int get_clock_divider(I2C *i2c_mem) {
    return (i2c_mem->base_addr[CLOCK_DIVIDER]);
}

/**
 * Data delay register
 */

void set_falling_edge_delay(int delay,I2C *i2c_mem) {
    i2c_mem->base_addr[DATA_DELAY] ^= (delay << 16);
}

int get_falling_edge_delay(I2C *i2c_mem) {
    return (i2c_mem->base_addr[DATA_DELAY] >> 16);
}

void set_rising_edge_delay(int delay,I2C *i2c_mem) {
    i2c_mem->base_addr[DATA_DELAY] ^= delay;
}

int get_rising_edge_delay(I2C *i2c_mem) {
    return (i2c_mem->base_addr[DATA_DELAY] << 16);
}

/**
 * Clock stretch timeout register
 */

void set_clock_stretch_timeout(int timeout,I2C *i2c_mem) {
    i2c_mem->base_addr[CLOCK_STRETCH_TIMEOUT] ^= timeout;
}

int get_clock_stretch_timeout(I2C *i2c_mem) {
    return (i2c_mem->base_addr[CLOCK_STRETCH_TIMEOUT] << 16);
}

/**
 * @brief Reads one byte of data from the 16 byte FIFO
 * 
 * @param I2C* A pointer to the I2C bus 
 * @return One byte of data
 */
int read_byte_from_FIFO(I2C *i2c_mem) {
    return (i2c_mem->base_addr[DATA_FIFO] & 0xFF);
}

/**
 * @brief Writes one byte of data that needs to be added to the FIFO
 * 
 * @param u8 Byte worth of data
 * @param I2C* A pointer to the I2C bus 
 */
void write_byte_to_FIFO(u8 byte, I2C *i2c_mem) {
	i2c_mem->base_addr[DATA_FIFO] = byte & 0xFF;
}

/**
 * @brief Writes multiple bytes of data sequentially to the FIFO
 * 
 * @param I2C* A pointer to the I2C bus 
 * @param uint* pointer to the first byte of data from an array
 * @param int amount of bytes of data
 */
void write_bytes_to_FIFO(I2C *i2c_mem, unsigned int *bytes, unsigned int no_bytes) {
    unsigned int i;
    for (i=0; i<no_bytes && i<16; i++) {
        i2c_mem->base_addr[DATA_FIFO] = (*(bytes+i) & 0xFF);
    }
}

/**
 * @brief Read bytes from I2C
 * 
 * @param I2C* A pointer to the I2C bus
 * @param u32 Address of the slave
 * @param u32* Pointer to the addressspace containing the register to be read from slave
 * @param u32 amount of bytes to read from register in slave
 */
void read_bytes2(I2C *i2c_mem,unsigned int I2C_address, unsigned int *bytes_in, unsigned int no_bytes) {
	
    //SETUP
    //uprintf("Setup \n\r");
    //for(tim = 0; tim < 5000000; tim++);
    
    reset_status_register(i2c_mem);
    clear_FIFO_data(i2c_mem);
    set_slave_address(I2C_address, i2c_mem);
    set_data_length(no_bytes, i2c_mem);
    
    //WRITE
    //uprintf("Start \n\r");
    //for(tim = 0; tim < 5000000; tim++);
    
    write_bytes_to_FIFO(i2c_mem,bytes_in, 1);    
    set_write_transfer(i2c_mem);
    start_transfer(i2c_mem);   
    // WANNEER DE TRANSFER ACTIEF IS, GAAN WE VERDER
    do{
	//...
    } while (!is_transfer_active(i2c_mem));
    
    //uprintf("Repeated Start \n\r");
    //for(tim = 0; tim < 5000000; tim++);
    set_data_length(no_bytes + 1, i2c_mem);
    set_read_transfer(i2c_mem);
    start_transfer(i2c_mem);
    
    //To make sure the i2c_get_change function doesn't damage an ongoing transfer
    while (is_transfer_active(i2c_mem));
    //{
// 	// Check if the FIFO is full but not all the data has been transmitted
// 	if (does_RX_need_reading(i2c_mem)) {
// 	    unsigned int i;
// 	    for (i=0; i<16; i++) {
// 		char c = read_byte_from_FIFO(i2c_mem);
// 		//uprintf("read data \n\r", c);
// 		i2c_mem->inbuf[i2c_mem->inhead++] = c;
// 		i2c_mem->inhead %= SBUFSIZE;
// 		i2c_mem->indata++; i2c_mem->inroom--;
// 		no_bytes--;
// 	    }
// 	} 
// 	else if (is_ERR(i2c_mem)) {
// 	    uprintf("ERROR HAPPENED WHILE WRITING TO FIFO\n\r");
// 	    //for(tim = 0; tim < 5000000; tim++);
// 	    //NACK received
// 	    break;
// 	}
//     }
//     
//     //read remaining data in the fifo
//     unsigned int i;
//     for (i=0; i<no_bytes; i++) {
// 	i2c_mem->inbuf[i2c_mem->inhead++] = read_byte_from_FIFO(i2c_mem);
// 	i2c_mem->inhead %= SBUFSIZE;
// 	i2c_mem->indata++; i2c_mem->inroom--;
//     }
}

/**
 * @brief write bytes to I2C slave
 * 
 * @param u32 Address of the slave
 * @param u32* Pointer to the addressspace containing the first of the bytes to write
 * @param u32 amount of bytes to read from register in slave
 * @param I2C* A pointer to the I2C bus
 */
void write_bytes(unsigned int I2C_address, unsigned int *bytes, unsigned int no_bytes, I2C *i2c_mem) {
    int tim;
    reset_status_register(i2c_mem);
    clear_FIFO_data(i2c_mem);
    
    set_slave_address(I2C_address, i2c_mem);
    
    write_bytes_to_FIFO(i2c_mem, bytes, no_bytes);    
    
    set_data_length(no_bytes, i2c_mem);
    
    set_write_transfer(i2c_mem);
    start_transfer(i2c_mem);   
    
    while(!is_transfer_done(i2c_mem)) {
	
	// check if the FIFO is empty but not all the data has been transmitted
	if (does_TX_need_writing(i2c_mem)) {
	    
	    no_bytes -= 16;
	    bytes += 16;
	    write_bytes_to_FIFO(i2c_mem, bytes, no_bytes);
	}
	else if (is_ERR(i2c_mem)) {
	    //NACK
	    uprintf("ERROR HAPPENNENENDN WHILE WRITING TO FIFO");
	    //for(tim = 0; tim < 5000000; tim++);
	    break;
	}
    }
}


// void read_8bit_reg(u8 reg_addr, I2C* i2c_mem, unsigned int i2c_address) {
//     //uprintf("reading addr \n\r", reg_addr);
//     int no_bytes = 1;
//     
//     write_reg_addr(i2c_address, reg_addr, i2c_mem);
//     
//     reset_status_register(i2c_mem);
//     clear_FIFO_data(i2c_mem);
//     
//     set_slave_address(i2c_address, i2c_mem);
//     set_data_length(1,i2c_mem);
//     set_read_transfer(i2c_mem);
//     
//     start_transfer(i2c_mem);   
//     while (!is_transfer_done(i2c_mem)) {
// 	// Check if the FIFO is full but not all the data has been transmitted
// 	if (does_RX_need_reading(i2c_mem)) {
// 	    unsigned int i;
// 	    for (i=0; i<16; i++) {
// 		char c = read_byte_from_FIFO(i2c_mem);
// 		//uprintf("read data \n\r", c);
// 		i2c_mem->inbuf[i2c_mem->inhead++] = c;
// 		i2c_mem->inhead %= SBUFSIZE;
// 		i2c_mem->indata++; i2c_mem->inroom--;
// 		no_bytes--;
// 	    }
// 	} 
// 	else if (is_ERR(i2c_mem)) {
// 	    //NACK received
// 	    break;
// 	}
//     }
//     
//     //read remaining data in the fifo
//     unsigned int i;
//     for (i=0; i<no_bytes; i++) {
// 	i2c_mem->inbuf[i2c_mem->inhead++] = read_byte_from_FIFO(i2c_mem);;
// 	i2c_mem->inhead %= SBUFSIZE;
// 	i2c_mem->indata++; i2c_mem->inroom--;
//     }
// }
// 
// void write_reg_addr(unsigned int address, u8 reg_addr, I2C *i2c_mem) {
//     reset_status_register(i2c_mem);
//     clear_FIFO_data(i2c_mem);
//     set_slave_address(address,i2c_mem);
//     write_byte_to_FIFO(reg_addr, 1,i2c_mem);    
//     set_data_length(1,i2c_mem);
//     set_write_transfer(i2c_mem);
//     start_transfer(i2c_mem);
//     while(!is_transfer_done(i2c_mem)); // wait till correct register has been adressed
// }

void read_bytes_init(I2C *i2c_mem,unsigned int address, unsigned int *bytes_in, unsigned int *bytes_out, unsigned int no_bytes) {
    int tim;
    //SETUP
    //uprintf("Setup \n\r");
    //for(tim = 0; tim < 5000000; tim++);
    
    reset_status_register(i2c_mem);
    clear_FIFO_data(i2c_mem);
    set_slave_address(address, i2c_mem);
    set_data_length(no_bytes, i2c_mem);
    
    //WRITE
    //uprintf("Start \n\r");
    //for(tim = 0; tim < 5000000; tim++);
    
    write_bytes_to_FIFO(i2c_mem, bytes_in, 1);    
    set_write_transfer(i2c_mem);
    start_transfer(i2c_mem);   
    
    // WANNEER DE TRANSFER ACTIEF IS, GAAN WE VERDER
    do{
	//...
    } while (!is_transfer_active(i2c_mem));
    
    //uprintf("Repeated Start \n\r");
    //for(tim = 0; tim < 5000000; tim++);
    set_data_length(no_bytes + 1, i2c_mem);
    set_read_transfer(i2c_mem);
    start_transfer(i2c_mem);
    
    while (!is_transfer_done(i2c_mem)) {
	// Check if the FIFO is full but not all the data has been transmitted
	if (does_RX_need_reading(i2c_mem)) {
	    unsigned int i;
	    for (i=0; i<16; i++) {
		*bytes_out = read_byte_from_FIFO(i2c_mem);
		bytes_out++;
		no_bytes--;
	    }
	} 
	else if (is_ERR(i2c_mem)) {
	    uprintf("ERROR HAPPENED WHILE WRITING TO FIFO\n\r");
	    //for(tim = 0; tim < 5000000; tim++);
	    //NACK received
	    break;
	}
    }
    
    //read remaining data in the fifo
    unsigned int i;
    for (i=0; i<no_bytes; i++) {
	*bytes_out = read_byte_from_FIFO(i2c_mem);
	bytes_out++;
    }
}

/*
void write_bytes(unsigned int address, unsigned int no_bytes, I2C *i2c_mem) {
    reset_status_register(i2c_mem);
    clear_FIFO_data(i2c_mem);
    
    set_slave_address(address,i2c_mem);
    
    write_bytes_to_FIFO(bytes, no_bytes,i2c_mem);    
    
    set_data_length(no_bytes,i2c_mem);
    
    set_write_transfer(i2c_mem);
    start_transfer(i2c_mem);   
    
    while(!is_transfer_done(i2c_mem)) {
	// check if the FIFO is empty but not all the data has been transmitted
	if (does_TX_need_writing(i2c_mem)) {
	    no_bytes -= 16;
	    bytes += 16;
	    write_bytes_to_FIFO(bytes, no_bytes,i2c_mem);
	}
	else if (is_ERR(i2c_mem)) {
	    //NACK
	    break;
	}
    }
}

void read_bytes(unsigned int address, unsigned int *bytes, unsigned int no_bytes) {
    reset_status_register();
    clear_FIFO_data();

    set_slave_address(address);
    set_data_length(no_bytes);
    set_read_transfer();
    start_transfer();

    while (!is_transfer_done()) {
        // Check if the FIFO is full but not all the data has been transmitted
        if (does_RX_need_reading()) {
            unsigned int i;
            for (i=0; i<16; i++) {
                *bytes = read_byte_from_FIFO();
                bytes++;
                no_bytes--;
            }
        } 
        else if (is_ERR()) {
            //NACK received
            break;
        }
    }

    //read remaining data in the fifo
    unsigned int i;
    for (i=0; i<no_bytes; i++) {
        *bytes = read_byte_from_FIFO();
        bytes++;
    }
}

int read_status_reg(I2C *i2c_mem) {
    return i2c_mem->base_addr[I2C_STATUS];
}


*/