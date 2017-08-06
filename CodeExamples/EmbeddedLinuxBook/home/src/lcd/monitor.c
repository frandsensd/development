/*

        File: monitor.c

    A Posix thread to monitor console input for parameter changes
    Also includes a function, createThread() called by main() in
    thermostat.c to create the Pthreads objects.  This version uses
    sockets over a network.

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

#include "../network/thermostat.h"

const char *delims = " \t,=";

pthread_mutex_t paramMutex;
pthread_t monitorT;

int createServer (void)
/*
    Sets up a server socket and waits for a connection.  Returns the
    client socket.  Returns -1 if anything fails
*/
{
    int server_socket, client_socket;
    unsigned int client_len;
    struct sockaddr_in server_addr, client_addr;
    int result;

// Create unnamed socket and give it a "name"
    server_socket = socket (PF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    result = inet_aton (SERVER, &server_addr.sin_addr);
    if (result == 0)
    {
        printf ("inet_aton failed\n");
        return -1;
    }
    server_addr.sin_port = htons (PORT);

// Bind to the socket
    result = bind (server_socket, (struct sockaddr *) &server_addr, sizeof (server_addr));
    if (result != 0)
    {
        perror ("bind");
        return -1;
    }
// Create a client queue
    result = listen (server_socket, 1);
    if (result != 0)
    {
        perror ("listen");
        return -1;
    }
    printf ("Network server running\n");

// Accept a connection
    client_len = sizeof (client_addr);
    client_socket = accept (server_socket, (struct sockaddr *) &client_addr, &client_len);
    printf ("Connection established to %s\n", inet_ntoa (client_addr.sin_addr));
    return client_socket;
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
    unsigned int value;
    int client_socket;

    client_socket = createServer();
    if (client_socket < 0)
    {
        printf ("Couldn't initialize server\n");
        exit (1);
    }

    while (1)
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
    return NULL;
}

#define CHECK_ERROR if (error) { \
        printf ("%s\n", strerror (error)); \
        return 1; }

int createThread ()
/*
    Creates the mutex and starts up the monitor thread
*/
{
    int error;
/*
    Create the Posix objects
*/
    error = pthread_mutex_init (&paramMutex, NULL);
    CHECK_ERROR;

    error = pthread_create (&monitorT, NULL, monitor, NULL);
    CHECK_ERROR;

    return 0;
}

void terminateThread (void)
{
	void *ret_val;
	
	pthread_cancel (monitorT);
	pthread_join (monitorT, &ret_val);
}
