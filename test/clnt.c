// Write CPP code here 
#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 

void handlemsg(int sockfd);
int main() 
{
	const static int PORT = 65321;

	// socket
	int clnt_sock = socket(AF_INET, SOCK_STREAM, 0);
	if(clnt_sock == -1){
		printf("client sock create fail!\n");
		exit(0);
	}
	else
	{
		printf("clnt sock created!\n");
	}

	// addr init
	struct sockaddr_in serv_addr;
	bzero(&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serv_addr.sin_port = htons(PORT);

	// connect
	int rt = connect(clnt_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	
	if(rt != 0)
	{
		printf("code: %d, clnt connect fail!\n", rt);
		exit(0);
	}
	else
		printf("code: %d, clnt connected!\n", rt);
	
	handlemsg(clnt_sock);

	close(clnt_sock);

	return 0;
} 

void handlemsg(int sockfd)
{
	const static int BUFF_MAX = 256;
	char buff[BUFF_MAX];
	for(;;){
		bzero(buff, sizeof(buff));
		printf("Say hello to server: ");
		for(int i=0; i<BUFF_MAX && (buff[i] = getchar()) != '\n'; i++);
		write(sockfd, buff, sizeof(buff));

		bzero(buff, sizeof(buff));
		read(sockfd, buff, sizeof(buff));
		printf("From Server: %s", buff);

		if(strncmp(buff, "exit", 4) == 0){
			printf("Client exit... \n");
			break;
		}		
	}
}