#include "rpi-spi.h"
#include "defines.h"
#include "rpi-uart.h"

/*

	NOTA: lees 10.6.2 van de BCM2835 arm om te begrijpen

*/
void spi_init() {
    spi_reg = (unsigned int*) SPI_BASE;
}

// zet alle SPI lijnen naar actief low
void spi_polarity() { 
	spi_reg[SPI_CONTROL] |= (0 << 21);
	spi_reg[SPI_CONTROL] |= (0 << 3);
	spi_reg[SPI_CONTROL] |= (0 << 6);
}
// enkel lezen van de peripherals bij bidirectional mode
void spi_read_enable() {
	spi_reg[SPI_CONTROL] |= (1 << 12);
}
// Selecteer de juiste SPI 
void spi_chip_select() {
	spi_reg[SPI_CONTROL] |= (0 << 1);
	spi_reg[SPI_CONTROL] |= (0 << 0);
}
// Kijkt of er minstens 1 byte data in de RX FIFO is
int spi_does_RX_contain_DATA() {
    return (spi_reg[SPI_CONTROL] >> 17) & 1;
}
//RX moet gelezen worden las hij vol is, cleared wanneer transfer gestopt is of er data gelezen wordt uit de RX FIFO 
void enable_RX_needs_reading() {
	spi_reg[SPI_CONTROL] |= (1 << 19);
}
// interrupt wanneer RX data heeft, werkt enkel als RX needs reading enabled is
void spi_enable_int_RX() {
	spi_reg[SPI_CONTROL] |= (1 << 10);
}
// staat aan wanneer er geen data meer naar de RX FIFO gaat of wanneer transfer gestopt is
void spi_transfer_done() {
	spi_reg[SPI_CONTROL] |= (1 << 16);
}
// interrupt wanneer transfer klaar is, werkt enkel als transfer done aan staat
void spi_enable_int_done() {
	spi_reg[SPI_CONTROL] |= (1 << 9);	
}

void spi_start_transfer() {
	spi_reg[SPI_CONTROL] |= (1 << 7);
}
// Stopt de transfer, dit cleared ook de RX FIFO needs reading en transfer done registers
void spi_stop_transfer() {
	spi_reg[SPI_CONTROL] |= (0 << 7);
}

// Cleared de TX en RX FIFO
void spi_clear_FIFO() {
	spi_reg[SPI_CONTROL] |= (1 << 4); //TX
	spi_reg[SPI_CONTROL] |= (1 << 5); //FX
}
// leest 32 bits uit de RX FIFO
int spi_get_RX_data() {
	spi_reg[SPI_TX_RX_FIFO] << 32;
}