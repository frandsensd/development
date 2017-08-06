/*

        File: multimon.c

    A Posix threads-based network server for the thermostat.  This one
    supports multiple clients simultaneously by spawning a new monitor
    thread for each client connection. Also includes functions createThread()
    and terminateThread() called by main() in thermostat.c to create and
    terminate Pthreads objects.

*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "thermostat.h"

const char *delims = " \t,=";

#define NTHREADS    10
typedef enum {IN_USE, FREE, PENDING} used;

pthread_mutex_t paramMutex;
pthread_t serverT, resourceT;

void *monitor (void *arg);
/*
    Declare and allocate an array of meta Pthread objects
*/
typedef struct {
    used flag;
    int socket;
    pthread_t thread;
} meta_pthread_t;

meta_pthread_t threads[NTHREADS];

meta_pthread_t *get_pthread (void)
{
    int i;

    for (i = 0; i < NTHREADS; i++)
        if (threads[i].flag == FREE)
        {
            threads[i].flag = IN_USE;
            return &threads[i];
        }
    return NULL;
}

void *server (void *arg)
/*
    Thread that sets up a server socket and waits for a connection.
    Spawns a new monitor thread to service the connection and goes
    back to wait for another one.
*/
{
    int server_socket, client_socket;
    unsigned int client_len;
    struct sockaddr_in server_addr, client_addr;
    int result, i;
    meta_pthread_t *pthr;

    for (i = 0; i < NTHREADS; i++)
        threads[i].flag = FREE;

// Create unnamed socket and give it a "name"
    server_socket = socket (PF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    result = inet_aton (SERVER, &server_addr.sin_addr);
    if (result == 0)
    {
        printf ("inet_aton failed\n");
        return NULL;
    }
    server_addr.sin_port = htons (PORT);

// Bind to the socket
    result = bind (server_socket, (struct sockaddr *) &server_addr, sizeof (server_addr));
    if (result != 0)
    {
        perror ("bind");
        return NULL;
    }
// Create a client queue
    result = listen (server_socket, 1);
    if (result != 0)
    {
        perror ("listen");
        return NULL;
    }
    printf ("Network server running\n");

// Accept connections
    while (1)
    {
        client_len = sizeof (client_addr);
        client_socket = accept (server_socket, (struct sockaddr *) &client_addr, &client_len);
        printf ("Connection established to %s\n", inet_ntoa (client_addr.sin_addr));

        if ((pthr = get_pthread()))
        {
            pthr->socket = client_socket;
            pthread_create (&pthr->thread, NULL, monitor, pthr);
        }
        else
        {
            char errmsg[22];

            sprintf (errmsg, "Out of pthread_ts\n");
            write (client_socket, errmsg, strlen (errmsg));
            close (client_socket);
        }
    }
}

void cleanup_monitors (void *arg)
/*
    Cancels and joins any active monitor threads
*/
{
    void *ret_val;
    int i;

    for (i = 0; i < NTHREADS; i++)
    	if (threads[i].flag == IN_USE)
    	{
    		pthread_cancel (threads[i].thread);
    		pthread_join (threads[i].thread, &ret_val);
    	}
}
void *resource (void *arg)
/*
    Wakes up periodically to scan the array of meta pthreads for
    any that have terminated.
*/
{
    void *ret_val;
    int i;

    pthread_cleanup_push (cleanup_monitors, NULL);
    while (1)
    {
        sleep (10);
        for (i=0; i < NTHREADS; i++)
        	if (threads[i].flag == PENDING)
        	{
        		pthread_join (threads[i].thread, &ret_val);
        		threads[i].flag = FREE;
        	}
    }
// This line should never be called, but is needed to
// close a bracket opened by pthread_cleanup_push()
    pthread_cleanup_pop (0);
}

void sendOK (int fd)
/*
    Reply to a parameter setting command
*/
{
    write (fd, "OK\n", 3);
}

void *monitor (void *arg)
{
    char text[80], text_out[10], *cmd, *valuep;
    unsigned int value, running = 1;
    meta_pthread_t *pthr = (meta_pthread_t *) arg;
    int client_socket = pthr->socket;

    while (running)
    {
        read (client_socket, text, sizeof (text));
        cmd = strtok (text, delims);

        while (cmd)
        {
        // This test is necessary because the '?' command doesn't need a
        // value and atoi() will choke if you pass a NULL pointer
            if ((valuep = strtok (NULL, delims)))
                value = atoi (valuep);

            pthread_mutex_lock (&paramMutex);   //get exclusive access to parameters
            switch (*cmd)
            {
                case 's':
                    setpoint = value;
                    sendOK (client_socket);
                    break;
                case 'l':
                    limit = value;
                    sendOK (client_socket);
                    break;
                case 'd':
                    deadband = value;
                    sendOK (client_socket);
                    break;
                case 'q':
                    running = 0;
                    break;
                case '?':
                    sprintf (text_out, "%d\n", curtemp);
                    write (client_socket, text_out, strlen (text_out));
                    break;
                default:
                    break;
            }
            pthread_mutex_unlock (&paramMutex);     // release the parameters

            cmd = strtok (NULL, delims);
        }
    }
    write (client_socket, "Closing connection\n", 20);
    close (client_socket);
    pthr->flag = PENDING;
    return NULL;
}

#define CHECK_ERROR if (error) { \
        printf ("%s\n", strerror (error)); \
        return 1; }

int createThread ()
/*
    Creates the mutex and starts up the server and resource threads
*/
{
    int error;
/*
    Create the Posix objects
*/
    error = pthread_mutex_init (&paramMutex, NULL);
    CHECK_ERROR;

    error = pthread_create (&serverT, NULL, server, NULL);
    CHECK_ERROR;
    error = pthread_create (&resourceT, NULL, resource, NULL);
    CHECK_ERROR;

    return 0;
}

void terminateThread (void)
{
	void *ret_val;

	pthread_cancel (serverT);
	pthread_join (serverT, &ret_val);
	pthread_cancel (resourceT);
	pthread_join (resourceT, &ret_val);
}
