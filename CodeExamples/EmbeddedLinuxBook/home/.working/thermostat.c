/*

        File: thermostat.c

    A program to control a "cooler" and limit alarm based on data from
    the pot attached to ADC channel 0. The current reading is sent
    to stdout.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include "driver.h"

unsigned int setpoint, limit, deadband;
extern int errno;

int running = 1;

void done (int sig)
/*
    Signal handler to stop the program gracefully
*/
{
    running = 0;
}

int main (int argc, char *argv[])
{
    unsigned int i = 0, t, alarm;
    enum state_t {OK, HIGH, LIMIT} state = OK;
    unsigned int wait, value;

    signal (SIGINT, done);  // set up signal handler
    if (argc > 1)           // get wait time
        sscanf (argv[1], "%d", &t);
    else
        t = 2;

    if (initDigIO() < 0)
    {
        printf ("Couldn't initialize digital I/O\n");
        exit (2);
    }
    if (initAD() < 0)
    {
        printf ("Couldn't initialize A/D converter\n");
        exit (1);
    }
    setpoint = 54;
    limit = 58;
    deadband = 1;
    wait = t;

    while (running)
    {
        sleep (1);
        if (++t >= wait)
        { /*
			The ADC returns values in the range of 0 to 1023.  We'll
			divide that by 10 to get a reasonable temperature range
		  */
            value = readAD (0)/10;
            printf ("Sample %d = %d\n", i, value);
            i++;
            t = 0;

            switch (state)
            {
                case OK:
                    if (value > setpoint + deadband)
                    {
                        state = HIGH;
                        setDigOut (COOLER);
                    }
                    if (value > limit)
                    {
                        state = LIMIT;
                        alarm = 1;
                        setDigOut (ALARM);
                    }
                    break;

                case HIGH:
                    if (value > limit)
                    {
                        state = LIMIT;
                        alarm = 1;
                        setDigOut (ALARM);
                    }
                    else if (value < setpoint - deadband)
                    {
                        state = OK;
                        clearDigOut (COOLER);
                    }
                    break;

                case LIMIT:
                    if (value < limit)
                    {
                        state = HIGH;
                        alarm = 0;
                        clearDigOut (ALARM);
                    }
                    break;
            }
        }
        if (state == LIMIT)    // blink the alarm indicator
            alarm ? (alarm = 0) : (alarm = 1);
        if (alarm)
            setDigOut (ALARM);
        else
            clearDigOut (ALARM);
    }
    closeAD();
    closeDigIO();
    return 0;
}
