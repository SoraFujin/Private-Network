/**
 * @file data_link.h
 * @brief Implementation of a simple Data Link Layer 
 *
 * This file simulates a basic Data Link Lyaer communication mechanism,
 * It includes functions for:
 *  - Framing data for transmission (Start of Frame, Source, Destination, Length, Payload, and Checksum).
 *  - Calculating checsums using CRC for error detection.
 *  - Creating frames and sending/receiving them with integrity verification.
 *
 *
 * @author Ahmad Diab
 * @data 
 *
 */
// Create the frame, calculate the data size, calcualte the checksum append everything to the frame, then send it through the frame.
// The reciever will take the frame reads the SOF checks the data it has, calculate the checksum using the data length, compares the checksum.

#include "./include/data_link.h"
#include <stdint.h>

uint16_t calculate_checksum(uint8_t *data, uint8_t length) {
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

void create_frame(uint8_t source, uint8_t destination, char *data, Frame *frame)
{
    // TODO: the size of the payload to be dyncamically added to memory using malloc
    // Index based approach
    uint16_t data_length = strlen(data);

    if(data_length > MAX_PAYLOAD_SIZE)
    {
        printf("Error: Data too large\n");
        return;
    }

    frame->sof = SOF;
    frame->destination = destination;
    frame->length = data_length;
    frame->source = source;

    for(uint16_t i = 0; i < data_length; i++)
        frame->payload[i] = data[i];

    frame->checksum = calculate_checksum(frame->payload, data_length);
    printf("Check Sum = %d\n", frame->checksum);
}

int check_integrity(Frame *frame)
{
    uint8_t checksum = calculate_checksum(frame->payload, frame->length);
    return (checksum == frame->checksum);
}

// For now I will print the data that is being sent and its length, later own using the source and dest will be sent to the appropriate reciever
// The way to send data is to create an array of frames, depending on the size of data, to ease the way to check for the integrity of data and
// extract the data
void send_frame(Frame *frame)
{
    Frame frm[225]; // Allocate the memory dyncamically instead of static size based on the size of data, max size of packet 56,000 -> check
                    // Size of the frame 1500 bytes each together to get one whole packet
                    // this is sent to the socket/server
    if(frame == NULL)
    {
        perror("Error: Frame is empty");
        return;
    }

    
    printf("Payload: %s\n", frame->payload);
}

int recieve_frame(Frame *frame)
{
    return check_integrity(frame);
}

int main(void)
{
    Frame frm;
    char *data = "Hello World!";

    create_frame(0, 0, data, &frm);

    printf("Payload Length %d\n", frm.length);
    send_frame(&frm);


    if(recieve_frame(&frm))
    {
        printf("Recieved\n");
    }

    return 0;
}
