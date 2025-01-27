#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 7777


int main()
{
    int status, data_size, client_fd;
    struct sockaddr_in server_addr;
    char* hello = "Hello from client";
    char buffer[1024] = { 0 };

    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(client_fd < 0)
    {
        perror("Client Socket error\n");
        return -1;
    }
    printf("Socket created successfully\n");

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    int ip_addr = inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);
    if(ip_addr <= 0)
    {
        printf("\n Invalid address/ Address not supported \n");
        return -1;
    }

    status = connect(client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if(status < 0)
    {
        printf("Connection failed\n");
        return -1;
    }
    send(client_fd, hello, strlen(hello), 0);
    printf("Hello Message sent\n");
    data_size = read(client_fd, buffer, 1024 - 1);
    printf("%s\n", buffer);

    close(client_fd);

    return 0;
}
