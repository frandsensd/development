/*
	file:	webvars.h

    defines a data structure for dynamic variables in an embedded
    web page
*/

typedef struct {
	char *name;
    char *(*var_function) ();
} webvars_t;

char *parseVariable (char *msgbody, char *var_name);
char *web_var (char *var_name, char *content);
