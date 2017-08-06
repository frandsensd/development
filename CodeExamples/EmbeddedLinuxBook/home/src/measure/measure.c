/*

        File: measure.c

    A simple data acquisition program that reads ADC channel 0
    and sends it to stdout.  Reads the pushbuttons and writes
    the button number to the LEDs.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include "driver.h"

extern int errno;

int running = 1;

// The switches are distributed somewhat randomly in GPIO register G
const unsigned int switch_bits[] = {1, 8, 0x20, 0x40, 0x80, 0x800};

void done (int sig)
/*
    Signal handler to stop the program gracefully
*/
{
    running = 0;
}

int main (int argc, char *argv[])
{
    int value;
    unsigned int wait, sample = 0;
    unsigned int switches, i;

    signal (SIGINT, done);  // set up signal handler
    if (argc > 1)           // get wait time
        sscanf (argv[1], "%d", &wait);
    else
        wait = 2;

    if (initDigIO() < 0)
    {
        printf ("Couldn't initialize digital I/O\n");
        exit (1);
    }
    if (initAD() < 0)
    {
        printf ("Couldn't initialize A/D converter\n");
        exit (2);
    }
    while (running)
    {
        sleep (wait);
        value = readAD (0);
        printf ("Sample %d = %d\n", sample, value);
        sample++;
/*
    There are six pushbuttons, but only four LEDs.  So we'll translate a button
    bit into a number between 1 and 6
*/
        switches = ~getDigIn();		// activated button reads 0
        for (i = 0; i < 6; i++)
            if (switches & switch_bits[i])
                writeDigOut (i+1);
    }
    writeDigOut (0);		// turn off the LEDs
    printf ("Goodbye\n");
    closeAD();
    closeDigIO();
    return 0;
}
