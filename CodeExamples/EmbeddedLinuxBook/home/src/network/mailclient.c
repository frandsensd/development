
#include <stddef.h>
#include <stdio.h>
#include "sendmail.h"

char *mailServer = "mail.cybermesa.com";
char *domain = "mydomain.com";

int main (void)
{
    struct mailHeader mail;
    
    mail.sender ="doug";
    mail.recipient = "doug@intellimetrix.us";
    mail.contentType = "text";
    mail.subject = "This is a test";
    mail.specialHeaders = NULL;
    mail.contents = "Another test of a embedded email\nProblem is, how to handle a multiple line message?\n";
        
   if (sendMail (&mail))
	   printf ("sendMail failed\n");
    return 0;
}
