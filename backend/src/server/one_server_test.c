#include "../../include/socket/Sockets.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include "one_server_socket.c"


int main()
{
    if(create_socket() < 0)
    {
        perror("Error creating a socket");
        return -1;
    }
    return 0;
}
