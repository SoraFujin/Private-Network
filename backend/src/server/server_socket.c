#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 7777

int main ()
{
    /*
     * This struct (server_addr) identifies the IP address of the socket in this case IPV4
     * The port number, after transforming it to short in order to be able to read it in big indian when going through the network,
     * and the IP address it will use, since this is local address (on this machine) the address is 127.0.0.1
     * This contains all the information that the socket needs to be bind after being created
     * */
    struct sockaddr_in server_addr;
    socklen_t server_addr_length = sizeof(server_addr); // storing the length for the server_addr struct
    int opt = 1; // This variable is used to identify if the operation that is used are being enabled or disabled.
    int socket_fd, new_socket; // creating a socket (socket_fd), and a new socket that is used as a channel between the server socket and the client that is connected to the server
                               // After the server listens to incoming request and accept, the new_socket is used to send and receive data between the server and client
                               // Basiclly we need 2 sockets, one that listens to a specific port and address, and the other to receive and send data to the client

    // for starting, i am sending a specific message to the client, and receiving a small data size, later on it will accept the data daynamically
    // for testing at the start messages sent and received will be static
    ssize_t client_data_size;
    char buffer[1024] = { 0 };
    char* hello = "Hello from server";

    // Creating the socket, using AF_INET to specify the IP address used (in this case IPV4)
    // Using SOCK_STREAM, to specify the connection type this uses TCP connection 
    // and 0 for the protocol, in this case 0 is used for Interenet protocol (basic)
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);


    // After creating the socket using (socket()), check if the returned value is a positive integer 
    // Which means that it was created or -1 which will throw an error
    // Check if the socket has been created correctly
    if (socket_fd < 0)
    {
        perror("Socket Creation Failed.\n");
        return -1;
    }
    printf("Socket Created successfully with file descriptor %d\n", socket_fd);

    /* 
     * This is used to set options for the socket, so it allows to modify the behavior of a socket before using it to communicate.
     * It allows the use of address and port.
     * The first it is given the socket (socket_fd) that has been created above.
     * Then the level at which the option resides, which means the layer at whcih the socket option applies, 
     * it determains wehter the option applies to specific protocol (such as TCP or UDP), or the socket it self (which will be the transport layer).
     * When using SOL_SOCKET, it means the option is applying to the socket it self not a specific protocol. 
     * optname: this uses the SO_REUSEADDR SO_REUSEPORT, which allows the socket to bind to an address and port that is in the TIME_WAIT state
     * a state when a connection is recently closed but is still in the process of cleaning up resources. 
     * This is useful when the server is restarted and the bind hte same port without waiting for system to release the port.
     * optval: to let the setsockopt know if the options that i am using are enabled or disabled, in this case (1) which means enabled 
     * SIDE NOTE: For the second option (level), if i am working on a specific level (layer) i can specify exactly what protocol a socket uses,
     * depending on what i am using (TCP, UDP, IP) it will affect how the socket handles the data that is used to transfer, therefore i need to specify the level i am working with
     * NOTE: at the start it will use the defults, later on I will try to implement my own custom level and optname variables to use here.
     */
    int socket_opt = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

    if(socket_opt < 0)
    {
        perror("Setsockopt Failed");
        return -1;
    }
    printf("Socket options set successfully (SO_REUSEADDR | SO_REUSEPORT)\n");

    // The specific information for binding the socket, ip address type, port, and ip address for the socket 
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // This is used to bind the server to a specific ip address
                                                          // The ip addresses it can accpet, for testing i will use the local host ip address 
                                                          // for later will change to INADDR_ANY to allow the server to accept connection on any of the host's network interfaces

    int bind_socket = bind(socket_fd,(struct sockaddr *) &server_addr,sizeof(server_addr));

    // Check if the socket is bind correctly
    if (bind_socket < 0)
    {
        perror("Bind Failed. \n");
        return -1;
    }
    printf("Socket bound to port %d successfully\n", PORT);

    // Making the socket listen to the specified port and wait for connection
    // the value 7 (backlog) means the server can accept up to 3 connection in the waiting queue at once, if there are more than 3 connection the connection will be dropped.
    // Then check if it is listening or not
    int server_listen = listen(socket_fd, 7);
    if(server_listen < 0)
    {
        perror("Listen Failed");
        return -1;
    }
    printf("Server is listening for connections on port %d\n", PORT);

    // This will keep the connection opened to accept multiple connection from different clients
    // There is 2 ways that can be used to handle connections:
    // Single connection without using while loop, or multiple connection that uses the while loop,
    // However to combine both logics, I can use the while loop to handle multiple clients and close the connection after each interaction
    while(1)
    {
        // Using the new socket to be abel to send and receive data from the client, it accepts the connection with the socket server, the address struct, and the length of that struct
        // The struct here holds the clinet address information
        // and the server lenfht will be updated to reflect the actual address length
        new_socket = accept(socket_fd, (struct sockaddr*)&server_addr, &server_addr_length);

        if(new_socket < 0)
        {
            perror("Accept Failed\n");
            continue; // Even if it fails it goes back to waiting to accept new connections.
        }
        printf("New connection is accepted\n");

        // This will be used to read the data from the clinet by using the new_socket connection.
        // the buffer array will be used to store the incoming data from the client.
        // 1024 -1 : the size of the buffer to read from the socket, subtracting 1 in order to leave spacve for the null terminator ('\0') that marks the end of a string in c
        // This ensures the buffer will not overflow when storing incoming data
        // Thus the read() will read up to   1024 - 1 bytes from the socket into the buffer and returns the number of bytes it actually read (Stored in client_data_size).
        // in c Strings are null terminated, which means they end with a '\0', if it is not manually added or checked after reading the data the string might be invalide 
        client_data_size = read(new_socket, buffer, 1024 - 1);
        // When the clinet disconnected the data size will be 0, therefore its good practice to check when the clinet is disconnected.
        if (client_data_size == 0)
        {
            printf("Clinet disconnected\n");
            break;
        }
        buffer[client_data_size] = '\0';
        printf("Received: %s\n", buffer);

        // This sends data back to the client using new_socket connection
        send(new_socket, hello, strlen(hello), 0);
        printf("Hello message sent\n");

    }
    // This closes the connection with the new_socket, once the communication with the client is done, we should close the socket to free up resources.
    close(new_socket);

    return 0;
}
