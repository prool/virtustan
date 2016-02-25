// from https://rsdn.ru/article/unix/sockets.xml

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../proollib/proollib.h"

#define PORT 2222
#define BUFLEN 2048

int main()
{
    int sock, listener;
    struct sockaddr_in addr;
    char buf[BUFLEN];
    int bytes_read;
    int i;

    printf("%s Test echoserver started\n", ptime());

    listener = socket(AF_INET, SOCK_STREAM, 0);
    if(listener < 0)
    {
        perror("socket");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("echoserver bind");
        exit(2);
    }

    listen(listener, 1);

    while(1)
    {
        sock = accept(listener, NULL, NULL);
        if(sock < 0)
        {
            perror("accept");
            exit(3);
        }

        while(1)
        {
	    for (i=0;i<BUFLEN;i++) buf[i]=0;
            bytes_read = recv(sock, buf, BUFLEN, 0);
            if(bytes_read <= 0) break;
	    printf("Input %s", buf);
	    if (!memcmp(buf,"GET ",4))
		{
		//printf("GET CMD\n");

#define HTML1 "\
HTTP/1.1 200 OK\n\
Server: prool-test-server/0.0.1\n\
Date: Thu, 25 Feb 2016 21:08:12 GMT\n\
Content-Type: text/html\n\
Content-Length: 612\n\
Last-Modified: Tue, 26 Jan 2016 15:03:33 GMT\n\
Connection: close\n\
Accept-Ranges: bytes\n\
\n\
<!DOCTYPE html>\n\
\n\
<html>\n\
<head>\n\
<title>First page</title>\n\
</head>\n\
<body>\n\
First <b>HTML</b> page v.2\n\
</body>\n\
</html>\n\n"
            	send(sock, HTML1, strlen(HTML1), 0);
		close(sock);
		}
	    else
		{
		//printf("NO GET CMD\n");
		#define UNKN_CMD "Unknown command\r\n"
            	send(sock, UNKN_CMD, strlen(UNKN_CMD), 0);
		}
            //send(sock, buf, bytes_read, 0);
        }
close(sock);
    }

    return 0;
}
