#ifndef http
#define http

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h> //BSD socket style
#include <netinet/in.h> //BSD AF INET and AF INET6
//#include <netdb.h> //Functions to resolve host name
#include <string.h>

#define RESPONSE_BUFFER_SIZE 500000

void sendRequest(char* ip, int port, char* request, char* response);
char* createRequest(char *http_type, char *path, char *host, char *user_agent, char *cookie);

#endif
