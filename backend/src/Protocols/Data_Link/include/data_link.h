#ifndef DATA_LINK
#define DATA_LINK

#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PAYLOAD_SIZE 1024
#define SOF 0x7E
#define EOP 0x7F
#define P 0x1021  // Polynomial for CRC

// Pre-defined indexes for the data inside the frames
#define SOF_INDEX    0
#define SEQ_BYTE     1
#define SEQ_INDEX    2
#define FLAG_INDEX   3
#define DEST_INDEX   4
#define SRC_INDEX    5
#define LENGTH_INDEX 6
#define PAYLOAD      7
#define CHECKSUM_INDEX    8



// If using index based frames no need for structs
typedef struct {
    uint16_t sof; // Start of frame
    uint8_t source; // Source address
    uint8_t destination; // Destination address
    uint8_t length; // Length of the payload
    uint8_t payload[MAX_PAYLOAD_SIZE]; // Payload data
    uint8_t checksum; // Error detection checksum
    uint8_t eop; // End of frame
} Frame;

// TODO: the structs will not be used, since I will implement an index based approach

#endif // DATA_LINK

