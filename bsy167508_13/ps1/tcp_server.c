#include<stdio.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netdb.h>
#include<stdlib.h>
#include<string.h>
#define MAX 1200
#define PORT 43454
#define SA struct sockaddr

void str_split(int sockfd, const char *buff)
{
	char send[200] = "";
	const char dlim1[6] = "://";
	const char dlim2[2] = "/";
	const char dlim3[2] = "?";
	const char dlim4[2] = "#"; 
	//make an array to append headers
	char array[5][20] = {"\nProtocol: ", "\nDomain name: ", "\nFilepath: ", "\nParameter: ", "\nAnchor: "};
	const char *protocol = strstr(buff, dlim1);
	const char *domain = strstr(&buff[3+protocol-buff], dlim2);
	const char *filepath = strstr(&buff[1+domain-buff], dlim3);
	const char *param = strstr(&buff[1+filepath-buff], dlim4);
	//create the string to send
	//printf("\n%s \n%s \n%s \n%s", protocol, domain, filepath, param);
	strcat(send, array[0]);
	strncat(send, buff, protocol-buff);
	strcat(send, array[1]);
	strncat(send, &buff[3+protocol-buff], (domain-protocol)-3);
	strcat(send, array[2]);
	strncat(send, &buff[domain-buff+1], filepath-domain-1);
	strcat(send, array[3]);
	strncat(send, &buff[filepath-buff+1], param-filepath-1);
	strcat(send, array[4]);
	strncat(send, &buff[param-buff+1], sizeof(buff)-(param-buff));
	write(sockfd, send, sizeof(send));
	return ;
}

void func(int sockfd)
{
char buff[MAX];
while(1)
{
	bzero(buff,MAX);
	read(sockfd, buff, sizeof(buff));
	printf("From client: %s\nSending the components :\n", buff);
	str_split(sockfd, buff);
	if(strncmp("exit", buff,4) == 0)
	{
		printf("Server Exit...\n");
		break;
	}
}
}

int main()
{
int sockfd,connfd,len;
struct sockaddr_in servaddr,cli;
sockfd=socket(AF_INET,SOCK_STREAM,0);
if(sockfd==-1)
{
printf("socket creation failed...\n");
exit(0);
}
else
printf("Socket successfully created..\n");
bzero(&servaddr,sizeof(servaddr));
servaddr.sin_family=AF_INET;
servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
servaddr.sin_port=htons(PORT);
if((bind(sockfd,(SA*)&servaddr, sizeof(servaddr)))!=0)
{
printf("socket bind failed...\n");
exit(0);
}
else
printf("Socket successfully binded..\n");
if((listen(sockfd,5))!=0)
{
printf("Listen failed...\n");
exit(0);
}
else
printf("Server listening..\n");
len=sizeof(cli);
connfd=accept(sockfd,(SA *)&cli,&len);
if(connfd<0)
{
printf("server acccept failed...\n");
exit(0);
}
else
printf("server acccept the client...\n");
func(connfd);
close(sockfd);
}
