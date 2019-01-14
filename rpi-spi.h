#ifndef RPI_I2C_H
#define RPI_I2C_H

static volatile unsigned int* spi_reg;

extern void spi_init();
extern void spi_polarity();
extern void spi_read_enable();
extern void spi_chip_select();
extern int spi_does_RX_contain_DATA();
extern void enable_RX_needs_reading();
extern void spi_enable_int_RX();
extern void spi_transfer_done();
extern void spi_enable_int_done();
extern void spi_start_transfer();
extern void spi_stop_transfer();
extern void spi_clear_FIFO();
extern int spi_get_RX_data();


#endif