#ifndef SOCKETS_H
#define SOCKETS_H

#include <stddef.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAX_BUFFER 1024
#define PORT 7777

// Public function
int create_socket();

// Internal functions
void set_socket_options(int socket_fd, int opt);
void bind_socket(int socket_fd, struct sockaddr_in *socket_addr);
void start_listening(int socket_fd);
int accept_connection(int socket_fd, struct sockaddr_in *socket_addr, socklen_t *addr_len);
int handle_client(int connection_socket);

#endif // SOCKETS_H
