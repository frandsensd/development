/*
    Embedded SMTP Client Header
*/
struct mailHeader {
	char *subject;
	char *sender;
	char *recipient;
	char *specialHeaders;
	char *contentType;
	char *contents;
};

/*
These string pointers will be declared and filled in by the embedded application
*/
extern char *domain;
extern char *mailServer;

int sendMail (struct mailHeader *);

