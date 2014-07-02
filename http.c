#include "http.h"

char* createRequest(char *http_type, char *path, char *host, char *user_agent, char *cookie) {
	char *connection = "Connection: keep-alive";
	char *request = (char*)malloc(sizeof(char)*1000);
	sprintf(request, "%s %s HTTP/1.1\r\nHost: %s\r\nUser-Agent: %s\r\nCookie: %s\r\n%s\r\n\r\n",http_type,path, host, user_agent,cookie, connection);
	return request;
}



void sendRequest(char* ip, int port, char* request, char* response) {
    struct sockaddr_in serveraddr;
    int tcpSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (tcpSocket < 0) {
    	printf("Error: failed to open connection\n");
    }
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(port);
    serveraddr.sin_addr.s_addr = inet_addr(ip);
	int result = connect(tcpSocket, (struct sockaddr *) &serveraddr, sizeof(serveraddr));
	if (result<0){
		printf("Error: failed connection attempt\n");
	}
	result = send(tcpSocket, request, strlen(request), 0);
	if (result < 0){
		printf("Error: failed to send data\n");
	}
	int r=52;
	int tot=0;
	char returnedResponse[52];
	do{
		r = read(tcpSocket, returnedResponse, r);
    	if (r>0){
			tot+=r;
			strncat(response, returnedResponse, r);
		}else if(r == -1){
			printf("Error: read socket fail\n");
			exit(1);
		}
    }while(r>0);
    response[tot]='\0';
    close(tcpSocket);
    return;
}
