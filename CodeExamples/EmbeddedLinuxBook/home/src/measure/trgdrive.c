/*

        file: trgdrive.c

    Target driver functions for "thermostat" example running on the E.L.L.K target
    board. Actual "temperature" is provided by a pot on the board. 
*/

#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <s3c2410-regs.h>

#include "driver.h"

static S3C2410P_GPIO GPIOp;
static S3C2410P_ADC ADCp;
static int fd;

int initAD (void)
/*
    Get the ADC memory space and initialize it.
*/
{
// Map the ADC register space
    ADCp = mmap (0, sizeof (S3C2410_ADC), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0x58000000);
    close (fd);
    if (ADCp == (void *) -1)
        return -1;

    ADCp->TSC = 0;			// normal mode
    ADCp->CON = PRSCEN | PRSCVL(0xff);

    return 0;
}

int readAD (unsigned int channel)
{
	write_reg (&ADCp->CON, 0x38, CHSEL(channel));
	set_reg (&ADCp->CON, START);
	while ((ADCp->DAT0 & ECFLG) == 0);

    return ADCp->DAT0 & 0x3ff;
}

void closeAD (void)
/*
    Unmap the connection to the ADC
*/
{
    munmap (ADCp, sizeof (S3C2410_ADC));    // don't worry about the return value
}

int initDigIO (void)
/*
    Sets up GPB for the LEDs and GPG for the switches.  This should be called
    before initAD(), which needs the file descriptor for /dev/mem opened here.
    Return value is -1 if we can't open /dev/mem, or mmap() fails.
*/
{
    if ((fd = open ("/dev/mem", O_RDWR | O_SYNC)) < 0)
        return -1;
    GPIOp = mmap (0, sizeof (S3C2410_GPIO), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0x56000000);
    if (GPIOp == (void *) -1)
    {
        close (fd);
        return -1;
    }
// set the LED bits for output
    write_reg (&GPIOp->GPBCON, 0x3FC00, GP_BIT(8, POUT) | GP_BIT(7, POUT) | GP_BIT(6, POUT) | GP_BIT(5, POUT));
    set_reg (&GPIOp->GPBDAT, 0x1E0);     // Set initially to "off"
    
// set the switch bits for input
    write_reg (&GPIOp->GPGCON, 0xC0FCC3, GP_BIT(0, PIN) | GP_BIT(3, PIN) | GP_BIT(5, PIN) | GP_BIT(6, PIN) | GP_BIT(7, PIN) | GP_BIT(11, PIN));
    
    return 0;
}

void closeDigIO (void)
/*
    Unmap the system peripheral space and close the /dev/mem file descriptor
*/
{
    munmap (GPIOp, sizeof (S3C2410_GPIO));    // don't worry about the return value
}

void setDigOut (unsigned int bit)
/*
    Set bit mask 'bit' in GPIOB data register.  This actually sets the bit to
    0 so it turns on the LED
*/
{
	bit <<= 5;		// shift over to where the LEDs are
	clear_reg (&GPIOp->GPBDAT, bit);
}

void clearDigOut (unsigned int bit)
/*
    Clear bit mask 'bit' in GPIOB data register.  This actually sets the bit to
    1 so it turns off the LED
*/
{
	bit <<= 5;		// shift over to where the LEDs are
	set_reg (&GPIOp->GPBDAT, bit);
}

void writeDigOut (unsigned int value)
/*
 * This specific instance of writeDigOut is specifically for the LEDs
 * on the mini2440 and so that a 1 turns the LED on.
 */
{
	value = ~value;
	write_reg (&GPIOp->GPBDAT, 0x1E0, value << 5);
}
unsigned int getDigIn (void)
/*
    Reads the GPIOG data register.  It's up to the application to decide which
    bits are valid
*/
{
    return GPIOp->GPGDAT;
}
