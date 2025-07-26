#ifndef SOCKETS_H
#define SOCKETS_H

#include <stddef.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <poll.h>
#include <sys/poll.h>


#define MAX_BUFFER 1024
#define PORT 7777

// Public function
int create_socket(void);

// Internal functions for creating the socket
void set_socket_options(int socket_fd, int opt);
void bind_socket(int socket_fd, struct sockaddr_in *socket_addr);
void start_listening(int socket_fd);
int accept_connection(int socket_fd, struct sockaddr_in *socket_addr, socklen_t *addr_len);
int handle_one_client(int connection_socket);
int handle_clients(int connection_socket);

// Monitor multiple sockets and clients
int create_server_socket(void);
void accept_new_connection(int server_socket, struct pollfd **poll_fds, int *poll_count, int *poll_size);
void read_data_from_socket(int i, struct pollfd **poll_fds, int *poll_count, int server_socket);
void add_to_poll_fds(struct pollfd *poll_fds[], int new_fd, int *poll_count, int *poll_size);
void del_from_poll_fds(struct pollfd **poll_fds, int i, int *poll_count);
void handle_client_connection(int client_fd);

// Authentication methods
int authentication(void);

#endif // SOCKETS_H
