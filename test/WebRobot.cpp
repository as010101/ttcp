#include <iostream>
#include <string>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <fstream>
#include <algorithm>

//using namespace std;

// split url to hostname & res path
void ParseUrl(const std::string& url, std::string& hostname, std::string& respath)
{
    hostname = url;
    respath = "/";
    char* headstrs[] = {
        "https://", "http://"
    };

    // remove http headers
    for(auto it = std::begin(headstrs); it != std::end(headstrs); it++)
    {
        auto pos = hostname.find(*it);
        if(pos != std::string::npos)
        {
            hostname.replace(pos, strlen(*it), "");
        }
    }

    // split
    auto pos = hostname.find("/");
    if(pos != std::string::npos)
    {
        respath = hostname.substr(pos);
        hostname = hostname.substr(0, pos);
    }
}

std::string GetPageContent(const std::string& url, int port)
{
    std::string hostname, respath, content;
    ParseUrl(url, hostname, respath);

    // get host by name
    struct hostent* he = gethostbyname(hostname.c_str());
    if(he == nullptr)
    {
        std::cout<< "gethostbyname fail!"<< std::endl;
        return content;
    }

    // init sock fd
    int sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if(sockfd == -1)
    {
        std::cout<< "socket init fail!"<< std::endl;
        return content;
    }

    // addr
    struct in_addr* ia = (struct in_addr*)he->h_addr;

    // init sock addr
    struct sockaddr_in sockadr;
    std::memset(&sockadr, 0, sizeof sockadr);

    sockadr.sin_family = AF_INET;
    sockadr.sin_port = htons(port);
    sockadr.sin_addr.s_addr = ia->s_addr;

    // connect
    int rt = connect(sockfd, (struct sockaddr*)&sockadr, sizeof(sockadr));
    if( rt == -1)
    {
        std::cout<< "connect fail to " << hostname << std::endl;
        return content;
    }

    // request header
    std::string reqhead = "GET " + respath + " HTTP/1.1\r\n";
    reqhead += "HOST: " + hostname + "\r\n";
    reqhead += "Connection: Close\r\n\r\n";

    // send
    rt = send(sockfd, reqhead.c_str(), reqhead.size(), 0);
    if( rt == -1)
    {
        std::cout<< "send fail to host: " << hostname << std::endl;
        return content;
    }

    // recv
    struct timeval timeout = {1, 0};
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(struct timeval));

    const int BUFFSIZE = 512;
    int len;
    char buff[BUFFSIZE] = {0};
    while((len = recv(sockfd, buff, BUFFSIZE - 1, 0)) > 0)
    {
        content += buff;
    }
    return content;
}

int main(int argc, char** argv)
{
    std::string urlstr;
    if(argc<2)
    {
        std::cout<< "input url for parsing: ";
        std::getline(std::cin, urlstr);
    }
    else
    {
        urlstr = argv[1];
    }

    // parse url
    std::string hostname, path, savefile;
    ParseUrl(urlstr, hostname, path);

    // save filename
    savefile = hostname + path;
    std::replace(savefile.begin(), savefile.end(), '/', '.');

    // file open
    std::ofstream of;
    of.open(savefile, std::ios::out);
    if(of)
    {
        // fetch url content
        of << GetPageContent(urlstr, 80);
        of.close();
    }
    else
    {
        std::cout<<"fail to create file: "<< savefile << std::endl;
    }
    
    std::cout<< "finished parsing url: " << urlstr << std::endl;
    return 0;
}