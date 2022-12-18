#include <stdint.h>
#include <sys/socket.h> // bind, connect, listen, accept, socket, recv, send
#include <sys/stat.h>   // stat
#include <signal.h>
#include <string.h>     // strlen(), strstr()
#include <stdio.h>
#include <stdlib.h>     // strtol()
#include <ctype.h>
#include <unistd.h>     // read(), write(), close()
#include <fcntl.h>

#include <err.h>        // errx
#include <errno.h>      // stderr

#include <arpa/inet.h>
#include <pthread.h>

#define BUFFER_SIZE 4096                    // set buffer size to 4096
#define CONTENT_LENGTH "Content-Length:"    // set CONTENT_LENGTH
#define HTTP "HTTP/1.1"                     // set HTTP
#define RNRN "\r\n\r\n"                     // set RNRN
#define REQUEST_ID "Request-Id:"            // set request ID


/*struct for variable that will use often*/
struct httpObject {
    char method[5];             // will store the method like PUT, HEAD, GET 
    char filename[28];          // will store filename with all URL will be 19 character or fewer
    char httpversion[9];        // will store the httpversion and its set to HTTP/1.1 for this lab
    size_t content_length;      // will store the size of content length
    size_t request_id;
    size_t lengthP;
    char status_code[30];		// will store the status code; 200, 201, 400, 403, 404, 500. 501
    char buffer[BUFFER_SIZE];	// will store message and to parse
    char* body;                 // message body
    char content[BUFFER_SIZE];  // will store info of content length
    char id[20];       // will store info of the request id
    int fd;
    int log_fd;
};

// struct worker
// {
//     int log_fd;
// }

/*
create server socket
listen socket 
bind the socket
*/
int create_listen_socket(uint16_t port) {
    signal(SIGPIPE, SIG_IGN);
    if (port == 0) {
        return -1;
    }
    struct sockaddr_in addr;
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0) {
        return -2;
    }
    memset(&addr, 0, sizeof addr);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htons(INADDR_ANY);
    addr.sin_port = htons(port);
    //if result of bind is sucessful, then return 0. if not, return -1
    if (bind(listenfd, (struct sockaddr *) &addr, sizeof addr) < 0) {
        return -3;
    }
    if (listen(listenfd, 500) < 0) {
        return -4;
    }
    return listenfd;
}


/*if not vaild return -1, and 19 character or fewer*/
int error_code(char* gg)
{
	if(strlen(gg) <= 19)
    {
		for(int i = 1; gg[i] != '\0'; i++)
        {
			char c = gg[i];
			if(!((c > 64 && c < 91) || (c > 96 && c < 123) || (c >= '0' && c <= '9') || c == '-' || c == '_' || c == '.'))
				return -1;
		}	
	}
	else{
		return -1;
	}
	return 1;
}

void write_log(struct httpObject* message, int log_fd)
{
    char buff[BUFFER_SIZE];
    struct stat filestat;
    stat(message->filename, &filestat);         //get the path and dump the info to filestat

   
    snprintf(buff, BUFFER_SIZE, "%s,/%s,%s,%zu\n", message->method, message->filename, message->status_code, message->request_id);
    // printf("buff: %s\n", buff);
    
    write(log_fd, buff, strlen(buff));
    memset(buff, 0, strlen(buff));
}