#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

int main()
{
    const int PORT = 1024;

    // socket
    int serv_sock = socket(AF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1){
        printf("socket create fail!\n");
        exit(0);
    }
    else{
        printf("socket create success!\n");
    }
    
    // addr init
    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(bzero));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(PORT);

    // bind
    if(bind(serv_sock, (struct sockaddr_in*)&serv_addr, sizeof(serv_addr)) != 0){
        printf("sock bind fail!\n");
        exit(0);
    }
    else{
        printf("sock bind success!\n");
    }

    // listen
    if(listen(serv_sock, 5) != 0){
        printf("listen fail!\n");
        exit(0);
    }
    else{
        printf("listening!\n");
    }

    // accept

    
}