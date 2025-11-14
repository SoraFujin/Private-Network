#ifndef NETWORK_LAYER
#define NETWORK_LAYER

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>

// These flags tells what kind of message the network layer is sending
#define DISCV_MSG 0x7E
#define DATA_MSG 0xAE

void send_to_data_link();
void create_table();
void discovery_message();
void create_packet();
void read_packet();

#endif
