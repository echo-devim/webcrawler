#include "webcrawler.h"

char* substring(char* str, char* pattern){
	int i=0;
	int j;
	while(str[i]!='\0'){
		j=0;
		int success=1;
		while(pattern[j]!='\0') {
			if ((pattern[j] == str[i])||(pattern[j]=='*')) {
				j++;
				i++;
			}else{
				success=0;
				j=0;
				break;
			}
		}
		if (success){
			return &str[i-j];
		}
		i++;
	}
	return NULL;
}

char* findLink(char *html, char *key, char *link) {
	char *start = substring(html, key);
	if (start==NULL)
		return NULL;
	else
		start+=strlen(key);
	int i=0;
	while((*start!='"') && (*start!='\'')){
		link[i] = *start;
		i++;
		start++;
	}
	link[i]='\0';
	//Remove href="javascript:...";
	if ((link[0]=='j')&&(link[1]=='a')&&(link[2]=='v')&&(link[3]=='a')&&(link[4]=='s')&&(link[10]==':'))
		return findLink(start,key,link);
	return start;
}

void findAllLinks(char *html){
	char keys[KEY_ARRAY_SIZE][KEY_SIZE] = {"href=*", "href*=**", "src=*","src*=**"};
	char *link = (char*)malloc(sizeof(char)*MAX_URL_LENGTH);
	int i;
	for (i=0; i<KEY_ARRAY_SIZE;i++){
		char *start = html;
		do{
			start = findLink(start, keys[i], link);
			sanitizeUrl(&link);
			if ((start!=NULL)&&(*link!='\0')&&(checkPage(link))){
				printf("%s\n", link);
				crawlPage(link);
			}
		}while(start!=NULL);
	}
}

void sanitizeUrl(char **url){
	char *new_url;
	char *host_name = host;
	if (*host_name == 'w')
		host_name+=4;
	char *tmp_url = substring(*url, host_name);
	if (tmp_url!=NULL) {
		*url = tmp_url+strlen(host_name);
	}
	if ((**url == '#')||(substring(*url, "://")!=NULL)){
		**url = '\0';
		return;
	}
	tmp_url = substring(*url, "?");
	if (tmp_url!=NULL)
		*tmp_url='\0';
	tmp_url = substring(*url, "#");
	if (tmp_url!=NULL)
		*tmp_url='\0';
	if ((substring(*url, "://")==NULL)&&(**url != '/')){
		new_url = (char*)malloc(MAX_URL_LENGTH);
		new_url[0] ='/';
		new_url[1] = '\0';
		strcat(new_url, *url);
		free(*url);
		*url = new_url;
	}
}

//This function check if a page was already analyzed
//return true if the page is new.
int checkPage(char *page){
	int i;
	for(i=0;i<pt_stack_page;i++){
		if (strcmp(stack_page[i],page)==0) return 0;
	}
	return 1;
}

int checkPageType(char *page){
	int size=8;
	char *types[] = {"php", "css", "js", "html", "htm", "txt", "asp", "jsp"};
	char *type = substring(page+(strlen(page)-5), "."); //Get last '.'
	if (type==NULL)
		return 1;
	type+=1;
	int i;
	for(i=0;i<8;i++){
		if (strcmp(type, types[i]) == 0)
			return 1;
	}
	return 0;
}

void crawlPage(char *page_url){
	if (checkPageType(page_url)) {
		strcpy(stack_page[pt_stack_page],page_url);
		pt_stack_page++;
		char *request = createRequest(http_type, page_url, host, user_agent, cookie);
		sendRequest(ip, port, request, response);
		findAllLinks(response);
	}
}

int main(int argc, char** argv) {
	if (argc < 8) {
		printf(
		" _    _      _     _____                    _   \n"
		"| |  | |    | |   /  __ \\                  | |  \n"
		"| |  | | ___| |__ | /  \\/_ __ __ ___      _| | ___ _ __ \n"
		"| |/\\| |/ _ \\ '_ \\| |   | '__/ _` \\ \\ /\\ / / |/ _ \\ '__|\n"
		"\\  /\\  /  __/ |_) | \\__/\\ | | (_| |\\ V  V /| |  __/ |   \n"
 		" \\/  \\/ \\___|_.__/ \\____/_|  \\__,_| \\_/\\_/ |_|\\___|_|             v0.1-Alpha  \n\n"
 		"Usage:\n"
 		"$ %s HTTP_TYPE start_page ip port host-name user-agent cookies\n\n"
 		"Example:\n"
 		"$ %s GET /index.php 111.222.333.444 80 www.domain.com Firefox cookie_key=null_value\n\n"
		, argv[0], argv[0]);
	}else{
		strcpy(http_type,argv[1]);
		strcpy(ip,argv[3]);
		port = atoi(argv[4]);
		strcpy(host,argv[5]);
		strcpy(user_agent,argv[6]);
		strcpy(cookie,argv[7]);
		response = (char*)malloc(sizeof(char)*RESPONSE_BUFFER_SIZE);
		crawlPage(argv[2]);
		free(response);
	}
	return 0;
}

