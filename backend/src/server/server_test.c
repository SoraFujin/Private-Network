#include <stdio.h>
#include "../../include/socket/Sockets.h"

int main()
{
    int server_socket = create_socket();
    if(server_socket < 0)
    {
        perror("Error creating a socket\n");
        return -1;
    }
    return 0;
}
