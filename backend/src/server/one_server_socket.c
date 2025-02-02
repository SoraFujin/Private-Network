#include "../../include/socket/Sockets.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

// Create a socket and initialize connection
int create_socket()
{
    printf("\t\t----Server----\n\n");
    struct addrinfo hints;
    struct sockaddr_in socket_addr;
    socklen_t socket_addr_length = sizeof(socket_addr);
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_fd < 0)
    {
        perror("Error creating socket");
        return -1;
    }

    printf("Socket created successfully.\n");
    set_socket_options(socket_fd, 1);

    socket_addr.sin_family = AF_INET;
    socket_addr.sin_port = htons(PORT);
    socket_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    bind_socket(socket_fd, &socket_addr);
    start_listening(socket_fd);
    accept_connection(socket_fd, &socket_addr, &socket_addr_length);

    /* close(socket_fd); */
    return 0;
}

// Set socket options
void set_socket_options(int socket_fd, int opt)
{
    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0)
    {
        perror("Error setting socket options");
    }
    printf("Socket options set successfully\n");
}

// Bind the socket
void bind_socket(int socket_fd, struct sockaddr_in *addr)
{
    if (bind(socket_fd, (struct sockaddr*)addr, sizeof(*addr)) < 0)
    {
        perror("Failed to bind");
    }
    printf("Socket bound on port %d\n", PORT);
}

// Start listening on the socket
void start_listening(int socket_fd)
{
    if (listen(socket_fd, 10) < 0)
    {
        perror("Failed to listen");
    }
    printf("Socket listening on port %d\n", PORT);
}

// Accept connections from clients
int accept_connection(int socket_fd, struct sockaddr_in *socket_addr, socklen_t *addr_len)
{
    while (1)
    {
        int connection_socket = accept(socket_fd, (struct sockaddr*)socket_addr, addr_len); 
        if (connection_socket < 0)
        {
            perror("Accept failed");
            return -1;
        }
        printf("New connection accepted\n");
        while( handle_one_client(connection_socket))
            return 0;
        return 0;
    }
    close(socket_fd);
    return 0;
}

// Handle client connection
int handle_one_client(int connection_socket)
{
    char buffer[MAX_BUFFER] = {0};

    while (1)
    {
        int client_data_size = read(connection_socket, buffer, sizeof(buffer) - 1);
        if (client_data_size <= 0)
        {
            if (client_data_size == 0)
                printf("Client disconnected\n");
            else 
                perror("Reading data from client failed");
            break;
        }
        buffer[client_data_size] = '\0';
        printf("Data from client: %s\n", buffer);

        char* hello = "Hello from server";
        send(connection_socket, hello, strlen(hello), 0);
        printf("Message sent to client.\n");
    }
    close(connection_socket);
    return 0;
}

/* int handle_clinets(int connection_socket) */
/* { */
/*     int select = select(int nfds, fd_set *restrict readfds, fd_set *restrict writefds, fd_set *restrict exceptfds, struct timeval *restrict timeout); */
/*     return 0; */
/* } */
