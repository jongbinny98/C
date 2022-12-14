#include <stdint.h>
#include <sys/socket.h> // bind, connect, listen, accept, socket, recv, send
#include <sys/stat.h>   // stat
#include <signal.h>
#include <string.h>     // strlen(), strstr()
#include <stdio.h>
#include <stdlib.h>     // strtol()
#include <err.h>        // errx
#include <unistd.h>     // read(), write(), close()
#include <fcntl.h>

#include <arpa/inet.h>

// Parses port number and binds and listens on it.
// returns a fd as a positive integer if successful
// returns -1 if passed an invalid port number
// returns -2 if opening the socket failed
// returns -3 if binding the socket failed
// returns -4 if listening failed
int create_listen_socket(uint16_t port);
