/*
	file:	webvars.c
    
    This is a set of functions linked to a web server that provide
    variable data for a web page
*/
#include <string.h>
#include <stdio.h>
#include <sys/shm.h>
#include <stdlib.h>

#include "webvars.h"
#include "thermostat.h"

// Forward function declarations
static char *cur_temp (char *);
static char *POSThandler (char *);

// Table of dynamic service functions
const webvars_t webvars[] = {
	{"cur_temp", cur_temp},
    {"control", POSThandler}
};
/*******************************************************
				Dynamic Service Functions
*******************************************************/

static char *cur_temp (char *content)
/*
	Return the current temp from shared memory
*/
{
    static char text[20];
    
    sprintf (text, "%d", curtemp);
	return text;
}

static char *POSThandler (char *msg_body)
{
  	printf("\nChanging parameters\n");

  	setpoint = atoi (parseVariable (msg_body, "Setpoint"));
  	limit    = atoi (parseVariable (msg_body, "Limit"));
  	deadband = atoi (parseVariable (msg_body, "Deadband"));
    
    return (char *)1;	//The return value of a POST handler is irrelevant but
    					// must be non-null to make web_var()'s return value valid
}
/************************************************
			Utilities supporting dynamic web data
************************************************/

char *parseVariable (char *msgbody, char *var_name)
/*
	Scans the body of a HTTP POST message looking for 'variable'.
    Returns the string value of 'variable'.
*/
{
	static char var_text[80];
  	int i;

  /* First, find the variable in the message body */
  	msgbody = strstr (msgbody, var_name);

  	if (msgbody == NULL)
    	return NULL;

  /* Skip the variable and the '=' */
  	msgbody += strlen (var_name) + 1;

  /* Finally, copy the value into the value string */
  	for (i = 0; msgbody[i] != '&'; i++)
    	var_text[i] = msgbody[i];

  	var_text[i] = 0;

  return var_text;
}
char *web_var (char * var_name, char *content)
/*
	Looks up a dynamic web variable name, calls the corresponding
    function and returns its string value.  Returns NULL if the
    variable name is not in the web_var table.
*/
{
	int i;
    
    for (i = 0; i < sizeof (webvars)/sizeof (webvars_t); i++)
    	if (!strcmp (var_name, webvars[i].name))
        	return webvars[i].var_function(content);
    return NULL;
}
