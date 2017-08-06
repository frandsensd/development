/*

        File: webserve.c

    A Posix thread that implements a web server for the thermostat

*/
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "webvars.h"

#define MAX_LINE 80
#define BUFF_SIZE 1024

const char *whitespace = {" \t"};

/* Status line messages */
const char *http_ver = {"HTTP/1.1 "};
const char *notimplemented = {"501 Not Implemented\n\n"};
const char *notfound = {"404 File not found\n\n"};
const char *success = {"200 OK\n"};
//const char *created = {"200 OK\n\n"};
const char *created = {"201 Created\n\n"};

const char *content_types[]={
// Content type strings for response header
	"text/html",
	"image/jpeg",
	"image/gif",
	"application/octet-stream",
	"text/x-hdml",
	"text/plain",
};

const char *content_suffix[] = {
//  Identifies content type by file suffix
	"htm", "html",
    "jpg", "jpeg",
    "gif",
    "class", "jar",
    "hdml",
    NULL};
const int content_map[] = {
//  Maps the content type suffix to an index into content_type strings
	0, 0,
    1, 1,
    2,
    3, 3,
    4};

char text[BUFF_SIZE];
pthread_mutex_t paramMutex;
pthread_t webmonitorT;

void responseHeader (int fd, int ct)
/*
	Sends the successful response header back to the client
*/
{
  	char line[MAX_LINE+1];

  	write (fd, success, strlen (success));

  	sprintf (line, "Server: webserve\n"); 
  	write (fd, line, strlen (line));

  	sprintf (line, "Connection: close\n");
    write (fd, line, strlen (line));

  	sprintf (line, "Content-Type: %s\n\n", content_types[ct]);
  	write (fd, line, strlen(line));
}

char *msgBody (char *buff)
/*
	Return pointer to the body of an HTTP request message
*/
{
//  	static const char *separator = {};
  	char *temp;

  	temp = strstr (buff, "\r\n\r\n");

  	if (temp == NULL) return NULL;
  	else return temp + 4;
}

int getContentfromSuffix (char *filename)
/*
	Returns an integer representing the content type of a
    file based on its suffix
*/
{
	int i, j;
    
    for (i = 0; filename[i]; i++)
    	if (filename[i] == '.')
        	break;
/*
  If the filename has no suffix, assume "octet_stream" type
*/
    if (filename[i++] == 0)
    	return 3;
        
	for (j = 0; content_suffix[j]; j++)
    	if (!strcasecmp (&filename[i], content_suffix[j]))
        	return (content_map[j]);
/*
  Unrecognized suffix, assume "octet_stream"
*/
	return 3;
}

void parseHTML (int socket, int file)
/*
	Parses an HTML file looking for dynamic content, <DATA ...>, tags.
    Write everything but the <DATA> tags to the socket.  When a <DATA>
    tag is found, replace it with the string returned by the corresponding
    dynamic content handler.
*/
{
	const char *delim = {">"};
    
	int n, i, j, nn;
    char *buff, *variable;
    
    do
    {
    	n = read (file, text, BUFF_SIZE);
        if (n > 0)
        {
        	nn = n;
        	buff = text;
        	for (i = 0; i < nn; i++)
            	if ((buff[i] == '<') && !strncasecmp (&buff[i+1], "DATA", 4))
                {
                	write (socket, buff, i-1);		// up to the DATA tag
                    buff += (i + 6);		// point to beginning of variable name
                    variable = strtok (buff, delim);	// and get it
                    j = strlen (variable);			// length of variable name
                    
                    variable = web_var (variable, NULL);
                   	write (socket, variable, strlen (variable));
                    buff += j;		// point past the variable name

                    nn -= i + j + 6;
                    i = -1;		// for loop will increment it to 0
                }
            write (socket, buff, nn);		// write to end of buffer
        }
    }
    while (n > 0);
}

void doPOSTmethod (int socket)
/*
    Get the function name from the POST header and call web_var to execute
    it, passing in a pointer to the message body.
*/
{
	char *function;

	function = strtok (NULL, whitespace);
    if (function[0] == '/')		//skip a leading "/"
    	function++;
// Search for the message body starting beyond the token we just extracted
    if (web_var (function, msgBody (function + strlen (function) + 1)))
    	write (socket, created, strlen(created));
    else
    	write (socket, notfound, strlen(notfound));
}
                    
void doGETmethod (int socket)
/*
	Get the filename and write it out to the socket.  If it's an HTML file
    we have to parse it for dyanmic data tags
*/
{
	char *filename;
    int n, ct, file;
    
	filename = strtok (NULL, whitespace);
    if (!strcmp (filename, "/"))
    	strcpy (filename, "index.html");
    if (filename[0] == '/')		// skip a leading '/' in 
    	filename++;				// the filename
            
    if ((file = open (filename, O_RDONLY)) < 0)
    	write (socket, notfound, strlen (notfound));
   else
    {
    	n = getContentfromSuffix (filename);
        ct = content_map[n];
    	responseHeader (socket, ct);
        
        if (ct == 0)
        	parseHTML (socket, file);		//Look for dynamic data tags
        else do
        {
        	n = read (file, text, BUFF_SIZE);
            if (n > 0)
            	write (socket, text, n);
        }
        while (n > 0);
        close (file);
    }
}
                    
int createServer (void)
/*
    Sets up a server socket.  Returns the server socket number for success
    and -1 if anything fails
*/
{
    int server_socket;
    struct sockaddr_in server_addr;
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
    server_addr.sin_port = htons (80);      // The HTTP port

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
    printf ("Web server running\n");
    return server_socket;    
}

void *webmonitor (void *arg)
{
    char *method;
    int server_socket, client_socket, client_len, len;
    struct sockaddr_in client_addr;
    
    if ((server_socket = createServer()) < 0)
    {
        printf ("Couldn't initialize server\n");
        exit (1);
    }
    
    while (1)
    {
     	client_len = sizeof (client_addr);
    	client_socket = accept (server_socket, (struct sockaddr *) &client_addr, &client_len);
        
        if (client_socket > 0)
        {
    	    printf ("Connection established to %s\n", inet_ntoa (client_addr.sin_addr));
    	    len = read (client_socket, text, BUFF_SIZE);
    	    text[len] = 0;
    
    	    write (client_socket, http_ver, strlen (http_ver));
    	    method = strtok (text, whitespace);
    
    	    if (!strcmp (method, "GET"))
        	    doGETmethod (client_socket);
    	    else if (!strcmp (method, "POST"))
    		    doPOSTmethod (client_socket);
		    else
                write (client_socket, notimplemented, strlen(notimplemented));

    	    close (client_socket);
        }
    }
}

#define CHECK_ERROR if (error) { \
        printf ("%s\n", strerror (error)); \
        return 1; }

int createThread ()
/*
    Creates the mutex and starts up both threads
*/
{
    int error;
/*
    Create the Posix objects
*/
    error = pthread_mutex_init (&paramMutex, NULL);
    CHECK_ERROR;
    
    error = pthread_create (&webmonitorT, NULL, webmonitor, NULL);
    CHECK_ERROR;
    
    return 0;
}

void terminateThread (void)
{
	void *ret_val;
	
	pthread_cancel (webmonitorT);
	pthread_join (webmonitorT, &ret_val);
}
