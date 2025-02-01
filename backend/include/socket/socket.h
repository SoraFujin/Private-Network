#ifndef CREATE_SOCKET_H
#define CREATE_SOCKET_H

#include <cstddef>
#include <sys/socket.h>
#define MAX_BUFFER 1024
#define PORT 7777
int create_socket(int *socket_fd, int type, int protocol);
int bind_socket(int *socket_fd, const struct sockaddr *addr, socklen_t len, int sin_family, int port, char ip_addr);
int socket_listen(int *socket_fd, int waiting_number);
int accept_connection(int *socket_fd, struct sockaddr *__restrict addr, socklen_t *__restrict addr_len);
int allocate_data(char *data, size_t data_size);
int free_data(char *data);
int set_socket_opt(int *socket_fd, int level, int optname, const void *optvalue, socklen_t optlen, const int opt, struct sockaddr_in socket_addr);

#endif
