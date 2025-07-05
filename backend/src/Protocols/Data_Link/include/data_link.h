#ifndef DATA_LINK
#define DATA_LINK

#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PAYLOAD_SIZE 1024
#define SOF 0x7E
#define P 0x1021  // Polynomial for CRC

// Pre-defined indexes for the data inside the frames
#define SOF_INDEX      0
#define SEQ_BYTE       1
#define LENGTH_BYTE    2
#define SEQ_INDEX      3

uint16_t calculate_checksum(const uint8_t *data, uint8_t length);
int seq_num_bytes(size_t payload_size, uint32_t number_of_frames);
void create_frame(const uint8_t *payload, size_t payload_size, uint32_t sequence_number, size_t chunk_size, uint16_t checksum);
void allocate_payload(uint8_t *payload, size_t payload_size);

#endif 

