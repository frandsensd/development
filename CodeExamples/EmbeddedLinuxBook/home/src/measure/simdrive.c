/*

        file: simdrive.c

    Simulated driver functions for "thermostat" example
*/
#define DRIVER

#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "driver.h"

static shmem_t *p;

int initAD (void)
/*
    Get the shared memory identifier and attach to it
*/
{
    int id;

    if ((id = shmget ((key_t) SHM_KEY, sizeof (shmem_t), 0666)) < 0)
        return -1;
    if ((p = (shmem_t *) shmat (id, (void *) 0, 0)) == (shmem_t *) -1)
        return -1;
    p->leds = 0;
// Get the devices output started
    kill (p->pid, SIGUSR1);

    return 0;
}

int readAD (unsigned int channel)
{
    return p->a2d;
}

void closeAD (void)
/*
    Detach shared memory region
*/
{
    shmdt (p);
}

int initDigIO (void)
/*
    A dummy.  Nothing to do
*/
{
    return 0;
}

void closeDigIO (void)
/*
    Another dummy.
*/
{
}

// Notify the devices process every time a digital output changes
void setDigOut (unsigned int bit)
{
    p->leds |= bit;
    kill (p->pid, SIGUSR1);
}

void clearDigOut (unsigned int bit)
{
    p->leds &= ~bit;
    kill (p->pid, SIGUSR1);
}

void writeDigOut (unsigned int bit)
{
    p->leds = bit;
    kill (p->pid, SIGUSR1);
}

unsigned int getDigIn (void)
{
    return 0;
}
