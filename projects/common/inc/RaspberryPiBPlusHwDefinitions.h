#pragma once
/* Header file defining the Raspberry Pi B+ J8 Header pins for 
** GPIO, I2C, SPI and Serialport.
** References:
** https://github.com/raspberrypi/documentation/raw/master/hardware/raspberrypi/schematics/Raspberry-Pi-B-Plus-V1.2-Schematics.pdf
** https://github.com/raspberrypi/documentation/blob/master/hardware/raspberrypi/bcm2835/BCM2835-ARM-Peripherals.pdf
*/


/* GPIO definitions */
#define RPI_GPIO02	/*I2C SDA1*/
#define RPI_GPIO03	/*I2C SCL1*/
#define RPI_GPIO04	/*I2C GCLK*/
#define RPI_GPIO05
#define RPI_GPIO06
#define RPI_GPIO07	/*SPI CE1*/
#define RPI_GPIO08	/*SPI CE0*/
#define RPI_GPIO09	/*SPI MISO*/	
#define RPI_GPIO10	/*SPI MOSI*/
#define RPI_GPIO11	/*SPI CLK*/
#define RPI_GPIO12
#define RPI_GPIO13
#define RPI_GPIO14	/*TXD0*/
#define RPI_GPIO15	/*RXD0*/
#define RPI_GPIO16
#define RPI_GPIO17	/*GEN0*/
#define RPI_GPIO18	/*GEN1*/
#define RPI_GPIO19
#define RPI_GPIO20
#define RPI_GPIO21
#define RPI_GPIO22	/*GEN3*/
#define RPI_GPIO23	/*GEN4*/
#define RPI_GPIO24	/*GEN5*/
#define RPI_GPIO25	/*GEN6*/
#define RPI_GPIO26
#define RPI_GPIO27	/*GEN2*/

/* I2C definitions 
**	SDA: Dataline
**	SCL: CLock
**	NOTE: Two I2C ports exists, but SDA0/SCL0 are allocated for 
**	DISPLAY and CAMERA header.
*/
#define	RPI_I2C_SDA1
#define	RPI_I2C_SCL1

/* SPI definitions
**	MOSI: Master Output, Slave Input line
**	MISO: Master Input, Slave Output line
**	CLK: Clock
**	CEx: Chip Enable
*/
#define	RPI_SPI_MOSI
#define	RPI_SPI_MISO	
#define RPI_SPI_CLK
#define RPI_SPI_CE0
#define RPI_SPI_CE1
