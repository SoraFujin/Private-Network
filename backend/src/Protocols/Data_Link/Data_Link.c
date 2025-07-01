#include "include/data_link.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#define HEADER_SIZE 17


/*
 *============ Sending ==============
 *
 * */

uint16_t calculate_checksum(const uint8_t *data, uint8_t length) {
    uint16_t crc = 0xffff; 

    for (int i = 0; i < length; i++) {
        crc ^= (data[i] << 8); 

        for (int j = 0; j < 8; j++) {
            if (crc & 0x8000) {
                crc = (crc << 1) ^ P;
            } else {
                crc <<= 1;
            }
        }
    }
    return crc; 
}

int seq_num_bytes(size_t payload_size)
{
    uint32_t number_of_frames = payload_size / MAX_PAYLOAD_SIZE;
    printf("Number of Frames needed: %u\n", number_of_frames);
    if (number_of_frames <= 0) {
        return 0;
    }
    if (number_of_frames <= 255) {
        return 1;
    }
    if (number_of_frames <= 65535) {
        return 2;
    }
    return 4;
}

void create_frame(const uint8_t *payload, size_t payload_size, uint32_t sequence_number, size_t chunk_size, uint16_t checksum)
{
    int bytes_seq = seq_num_bytes(payload_size);
    size_t frame_size = HEADER_SIZE + bytes_seq + chunk_size;
    uint8_t *frame = malloc(frame_size);


    frame[SOF_INDEX] = SOF;
    frame[SEQ_BYTE] = bytes_seq;
    frame[SEQ_INDEX] = sequence_number;
    frame[DEST_INDEX] = 1;
    frame[SRC_INDEX] = 2;
    frame[LENGTH_INDEX] = chunk_size;
    frame[PAYLOAD] = *payload;
    frame[CRC_INDEX] = checksum;

    printf("\n=== FRAME ===\n");
    printf("SOF: %u\n", SOF);
    printf("SEQ Bytes: %u\n", bytes_seq);
    printf("SEQ NUM: %u\n", sequence_number);
    printf("Chunk size: %zu\n", chunk_size);
    printf("Checksum: %u\n", checksum);
    printf("Payload: \"");
    for(size_t i = 0; i < chunk_size; i++) {
        putchar(payload[i]);
    }
    printf("\"\n");

}

void allocate_payload(uint8_t *payload, size_t payload_size) 
{
    int offset = 0;
    uint8_t *chunk = 0;
    uint32_t sequence_number = 0;
    while (payload_size > offset)
    {
        size_t chunk_size = 0;
        if(payload_size - offset > MAX_PAYLOAD_SIZE) {
            chunk_size = MAX_PAYLOAD_SIZE;
            chunk = &payload[chunk_size];
        } else {
            chunk_size = payload_size - offset;
            chunk = &payload[offset];
        }
        offset += chunk_size;
        sequence_number++;
        uint16_t checksum = calculate_checksum(payload, chunk_size);
        create_frame(chunk, payload_size, sequence_number, chunk_size, checksum);
    }
}

/*============= Receiving ============*/


int main(void) 
{
    printf("\n --- Testing large data ---\n");
    size_t huge_data_size = 1ULL * 1024 * 1024 * 1024 + 432;
    uint8_t *huge_data = malloc(huge_data_size);

    if (huge_data == NULL) {
        perror("Failed to allocate test data");
        return 1;
    }

    for (size_t i = 0; i < huge_data_size; i++) {
        huge_data[i] = (uint8_t)((i % 26) + 'A');  
    }

    allocate_payload((uint8_t*)huge_data, huge_data_size);

    free(huge_data);

    return 0;
}
