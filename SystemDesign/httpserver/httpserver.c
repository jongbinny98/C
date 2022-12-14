#include "httpserver.h"                     // header 

#define BUFFER_SIZE 4096                    // set buffer size to 4096
#define CONTENT_LENGTH "Content-Length:"    // set CONTENT_LENGTH
#define HTTP "HTTP/1.1"                     // set HTTP
#define RNRN "\r\n\r\n"                     // set RNRN

/*struct for variable that will use often*/
struct httpObject {
    char method[5];             // will store the method like PUT, HEAD, GET 
    char filename[28];          // will store filename with all URL will be 19 character or fewer
    char httpversion[9];        // will store the httpversion and its set to HTTP/1.1 for this lab
    size_t content_length;      // will store the size of content length
    size_t lengthP;
    char status_code[30];		// will store the status code; 200, 201, 400, 403, 404, 500. 501
    char buffer[BUFFER_SIZE];	// will store message and to parse
    char* body;                 // message body
    char content[BUFFER_SIZE];  // will store info of content length
    int fd;
};

/* given startercode
will create socket and bind and listen to connect with the client.
if it accept the connection, it will start sending and receiving the message.
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

/* recv_process_request()
it will receive the HTTP request from the client and process it depending on the method.
*/
void recv_process_request(int fd, struct httpObject* message)
{
    size_t size = recv(fd, message->buffer, BUFFER_SIZE, 0);
    message->buffer[size] = 0;

    sscanf(message->buffer, "%s /%s %s\r\n%s %zu\r\n\r\n%s", 
    message->method, message->filename, message->httpversion, message->content, &message->content_length, message->body);

    message->lengthP = message->content_length; //used for PUT
    // errx(1, "%zu", message->lengthP);
    // message->body = strstr(message->buffer, RNRN);

    if(strcmp(message->httpversion, HTTP) != 0 || (error_code(message->filename) == -1)) //when a request is ill-formatted
    {
        strcpy(message->status_code, "400");
        message->content_length = 12;
        return;
    }
    
    // // errx(1, "%s", message->content);
    // if (strcmp(message->content, CONTENT_LENGTH) != 0)
    // {
    //     strcpy(message->status_code, "400");
    //     message->content_length = 12;
    //     return;
    // }
    
    /* need size of the file in bytes */
    struct stat filestat;
    stat(message->filename, &filestat);         //get the path and dump the info to filestat
    int contentLength = filestat.st_size;    //need to manually get the size of file for GET, HEAD
    int if_file = S_ISREG(filestat.st_mode);

    if (strcmp(message->method, "GET") == 0 || strcmp(message->method, "HEAD") == 0)
    {
        if(access(message->filename, F_OK) < 0)         // the URL file does not exist.
        {
            strcpy(message->status_code, "404");
            message->content_length = 10;
        }
        else if (if_file == 0 || access(message->filename, R_OK) < 0)   // no read permission
        {
            strcpy(message->status_code, "403");
        }
        else
        {
            strcpy(message->status_code, "200");
            message->content_length = contentLength;
        }
        if(contentLength < 0)
        {
            strcpy(message->status_code, "500");
        }
    }
    else if (strcmp(message->method, "PUT") == 0)
    {
        if(access(message->filename, F_OK) < 0)     // the URL file does not exist
        {
            strcpy(message->status_code, "201");
            message->content_length = 8;
        }
        else if (if_file == 0 || access(message->filename, W_OK) < 0) // no write permission
        {
            strcpy(message->status_code, "403");
        }
        else
        {
            strcpy(message->status_code, "200");
            message->content_length = 3;
        }
    }
    else
    {
        strcpy(message->status_code, "501");
    }
}
/*server_response()
server will respond back to client deoending on their request.
*/
void server_response(int fd, struct httpObject* message)
{
    char response[BUFFER_SIZE];
    char* status_phrase;

    if (strcmp(message->status_code, "200") == 0) status_phrase = "OK";
    else if (strcmp(message->status_code, "201") == 0) status_phrase = "Created";
    else if (strcmp(message->status_code, "400") == 0) status_phrase = "Bad Request";
    else if (strcmp(message->status_code, "403") == 0) status_phrase = "Forbidden";
    else if (strcmp(message->status_code, "404") == 0) status_phrase = "Not Found";
    else if (strcmp(message->status_code, "500") == 0) status_phrase = "Internal Server Error";
    else if (strcmp(message->status_code, "501") == 0) status_phrase = "Not Implemented"; 

    // response back to the client for request "GET"
    if (strcmp(message->method, "GET") == 0)
    {
        if (strcmp(message->status_code, "200") == 0)
        {
            snprintf(response, BUFFER_SIZE, "%s %s %s\r\nContent-Length: %zu\r\n\r\n",              // store response 
            message->httpversion, message->status_code, status_phrase, message->content_length);
            send(fd, response, strlen(response), 0);    // send reponse to fd            
            message->fd = open(message->filename, O_RDONLY);    //open read only
            while(1) 
            {   
                size_t size = read(message->fd, message->buffer, BUFFER_SIZE); // read message->fd and store to buffer
                if (size == 0)
                    break;
                send(fd, message->buffer, size, 0);     // send buffer to the fd
            }
            close(message->fd);
        }
        else 
        {
            char strG[100];
            int size = snprintf(strG, 100, "%s\n", status_phrase);
            snprintf(response, BUFFER_SIZE, "%s %s %s\r\nContent-Length: %u\r\n\r\n",              // store response 
            message->httpversion, message->status_code, status_phrase, size);

            // errx(1, "%s", message->status_code);
            send(fd, response, strlen(response), 0);    // send reponse to fd 
            send(fd, strG, size, 0); 
        }
    }
    else if(strcmp(message->method, "HEAD") == 0)
    {
        if (strcmp(message->status_code, "200") == 0)
        {
            snprintf(response, BUFFER_SIZE, "%s %s %s\r\nContent-Length: %zu\r\n\r\n",              // store response 
            message->httpversion, message->status_code, status_phrase, message->content_length);
            send(fd, response, strlen(response), 0);    // send reponse to fd
        }
        else
        {
            char strH[100];
            int size = snprintf(strH, 100, "%s\n", status_phrase);
            snprintf(response, BUFFER_SIZE, "%s %s %s\r\nContent-Length: %u\r\n\r\n",              // store response 
            message->httpversion, message->status_code, status_phrase, size);
            send(fd, response, strlen(response), 0);    // send reponse to fd
        }
    }
    else if (strcmp(message->method, "PUT") == 0)
    {   /*read/write, path if file isn't exist then create, if file exist, allow write */
        message->fd = open(message->filename, O_RDWR|O_CREAT|O_TRUNC, 0666);
        int i = message->lengthP; //content
        
        char *message_body = strtok(message->buffer, RNRN); // http /1.1 /filename
        message_body = strtok(NULL, RNRN); // content length 13
        message_body = strtok(NULL, RNRN); // message body --> hello world 
        message->body = message_body;
        // errx(1, "%ld", message->body - message->buffer); //45
        // int max_write = message->lengthP;
        
        // if(max_write > BUFFER_SIZE - message_body + message->buffer)
        // {
        //     max_write = BUFFER_SIZE - message_body + message->buffer;
        // }
        // write(message->fd, message->body, max_write);
        // i -= max_write;

        while(i > 0) 
        {
            int max_read = i;   // how much i need to read == contentlength
            if (max_read > BUFFER_SIZE) // if maxread is greater than buffer size, just read buffersize of maxread 
            {
                max_read = BUFFER_SIZE;
            }
            size_t size = write(message->fd, message->body, max_read);     // write to the fd
            i -= size;
        }
        snprintf(response, BUFFER_SIZE, "%s %s %s\r\nContent-Length: %zu\r\n\r\n%s\n",              // store response 
        message->httpversion, message->status_code, status_phrase, message->content_length, status_phrase);
        send(fd, response, strlen(response), 0);    // send reponse to fd
        
        close(message->fd);
    }
}

int main(int argc, char *argv[])
{
    char *endptr;
    long portNum;
    int server_socket;

    if(argc != 2)
    {
        errx(1, "wrong arguements: %s port_num\nusage: %s <port>", argv[0], argv[0]);
    }
    // ignore white space at the start and takes str in long and remainings
    portNum = strtol(argv[1], &endptr, 10);

    // less than uint16_MAX(65535) or less than 0
  	if (*endptr != '\0' || portNum > UINT16_MAX || (portNum < 1024 && portNum > 0))
    {
    	errx(1, "bind: Permission denied");
    }
    // create the socket to connect, basically fd
    server_socket = create_listen_socket((uint16_t)portNum); 

    // when I ctrl+z it failed+ and gave me binf address is already in use error so check for it.
    if (server_socket == -3)
    {
        errx(1, "bind: Address already in use");
    }

    while(1)
    {
        int fd = accept(server_socket, NULL, NULL); // accept server socket as in fd(client socket)
        // errx(1, "fd value:%d\n", fd);
        struct httpObject message;
        
        recv_process_request(fd, &message);  // process the HTTP request from the client
        // errx(1, "1:%d\n", fd);
        server_response(fd, &message);       // response from the server
        // errx(1, "2:%d\n", fd);
        uint8_t buff[BUFFER_SIZE+1];
        ssize_t bytes = recv(fd, buff, BUFFER_SIZE, 0); // recv fd and store in buff
        buff[bytes] = 0;                                // set null terminate
        write(STDOUT_FILENO, buff, bytes);        // write buff to stdout
    }
    
}