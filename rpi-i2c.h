#ifndef RPI_I2C_H
#define RPI_I2C_H

#include "defines.h"
#include "rpi-uart.h"
#include "rpi-gpio.h"

static volatile unsigned int* i2c_reg;

typedef volatile struct i2c{
  unsigned int *base_addr;
}I2C;

typedef volatile struct last_reg{
  unsigned int addr;
}REG;

REG last_reg;

typedef volatile struct last_byte{
  int b;
}BYTE;

BYTE last_byte;

typedef volatile struct mpr121_change{
  unsigned int bsc0_1;
  unsigned int bsc0_2;
  unsigned int bsc0_3;
  unsigned int bsc0_4;
  unsigned int bsc1_1;
  unsigned int bsc1_2;
  unsigned int bsc1_3;
  unsigned int bsc1_4;
}CHANGE;

CHANGE mpr_change;
I2C i2c_0;
I2C i2c_1;

extern I2C* i2c_init_BSC_0();
extern I2C* i2c_init_BSC_1();
extern int i2c_handler();
void mpr121_init(int address, I2C *i2c_mem);

extern void i2c_enable(I2C *i2c_mem);
extern void i2c_disable(I2C *i2c_mem);
extern void enable_int_RX(I2C *i2c_mem);
extern void disable_int_RX(I2C *i2c_mem);
extern void enable_int_TX(I2C *i2c_mem);
extern void disable_int_TX(I2C *i2c_mem);
extern void enable_int_DONE(I2C *i2c_mem);
extern void disable_int_DONE(I2C *i2c_mem);
extern void start_transfer(I2C *i2c_mem);
extern void clear_FIFO_data(I2C *i2c_mem);
extern void set_read_transfer(I2C *i2c_mem);
extern void set_write_transfer(I2C *i2c_mem);
extern int get_transfer_type(I2C *i2c_mem); 

/**
 * Status register
*/

extern int is_CLKT(I2C *i2c_mem);
extern void reset_CLKT(I2C *i2c_mem);
extern int is_ERR(I2C *i2c_mem);
extern void reset_ERR(I2C *i2c_mem);
extern int is_RX_FIFO_full(I2C *i2c_mem);
extern int is_TX_FIFO_empty(I2C *i2c_mem);
extern int does_RX_contain_DATA(I2C *i2c_mem);
extern int does_TX_accept_DATA(I2C *i2c_mem);
extern int does_RX_need_reading(I2C *i2c_mem);
extern int does_TX_need_writing(I2C *i2c_mem);
extern int is_transfer_done(I2C *i2c_mem);
void reset_transfer_done(I2C *i2c_mem);
extern int is_transfer_active(I2C *i2c_mem);

/**
 * Data length register
 */

extern void set_data_length(int num,I2C *i2c_mem);
extern int get_data_length(I2C *i2c_mem);

/**
 * Slave address register
 */

extern void set_slave_address(int address,I2C *i2c_mem);
extern int get_slave_address(I2C *i2c_mem);

/**
 * Data FIFO register
 */

extern int get_FIFO_data(I2C *i2c_mem);
extern void set_FIFO_data(int data,I2C *i2c_mem);

/**
 * Clock divider register
 */

extern void set_clock_divider(int divisor,I2C *i2c_mem);
extern int get_clock_divider(I2C *i2c_mem);

/**
 * Data delay register
 */

extern void set_falling_edge_delay(int delay,I2C *i2c_mem);
extern int get_falling_edge_delay(I2C *i2c_mem);
extern void set_rising_edge_delay(int delay,I2C *i2c_mem);
extern int get_rising_edge_delay(I2C *i2c_mem);

/**
 * Clock stretch timeout register
 */

extern void set_clock_stretch_timeout(int timeout,I2C *i2c_mem);
extern int get_clock_stretch_timeout(I2C *i2c_mem);

/**
 *
 */
void reset_status_register(I2C *i2c_mem);


int read_byte_from_FIFO(I2C *i2c_mem);
void write_byte_to_FIFO(u8, I2C *i2c_mem);
int read_status_reg(I2C *i2c_mem);

/**
 *
 */

void read_bytes2(I2C *i2c_mem, unsigned int, unsigned int *, unsigned int);
void write_bytes(unsigned int, unsigned int *, unsigned int, I2C *i2c_mem);

void read_bytes_init(I2C *i2c_mem, unsigned int, unsigned int *, unsigned int *, unsigned int);
// void read_8bit_reg(u8 reg_addr, I2C* i2c_mem, unsigned int i2c_address);
// void write_reg_addr(unsigned int address, u8 reg_addr, I2C *i2c_mem);*/

#endif
