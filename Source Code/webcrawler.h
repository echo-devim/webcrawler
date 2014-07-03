#ifndef crawler_h
#define crawler_h


#define MAX_URL_LENGTH 100
#define KEY_SIZE 10
#define KEY_ARRAY_SIZE 4
#include "http.h"

char http_type[4];
char ip[15];
int port;
char host[20];
char user_agent[100];
char cookie[200];
char stack_page[10000][MAX_URL_LENGTH];
int pt_stack_page=0;

char* substring(char* str, char* pattern);
char* findLink(char *html, char *key, char *link);
void findAllLinks(char *html);
void crawlPage(char *page_url);
void sanitizeUrl(char **url);
int checkPage(char *page);
int checkPageType(char *page);
char *response;

#endif
