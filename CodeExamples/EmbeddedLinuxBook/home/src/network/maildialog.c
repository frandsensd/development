/*

        maildialog
        
        D. Abbott   1/19/12
        
        Opens a connection to a SMTP mail server for an interactive dialog
        
        Loosely based on code published in:
        Jones, M. Tim, "TCP/IP Application Layer Protocols for Embedded Systems"
        Copyright (c) 2002 Charles River Media.  All rights reserved.
*/
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int running = 1;

void done (int sig)
/*
    Signal handler to stop the program gracefully if necessary
*/
{
    running = 0;
}

int getResponse (int socket)
/*
    Gets the response from the SMTP server and prints it
    Returns the value returned by read
*/
{
    char text[100];
    int result;
    
    result = read (socket, text, sizeof (text));
	if (result > 0)
	{
		text[result] = 0;
		puts (text);
	}
    return result;
}

int main (int argc, char *argv[])
{
    int client_socket, result;
    struct sockaddr_in servaddr;
    struct hostent *hptr;
    char text[100];


    if (argc < 2)
    {
        printf ("Usage:\n\tmaildialog <mail_server>\n");
        exit (1);
    }
    signal (SIGINT, done);  // set up signal handler
/*
    Set up the client socket
*/
    client_socket = socket (AF_INET, SOCK_STREAM, 0);
    bzero ((void *)&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons (25);     //SMTP port

    hptr = gethostbyname (argv[1]);
    if (hptr == NULL)
    {
        perror ("Mail server name not recognized\n");
        exit (2);       // out of luck
    }
    else
        memcpy (&servaddr.sin_addr, hptr->h_addr_list[0], sizeof(struct in_addr));
 
    result = connect (client_socket, (struct sockaddr *) &servaddr, sizeof(servaddr));
    if (result < 0)
    {
        perror ("Client can't connect\n");
        exit (3);
    }
    printf ("Connection established to %s\n", argv[1]);
    text[0] = 'a';     // make sure text doesn't start with "data"
    
    do
    {       
        if (getResponse (client_socket) < 0)
        {
            running = 0;   // bail out on an error condition
            perror ("Server failed to respond\n");
            break;
        }    
        if (!strncasecmp (text, "data", 4))
        {
            do
            {  // get the message body.  The server doesn't respond
               // again until it sees a line with just a "."
                fgets (text, sizeof (text), stdin);
                write (client_socket, text, strlen (text));
            }
            while (strncmp (text, ".\n", 2));
            getResponse (client_socket);
        }
        else if (!strncasecmp (text, "quit", 4))
        { // we're outta' here.  The server has already disconnected
            running = 0;
            break;
        }
        printf ("Client> ");
        fgets (text, sizeof (text), stdin);
        write (client_socket, text, strlen (text));
    }
    while (running);
    
    close (client_socket);
    printf ("Connection closed\n");
    return 0;
}
