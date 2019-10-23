#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

void handlemsg(int sockfd);
int main(int argc, char** argv)
{
    // const static int PORT = 63321;
    int port = 63321;
    if(argc<2)
    {
        char buff[32];
        printf("please input port to bind: ");
        scanf("%s", buff);
        port = atoi(buff);
    }
    
    // socket
    int serv_sock = socket(AF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1){
        printf("server socket create fail!\n");
        exit(0);
    }
    else{
        printf("server socket create success!\n");
    }
    
    // addr init
    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(bzero));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(port);

    // bind
    if(bind(serv_sock, (struct sockaddr_in*)&serv_addr, sizeof(serv_addr)) != 0){
        printf("server sock bind fail!\n");
        exit(0);
    }
    else{
        printf("server sock bind success!\n");
    }

    // listen
    if(listen(serv_sock, 5) != 0){
        printf("server listen fail!\n");
        exit(0);
    }
    else{
        printf("server listening!\n");
    }

    // accept
    struct sockaddr_in clnt_addr;
    int len = sizeof(clnt_addr);
    int sockin = accept(serv_sock, (struct sockaddr*)&clnt_addr, &len);
    if(sockin < 0){
        printf("server accept fail!\n");
        exit(0);
    }
    else{
        printf("server accept success!\n");
    }
    
    handlemsg(sockin);

    // close
    printf("server exit!\n");
    close(serv_sock);

    return 0;
}

void handlemsg(int sockfd)
{
    const static int BUFF_MAX = 256;
	char buff[BUFF_MAX];
	for(;;){
		bzero(buff, sizeof(buff));
		read(sockfd, buff, sizeof(buff));
		//printf("From Server: %s", buff);

        write(sockfd, buff, sizeof(buff));

		if(strncmp(buff, "exit", 4) == 0){
			printf("server exit... \n");
			break;
		}        
	}
}