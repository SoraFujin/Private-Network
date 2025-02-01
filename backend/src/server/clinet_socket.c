#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAX_BUFFER 1024

int main()
{
    int client_fd, status, data_size;
    struct sockaddr_in server_addr;
    char *data = NULL, buffer[MAX_BUFFER] = {0};

    const in_port_t port = 7777;
    // Allocate memory for data buffer
    data = malloc(64 * sizeof(char));
    if (!data)
    {
        perror("Failed to allocate memory for data");
        return -1;
    }

    // Allocate memory for IP address input
    char *ip_addr_con = malloc(16 * sizeof(char));
    if (!ip_addr_con)
    {
        perror("Failed to allocate memory for IP address");
        free(data);
        return -1;
    }

    // Create the client socket
    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd < 0)
    {
        perror("Socket creation failed");
        free(data);
        free(ip_addr_con);
        return -1;
    }
    printf("Socket created successfully\n");

    // Initialize server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    while (1) // Loop to get a valid IP address
    {
        printf("Enter the server IP address (e.g., 127.0.0.1): ");
        if (!fgets(ip_addr_con, 16, stdin))
        {
            perror("Failed to read IP address");
            free(data);
            free(ip_addr_con);
            close(client_fd);
            return -1;
        }

        // Remove newline character from input
        ip_addr_con[strcspn(ip_addr_con, "\n")] = '\0';

        // Convert IP address from text to binary
        if (inet_pton(AF_INET, ip_addr_con, &server_addr.sin_addr) <= 0)
        {
            printf("Invalid IP address. Please try again.\n");
            continue; // Retry entering the IP
        }
        break;
    }

    // Attempt to connect to the server
    status = connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (status < 0)
    {
        perror("Connection to server failed");
        free(data);
        free(ip_addr_con);
        close(client_fd);
        return -1;
    }
    printf("Connected to the server successfully.\n");

    while (1) // Main communication loop
    {
        printf("Enter message to send (type 'exit' to quit): ");
        if (!fgets(data, 64, stdin))
        {
            perror("Failed to read input");
            break;
        }

        // Remove newline character from input
        data[strcspn(data, "\n")] = '\0';

        // Check for exit condition
        if (strcmp(data, "exit") == 0)
        {
            printf("Closing connection...\n");
            break;
        }

        // Send the message to the server
        status = send(client_fd, data, strlen(data), 0);
        if (status < 0)
        {
            perror("Failed to send message");
            break;
        }
        printf("Message sent: %s\n", data);

        // Receive response from the server
        data_size = read(client_fd, buffer, MAX_BUFFER - 1);
        if (data_size < 0)
        {
            perror("Failed to receive message");
            break;
        }
        else if (data_size == 0)
        {
            printf("Server closed the connection.\n");
            break;
        }
        else
        {
            buffer[data_size] = '\0'; // Null-terminate received data
            printf("Server response: %s\n", buffer);
        }
    }

    // Clean up resources
    free(data);
    free(ip_addr_con);
    close(client_fd);
    printf("Connection closed. Goodbye!\n");

    return 0;
}
