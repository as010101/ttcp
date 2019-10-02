#include <iostream>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
using namespace std;
int main(int argc, char** argv)
{
    struct hostent* hp;
    char* hostname = "www.taobao.com";
    if((hp = gethostbyname(hostname)) == nullptr)
    {
        cout<<"gethostbyname fail: "<<hostname<<endl;
        exit(0);
    }

    if(hp->h_addrtype != AF_INET || hp->h_addrtype != AF_INET6)
    {
        cout<<"UNKNOWN addrtype: "<<hp->h_addrtype<<endl;
    }

    cout<<"hostname:    "<<hostname<<endl;
    cout<<"h_name:  "<<hp->h_name<<endl;
    
    // cout<<"h_alias_0: "<<hp->h_aliases[0]<<endl;
    for(char** aap = hp->h_aliases; *aap!=nullptr; aap++)
    {
        cout<<"hp->haliases:    "<<*aap<<endl;
    }

    cout<<"h_addr_type: "<<hp->h_addrtype<<endl;
    cout<<"AF_INET: "<<AF_INET<<"   "<<"AF_INET6: "<<AF_INET6<<endl;
    cout<<"h_length:    "<<hp->h_length<<endl;

    char buff[32] = {0};
    for(char** aap = hp->h_addr_list; *aap!=nullptr; aap++)
    {
        cout<< inet_ntop(hp->h_addrtype, *aap, buff, sizeof(buff))<<endl;
    }
    return 0;
}