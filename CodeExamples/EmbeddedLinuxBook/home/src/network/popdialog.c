/*

        maildialog
        
        D. Abbott   12/8/05
        
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
char resptext[256];

int getResponse (int socket)
/*
    Gets the response from the SMTP server and prints it
    Returns the value returned by read
*/
{
    int result;
    
    result = read (socket, resptext, sizeof (resptext));
    resptext[result] = 0;
    puts (resptext);
    return result;
}

int main (int argc, char *argv[])
{
    int client_socket, result;
    struct sockaddr_in servaddr;
    struct hostent *hptr;
    char text[256];

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
    servaddr.sin_port = htons (110);  //SMTP port

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
        if (!strncasecmp (text, "list", 4) || !strncasecmp (text, "retr", 4))
            while (1)
            {
                char *rptr;
                
                getResponse (client_socket);
        /* Look for terminating sequence */
                if (rptr = strstr (resptext, "\n."))
                    if ((rptr[2] == '\r') || (rptr[2] == '\n'))
                        break;
            }
        else if (!strncasecmp (text, "quit", 4))
        { // we're outta' here.  The server has already disconnected
            running = 0;
            break;
        }
        printf ("Client> ");
        gets (text);
        strcat (text, "\n");
        write (client_socket, text, strlen (text));
    }
    while (running);
    
    close (client_socket);
    printf ("Connection closed\n");
    return 0;
}
