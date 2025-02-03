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
#include "server_poll.c"
/* #include "../../include/socket/Sockets.h" */

int main()
{

    /* int server_socket = create_socket(); */
    /* if(server_socket < 0) */
    /* { */
    /*     perror("Error creating a socket\n"); */
    /*     return -1; */
    /* } */
    printf("\t\t------ SERVER ------\n\n");

    int server_socket;
    int status;
    struct pollfd *poll_fds; 
    int poll_size; 
    int poll_count;

    server_socket = create_server_socket();
    if (server_socket == -1) 
        return (1);

    printf("[Server] Listening on port %d\n", PORT);
    status = listen(server_socket, 10);
    if (status != 0) 
    {
        fprintf(stderr, "[Server] Listen error: %s\n", strerror(errno));
        return (3);
    }

    poll_size = 5;
    poll_fds = calloc(poll_size + 1, sizeof *poll_fds);
    if (!poll_fds) 
        return (4);
    poll_fds[0].fd = server_socket;
    poll_fds[0].events = POLLIN;
    poll_count = 1;

    printf("[Server] Set up poll fd array\n");

    while (1) 
    {
        status = poll(poll_fds, poll_count, 2000);
        if (status == -1) 
        {
            fprintf(stderr, "[Server] Poll error: %s\n", strerror(errno));
            exit(1);
        }
        else if (status == 0) 
        {
            printf("[Server] Waiting...\n");
            continue;
        }

        for (int i = 0; i < poll_count; i++) 
        {
            if ((poll_fds[i].revents & POLLIN) != 1) 
                continue ;

            printf("[%d] Ready for I/O operation\n", poll_fds[i].fd);
            if (poll_fds[i].fd == server_socket) 
                accept_new_connection(server_socket, &poll_fds, &poll_count, &poll_size);
            else 
                read_data_from_socket(i, &poll_fds, &poll_count, server_socket);
        }
    }
    return (0);
    }
