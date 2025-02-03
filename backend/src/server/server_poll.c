#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/poll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include "../../include/socket/Sockets.h"


int create_server_socket()
{
    struct sockaddr_in socket_addr;
    int socket_fd;
    int status;

    memset(&socket_addr, 0, sizeof socket_addr);
    socket_addr.sin_family = AF_INET;
    socket_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    socket_addr.sin_port = htons(PORT);

    socket_fd = socket(socket_addr.sin_family, SOCK_STREAM, 0);
    if(socket_fd < 0)
    {
        fprintf(stderr, "[Server] Socket error: %s\n", strerror(errno));
        return -1;
    }
    printf("[Server]: Created server socket fd: %d\n", socket_fd);

    status = bind(socket_fd, (struct sockaddr*)&socket_addr, sizeof socket_addr);
    if(status < 0)
    {
        fprintf(stderr, "[Server] Bind error: %s\n", strerror(errno));
        return -1;
    }
    printf("[Server]: Bound socket to localhost port %d\n", PORT);

    return socket_fd;
}

void accept_new_connection(int server_socket, struct pollfd **poll_fds, int *poll_count, int *poll_size)
{
    int client_fd;
    char msg_to_send[BUFSIZ];
    int status;

    client_fd = accept(server_socket, NULL, NULL);
    if(client_fd < 0)
    {
        fprintf(stderr, "[Server]: Accept error: %s\n", strerror(errno));
        return ;
    }
    add_to_poll_fds(poll_fds, client_fd, poll_count, poll_size);

    printf("[Server]: Accepted new connection on client socket %d\n", client_fd);

    memset(&msg_to_send, '\0', sizeof msg_to_send);
    sprintf(msg_to_send, "Welcome. You are client fd [%d]\n", client_fd);
    status = send(client_fd, msg_to_send, strlen(msg_to_send), 0);
    if(status < 0)
        fprintf(stderr, "[Server]: Send error to client %d: %s\n", client_fd, strerror(errno));

}

void read_data_from_socket(int i, struct pollfd **poll_fds, int *poll_count, int server_socket)
{
    char buffer[BUFSIZ];
    char msg_to_send[BUFSIZ];
    int bytes_read;
    int status;
    int dest_fd;
    int sender_fd;

    sender_fd = (*poll_fds)[i].fd;
    memset(&buffer, '\0', sizeof buffer);
    bytes_read = recv(sender_fd, buffer, BUFSIZ, 0);
    if (bytes_read <= 0)
    {
        if (bytes_read == 0) 
            printf("[%d] Client socket closed connection.\n", sender_fd);
        else 
            fprintf(stderr, "[Server] Recv error: %s\n", strerror(errno));
        close(sender_fd); 
        del_from_poll_fds(poll_fds, i, poll_count);
    }
    else
    {
        printf("[%d] Got message: %s", sender_fd, buffer);

        memset(&msg_to_send, '\0', sizeof msg_to_send);
        snprintf(msg_to_send, sizeof(msg_to_send), "[%d] says: %.8180s", sender_fd, buffer);
        for (int j = 0; j < *poll_count; j++) 
        {
            dest_fd = (*poll_fds)[j].fd;
            if (dest_fd != server_socket && dest_fd != sender_fd) 
            {
                status = send(dest_fd, msg_to_send, strlen(msg_to_send), 0);
                if (status == -1) 
                    fprintf(stderr, "[Server] Send error to client fd %d: %s\n", dest_fd, strerror(errno));
            }
        }
    }
}

void add_to_poll_fds(struct pollfd *poll_fds[], int new_fd, int *poll_count, int *poll_size) {
    if (*poll_count == *poll_size) 
    {
        *poll_size *= 2; 
        *poll_fds = realloc(*poll_fds, sizeof(**poll_fds) * (*poll_size));
    }
    (*poll_fds)[*poll_count].fd = new_fd;
    (*poll_fds)[*poll_count].events = POLLIN;
    (*poll_count)++;
}

void del_from_poll_fds(struct pollfd **poll_fds, int i, int *poll_count) {
    (*poll_fds)[i] = (*poll_fds)[*poll_count - 1];
    (*poll_count)--;
}

/* int main(void) */
/* { */
/*     printf("\t\t---- SERVER ----\n\n"); */

/*     int server_socket; */
/*     int status; */
/*     struct pollfd *poll_fds; */ 
/*     int poll_size; */ 
/*     int poll_count; */

/*     server_socket = create_server_socket(); */
/*     if (server_socket == -1) */ 
/*         return (1); */

/*     printf("[Server] Listening on port %d\n", PORT); */
/*     status = listen(server_socket, 10); */
/*     if (status != 0) */ 
/*     { */
/*         fprintf(stderr, "[Server] Listen error: %s\n", strerror(errno)); */
/*         return (3); */
/*     } */

/*     poll_size = 5; */
/*     poll_fds = calloc(poll_size + 1, sizeof *poll_fds); */
/*     if (!poll_fds) */ 
/*         return (4); */
/*     poll_fds[0].fd = server_socket; */
/*     poll_fds[0].events = POLLIN; */
/*     poll_count = 1; */

/*     printf("[Server] Set up poll fd array\n"); */

/*     while (1) */ 
/*     { */
/*         status = poll(poll_fds, poll_count, 2000); */
/*         if (status == -1) */ 
/*         { */
/*             fprintf(stderr, "[Server] Poll error: %s\n", strerror(errno)); */
/*             exit(1); */
/*         } */
/*         else if (status == 0) */ 
/*         { */
/*             printf("[Server] Waiting...\n"); */
/*             continue; */
/*         } */

/*         for (int i = 0; i < poll_count; i++) */ 
/*         { */
/*             if ((poll_fds[i].revents & POLLIN) != 1) */ 
/*                 continue ; */
/*             printf("[%d] Ready for I/O operation\n", poll_fds[i].fd); */
/*             if (poll_fds[i].fd == server_socket) */ 
/*                 accept_new_connection(server_socket, &poll_fds, &poll_count, &poll_size); */
/*             else */ 
/*                 read_data_from_socket(i, &poll_fds, &poll_count, server_socket); */
/*         } */
/*     } */
/*     return (0); */
/* } */
