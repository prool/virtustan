/* prool httpd
proolix@gmail.com www.prool.kharkov.org github.com/prool
some socket i/o examples from https://rsdn.ru/article/unix/sockets.xml
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../proollib/proollib.h"

#define PORT 2222
#define BUFLEN 2048

#define INDEX "index.html"

int main()
{
int sock, listener;
struct sockaddr_in addr;
char buf[BUFLEN];
int bytes_read;
int i;
struct stat struktura;
FILE *fp;
char buf2[BUFLEN];

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
	    printf("%s GET\n", ptime());
	    //printf("Input %s", buf);
	    if (!memcmp(buf,"GET ",4))
		{
		if (stat(INDEX, &struktura))
			{// file not found
#define ERR404 "HTTP/1.1 404 Not Found`\n\
Server: proolhttpd\n\
Date: Sat, 27 Feb 2016 21:45:06 GMT\n\
Content-Type: text/html\n\
Content-Length: 144\n\
Connection: close\n\
\n\
<html>\n\
<head><title>404 Not Found</title></head>\n\
<body>\n\
<center><h1>404 Not Found</h1></center>\n\
<hr><center>proolhttpd</center>\n\
</body>\n\
</html>\n"
            	send(sock, ERR404, strlen(ERR404), 0);
			}
		else
			{// file found
#define HTML1 "\
HTTP/1.1 200 OK\n\
Server: proolhttpd/0.0.2\n"
            	send(sock, HTML1, strlen(HTML1), 0);
//Date: Thu, 25 Feb 2016 21:18:12 GMT\n
		sprintf(buf2,"Date: %s\n", ptime());
            	send(sock, buf2, strlen(buf2), 0);
#define HTML15 "Content-Type: text/html\n"
            	send(sock, HTML15, strlen(HTML15), 0);
//Content-Length: 612\n
//Last-Modified: Tue, 26 Jan 2016 15:03:33 GMT\n
		sprintf(buf2,"Content-Length: %li\nLast-Modified: %s\n", struktura.st_size+16, ptime());
            	send(sock, buf2, strlen(buf2), 0);
#define HTML16 "Connection: close\n\
Accept-Ranges: bytes\n\
\n\
<!DOCTYPE html>\n\
\n"
            	send(sock, HTML16, strlen(HTML16), 0);
// file
		fp=fopen(INDEX, "r");
		if (fp)
			{
			while(!feof(fp))
				{
				buf2[0]=0;
				fgets(buf2, BUFLEN, fp);
				if (buf2[0])
					{
            				send(sock, buf2, strlen(buf2), 0);
					}
				}
			}
            	send(sock, "\n", 1, 0);
			}
		close(sock);
		if (0)
			{
/* struct stat {
               dev_t     st_dev;         // ID of device containing file
               ino_t     st_ino;         // inode number
               mode_t    st_mode;        // protection
               nlink_t   st_nlink;       // number of hard links
               uid_t     st_uid;         // user ID of owner
               gid_t     st_gid;         // group ID of owner
               dev_t     st_rdev;        // device ID (if special file)
               off_t     st_size;        // total size, in bytes
               blksize_t st_blksize;     // blocksize for filesystem I/O
               blkcnt_t  st_blocks;      // number of 512B blocks allocated
		}
 */
               printf("st_dev=%li\n",struktura.st_dev);         // ID of device containing file
               printf("st_ino=%li\n",struktura.st_ino);         // inode number
               printf("st_mode=%i\n",struktura.st_mode);        // protection
               printf("st_nlink=%li\n",struktura.st_nlink);       // number of hard links
               printf("st_uid=%i\n",struktura.st_uid);         // user ID of owner
               printf("st_gid=%i\n",struktura.st_gid);         // group ID of owner
               printf("st_rdev=%li\n",struktura.st_rdev);        // device ID (if special file)
               printf("st_size=%li\n",struktura.st_size);        // total size, in bytes
               printf("st_blksize=%li\n",struktura.st_blksize);     // blocksize for filesystem I/O
               printf("st_blocks=%li\n",struktura.st_blocks);      // number of 512B blocks allocated
			}
		}
	    else if (!strcmp(buf,"QUIT\r\n"))
		{
		printf("Quit\n");
		close(sock);
		exit(0);
		}
	    else
		{
		#define UNKN_CMD "Unknown command\r\n"
            	send(sock, UNKN_CMD, strlen(UNKN_CMD), 0);
		printf("%s Unknown command '%s'\n", ptime(), buf);
		for (i=0;i<strlen(buf);i++) printf("%02X ", buf[i]);
		printf("\n");
		close(sock);
		}
        }
close(sock);
    }

    return 0;
}
