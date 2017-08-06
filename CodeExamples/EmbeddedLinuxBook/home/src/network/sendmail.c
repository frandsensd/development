/*
    File:   sendmail.c
    D. Abbott   12/11/05

    Simple embedded SMTP Client

    Derived from code published in:
    Jones, M. Tim, "TCP/IP Application Layer Protocols for Embedded Systems"
    Copyright (c) 2002 Charles River Media.  All rights reserved.
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#include "sendmail.h"

int strscan (char *line, char *expect)
/*
    Searches for the status code in an SMTP response and checks it against
    the expected 3-digit reference code.

    Returns 0 on success, -1 on error.
 */
{
    int i;

    for (i = 0; i < strlen(line) - 3; i++)
        if (line[i] == expect[0])
            return strncmp (line, expect, 3);

    return -1;
}

int dialog (int socket, char *command, char *expect)
/*
    Perform a dialog with mailServer.  If the command is present, send
    it through the socket to the server.  If the expected response is present,
    check it against the return code from mailServer.

    Returns 0 on success, -1 on error.
 */
{
    int ret, len;
    char line[128];

    if (command)
    {
        len = strlen (command);
        if (write (socket, command, len) != len)
            return -1;
    }
    if (expect)
    {
        ret = read (socket, line, sizeof(line)-1);
        line[ret] = 0;
        return strscan (line, expect);
    }
    return 0;
}

int sendMail (struct mailHeader *mail)
/*
    Sends the email specified in mailHeader to the mailServer

    Returns 0 on success, -1 on error.
*/
{
    int client_socket, result, goodMsg = -1;
    struct sockaddr_in servaddr;
    struct hostent *hptr;
    char line[256];
/*
    Set up the client socket
*/
    client_socket = socket (AF_INET, SOCK_STREAM, 0);
    bzero ((void *)&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(25);

    hptr = gethostbyname (mailServer);
    if (hptr == NULL)
        return -1;      // Can't find mailServer
    else
        memcpy (&servaddr.sin_addr, hptr->h_addr_list[0], sizeof(struct in_addr));

    result = connect (client_socket, (struct sockaddr *)&servaddr, sizeof(servaddr));
    if (result < 0)
        return -1;          // Couldn't connect to the server

    do
    {
    /* Look for initial salutation */
        if (dialog (client_socket, NULL, "220")) break;

    /* Send HELO */
        sprintf (line, "HELO <%s>\n", domain);
        if (dialog (client_socket, line, "250")) break;

    /* Send MAIL FROM */
        sprintf (line, "MAIL FROM: <%s>\n", mail->sender);
        if (dialog (client_socket, line, "250")) break;

    /* Send RCPT TO */
        sprintf( line, "RCPT TO: <%s>\n", mail->recipient);
        if (dialog (client_socket, line, "250")) break;

    /* Send DATA */
        if (dialog (client_socket, "DATA\n", "354")) break;

    /* Send out whatever header fields are present */
        if (mail->sender)
        {
            sprintf (line, "From: %s\n", mail->sender);
            if (dialog (client_socket, line, NULL)) break;
        }
        if (mail->recipient)
        {
            sprintf (line, "To: %s\n", mail->recipient);
            if (dialog (client_socket, line, NULL)) break;
        }
        if (mail->subject)
        {
            sprintf (line, "Subject: %s\n", mail->subject);
            if (dialog (client_socket, line, NULL)) break;
        }
        if (mail->contentType)
        {
            sprintf (line, "Content-Type: %s\n", mail->contentType);
            if (dialog (client_socket, line, NULL)) break;
        }
        if (mail->specialHeaders)
            if (dialog (client_socket, mail->specialHeaders, NULL)) break;

        if (dialog (client_socket, mail->contents, NULL)) break;

    /* Send mail-end*/
        if (dialog (client_socket, "\n.\n", "250")) break;
        if (dialog (client_socket, "QUIT\n", "221")) break;
        goodMsg = 0;        // if we get here, everything is OK
    }
    while (0);

    close (client_socket);
    return goodMsg;
}

