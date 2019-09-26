// Write CPP code here 
#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#define MAX 80 
#define PORT 8080 
#define SA struct sockaddr 

int main() 
{
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
	if(connect(clnt_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr) != 0)){
		printf("clnt connect fail!\n");
		exit(0);
	}
	else
		printf("clnt connected!\n");
	
	// reply server
	char buff[64] = {0};
	write(clnt_sock, buff, sizeof(buff));

	// receive from server
	bzero(buff, sizeof(buff));
	read(clnt_sock, buff, sizeof(buff));

	// close
	printf("client exit!\n");
	close(clnt_sock);

	return 0;
} 

