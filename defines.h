/********************************************************************
Copyright 2010-2017 K.C. Wang, <kwang@eecs.wsu.edu>
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

NOTE: This header file is modified from its original contents to 
	  facilitate the implementation of an interrupt-driven UART
	  driver.
********************************************************************/

// defines.h file

#ifndef DEFINES_H
#define DEFINES_H

//global constants
extern const int UART_TX_pin;
extern const int UART_RX_pin;

extern const int I2C_SDA_BSC_0_pin;
extern const int I2C_SCL_BSC_0_pin;

extern const int I2C_SDA_BSC_1_pin;
extern const int I2C_SCL_BSC_1_pin;

extern const int MPR121_ADR_1;
extern const int MPR121_ADR_2;
extern const int MPR121_ADR_3;
extern const int MPR121_ADR_4;

extern const int I2C_BSC_0_MPR_1_IRQ_pin;
extern const int I2C_BSC_0_MPR_2_IRQ_pin;
extern const int I2C_BSC_0_MPR_3_IRQ_pin;
extern const int I2C_BSC_0_MPR_4_IRQ_pin;
extern const int I2C_BSC_1_MPR_1_IRQ_pin;
extern const int I2C_BSC_1_MPR_2_IRQ_pin;
extern const int I2C_BSC_1_MPR_3_IRQ_pin;
extern const int I2C_BSC_1_MPR_4_IRQ_pin;
// General type definitions
typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned int   u32;

#define I2C_DATALENGTH  2

// GPIO 
#define GPIO_BASE	0x20200000

#define GPIO_GPFSEL0    0
#define GPIO_GPFSEL1    1
#define GPIO_GPFSEL2    2
#define GPIO_GPFSEL3    3
#define GPIO_GPFSEL4    4
#define GPIO_GPFSEL5    5

#define GPIO_GPSET0     7
#define GPIO_GPSET1     8

#define GPIO_GPCLR0     10
#define GPIO_GPCLR1     11

#define GPIO_GPLEV0     13
#define GPIO_GPLEV1     14

#define GPIO_GPEDS0     16
#define GPIO_GPEDS1     17

#define GPIO_GPREN0     19
#define GPIO_GPREN1     20

#define GPIO_GPFEN0     22
#define GPIO_GPFEN1     23

#define GPIO_GPHEN0     25
#define GPIO_GPHEN1     26

#define GPIO_GPLEN0     28
#define GPIO_GPLEN1     29

#define GPIO_GPAREN0    31
#define GPIO_GPAREN1    32

#define GPIO_GPAFEN0    34
#define GPIO_GPAFEN1    35

#define GPIO_GPPUD      37
#define GPIO_GPPUDCLK0  38
#define GPIO_GPPUDCLK1  39

// Peripheral base address
#define PERIPHERAL_BASE 0x20000000UL

// UART
#define UDR   0x00
#define UDS   0x04
#define RXFE  0x10
#define UFR   0x18
#define TXFF  0x20
#define CNTL  0x2C
#define IMSC  0x38
#define MIS   0x40
#define SBUFSIZE 128

// I2C BSC 0
#define I2C_BASE_BSC_0 0x20205000UL
// I2C BSC 1
#define I2C_BASE_BSC_1 0x20804000UL

#define I2C_CONTROL 0
#define I2C_STATUS 1
#define I2C_DLEN 2
#define I2C_SLAVE_ADDRESS 3
#define DATA_FIFO 4
#define CLOCK_DIVIDER 5
#define DATA_DELAY 6
#define CLOCK_STRETCH_TIMEOUT 7

#define I2C_C_I2CEN    (1 << 15)
#define I2C_C_INTR    (1 << 10)
#define I2C_C_INTT    (1 << 9)
#define I2C_C_INTD    (1 << 8)
#define I2C_C_ST    (1 << 7)
#define I2C_C_CLEAR    (1 << 4)
#define I2C_C_READ    1

#define START_READ    I2C_C_I2CEN|I2C_C_ST|I2C_C_CLEAR|I2C_C_READ
#define START_WRITE    I2C_C_I2CEN|I2C_C_ST

#define I2C_S_CLKT    (1 << 9)
#define I2C_S_ERR    (1 << 8)
#define I2C_S_RXF    (1 << 7)
#define I2C_S_TXE    (1 << 6)
#define I2C_S_RXD    (1 << 5)
#define I2C_S_TXD    (1 << 4)
#define I2C_S_RXR    (1 << 3)
#define I2C_S_TXW    (1 << 2)
#define I2C_S_DONE    (1 << 1)
#define I2C_S_TA 1




// SPI
#define SPI_BASE 0x20204000
#define SPI_CONTROL 0
#define SPI_TX_RX_FIFO 1
#define SPI_CLK_DIVIDER 2
#define SPI_DATA_LENGTH 3
#define SPI_LOSSI 4
#define SPI_DREQ 5


// Interrupt register
#define IRQ2 0x85

// IRQ register locations
#define IRQ_BASIC 0x2000B200UL
#define IRQ_PEND1 0x2000B204UL
#define IRQ_PEND2 0x2000B208UL
#define IRQ_FIQ_CONTROL 0x2000B20CUL
#define IRQ_ENABLE1 0x2000B210UL
#define IRQ_ENABLE2 0x2000B214UL
#define IRQ_ENABLE_BASIC 0x2000B218UL
#define IRQ_DISABLE1 0x2000B21CUL
#define IRQ_DISABLE2 0x2000B220UL
#define IRQ_DISABLE_BASIC 0x2000B224UL

/** @brief Bits in the Enable_Basic_IRQs register to enable various interrupts.
 *    See the BCM2835 ARM Peripherals manual, section 7.5 */
#define BASIC_ARM_TIMER_IRQ         (1 << 0)
#define BASIC_ARM_MAILBOX_IRQ       (1 << 1)
#define BASIC_ARM_DOORBELL_0_IRQ    (1 << 2)
#define BASIC_ARM_DOORBELL_1_IRQ    (1 << 3)
#define BASIC_GPU_0_HALTED_IRQ      (1 << 4)
#define BASIC_GPU_1_HALTED_IRQ      (1 << 5)
#define BASIC_ACCESS_ERROR_1_IRQ    (1 << 6)
#define BASIC_ACCESS_ERROR_0_IRQ    (1 << 7)
#define BASIC_PENDING_REGISTER_2_IRQ    (1 << 9)

#define IRQ_2_ARM_I2C_IRQ		(1 << 21)
#define IRQ_2_ARM_SPI_IRQ		(1 << 22)
#define IRQ_2_ARM_PCM_IRQ		(1 << 23)
#define IRQ_2_ARM_UART_IRQ		(1 << 25)

// Colors for UART color printing
#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"



#endif
