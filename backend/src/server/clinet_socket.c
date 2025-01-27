#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 7777

// Main improvements:
// Option to add the ip server to connect to
// Option to send specific data to the server
// Option to close the connection when needed
// Option to restart the connection 
int main()
{
    int status, data_size, client_fd;
    struct sockaddr_in server_addr;
    char* data = malloc(64 * sizeof(char));
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

    char* ip_addr_con = malloc(16 * sizeof(char));  // Allocate memory for the IP address string
    if (ip_addr_con == NULL) {
        perror("Memory allocation failed");
        return -1;
    }

    printf("Enter the IP address to connect to: \n");
    scanf("%s", ip_addr_con);  // Read the IP address from the user

    // Convert the IP address to binary form
    int ip_addr = inet_pton(AF_INET, ip_addr_con, &server_addr.sin_addr);
    if (ip_addr <= 0) {
        printf("\n Invalid address/ Address not supported \n");
        free(ip_addr_con);  // Free allocated memory
        return -1;
    }

    status = connect(client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if(status < 0)
    {
        printf("Connection failed\n");
        return -1;
    }
    printf("Send message to server\n");
    fgets(data, 64, stdin);
    data[strcspn(data, "\n")] = '\0';

    send(client_fd, data, strlen(data), 0);
    printf("Hello Message sent\n");

    free(data);

    data_size = read(client_fd, buffer, 1024 - 1);
    /* printf("%s\n", buffer); */

    close(client_fd);

    return 0;
}
