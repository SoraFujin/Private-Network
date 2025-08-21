#ifndef DATA_LINK
#define DATA_LINK

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <stddef.h>


#define MAX_PAYLOAD_SIZE 1024
#define SOF 0x7E
#define P 0x1021  // Polynomial for CRC

// Pre-defined indexes for the data inside the frames
#define SOF_INDEX      0
#define SEQ_BYTE       1
#define LENGTH_BYTE    2
#define SEQ_INDEX      3

// static uint8_t* frame;

uint16_t calculate_checksum(const uint8_t *data, uint8_t length);
int seq_num_bytes(uint32_t number_of_frames);
void create_frame(const uint8_t *payload, size_t payload_size, uint32_t sequence_number, size_t chunk_size, uint8_t source, uint8_t dest, uint16_t checksum);
void allocate_payload(uint8_t *payload, size_t payload_size, uint8_t source, uint8_t dest);
void read_frame(uint8_t *frame, size_t frame_size, size_t number_of_frames);
int send_frame(uint8_t* frame, size_t frame_size, uint8_t source, uint8_t dest);

#endif 

