/*

        file: trgdrive.c

    Target driver functions for "thermostat" example running on the FriendlyARM target
    board. Actual temperature is provided by the pot attached to ADC channel 0. This
    version uses the Linux misc device drivers
*/

#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include "driver.h"

static int fd_adc, fd_led, fd_button;

int initAD (void)
/*
    Open the /dev/adc device.
*/
{
    fd_adc = open ("/dev/adc", O_RDWR | O_SYNC);
    if (fd_adc < 0)
        return -1;

    return 0;
}

int readAD (unsigned int channel)
{
    char text[10];
    int value = -1, len;

    len = read (fd_adc, text, sizeof (text) - 1);
    if (len > 0)
    {
        text[len] = 0;
        sscanf (text, "%d", &value);
    }
    return value;
}

void closeAD (void)
{
    close (fd_adc);    // don't worry about the return value
}

int initDigIO (void)
/*
    Open the LED and buttons devices
*/
{
    int i;

    if ((fd_led = open ("/dev/leds", O_RDWR | O_SYNC)) < 0)
        return -1;
    if ((fd_button = open ("/dev/buttons", O_RDWR | O_SYNC)) < 0)
        return -1;

// Turn off the LEDs
    for (i = 0; i < 4; i++)
        ioctl (fd_led, 0, i);

    return 0;
}

void closeDigIO (void)
{
    close (fd_led);    // don't worry about the return value
    close (fd_button);
}

void setDigOut (unsigned int bit)
/*
    Scan 'bit' looking for 1s.  Set the corresponding LED
*/
{
    int i;

	for (i = 0; i < 4; i++, bit >>= 1)
        if (bit & 1)
            ioctl (fd_led, 1, i);
}

void clearDigOut (unsigned int bit)
/*
    Scan 'bit' looking for 1s.  Clear the corresponding LED.
*/
{
    int i;

    for (i = 0; i < 4; i++, bit >>= 1)
        if (bit & 1)
            ioctl (fd_led, 0, i);
}

void writeDigOut (unsigned int value)
/*
 * This specific instance of writeDigOut is specifically for the LEDs
 * on the mini2440 and so that a 1 turns the LED on.
 */
{
    int i;

    for (i = 0; i < 4; i++, value >>= 1)
        ioctl (fd_led, value & 1, i);
}

unsigned int getDigIn (void)
/*
    Reads the buttons device.  The result is moved to the physical button
    bits to mimic the behavior of getDigIn() in the measure directory.
*/
{
    unsigned int bits = 0, i;
    static const unsigned int button_bit[] = {1, 8, 0x20, 0x40, 0x80, 0x800};
    char buttons[6];

    read (fd_button, buttons, sizeof (buttons));
    for (i = 0; i < 6; i++)
        if (buttons[i] == '1')
            bits |= button_bit[i];

    return bits;
}
