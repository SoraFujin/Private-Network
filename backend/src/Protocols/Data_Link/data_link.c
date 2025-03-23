#include <complex.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PAYLOAD_SIZE 1024
#define SOF 0x7E
#define eof 0x7F
/* #define P 0x8005 */
#define P 0x1021

/** Data Link Layer **/
/* 
 * This layer ensures reliable communication between directly connected devices
 * Framing - Defining how data is packaged into frames for transmission.
 * Addressing - Assigining unique IDs to devices so data is sent to the correct recipient .
 * Error detecting - Ensuring data is received correctly without corruption.
 * 
 *
 * Protocols inside the Data Link Protocol: 
 * 1. SDLC (Synchronous Data Link)
 * 2. HDLC (High Level Data Link)
 * 3. SLIP (Serial Line Interface Protocol)
 * 4. PPP (Point-To-Point Protocol)
 * 5. LCP (Link Control Protocol)
 * 6. LAP (Link Access Procedure)
 * 7. NCP (Network Control Protocol)
 * */

typedef struct {
    uint16_t sof; //start of the frame 
    uint8_t source; // Where did the data come from 
    uint8_t destination; // Where the data is going 
    uint8_t length; // Length of the data being sent 
    uint8_t payload[1024]; // The data that is sent 
    uint8_t checksum; // Error detection
} Frame;

// Create the frame, calculate the data size, calcualte the checksum append everything to the frame, then send it through the frame.
// The reciever will take the frame reads the SOF checks the data it has, calculate the checksum using the data length, compares the checksum.

// Change the checksum to use CRC-16 encoding to check for any errors
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

// For now i will print the data that is being sent and its length, later own using the source and dest will be sent to the appropriate reciever
void send_frame(Frame *frame)
{
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
