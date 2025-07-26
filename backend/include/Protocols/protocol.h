/* 
 * Network Layers:
 * Physical layer = How the data phyiscally travels (Ethernet, RF, etc)
 * Data Link Layer = Hanldes MAC addresses and local delivery 
 * Network Layer = routing and addressing between nodes (like IP)
 * Transport Layer = Reliable data delivery (like TCP)
 * */
#ifndef PROTOCOL_H  
#define PROTOCOL_H

#include <stdint.h> 

// ===== Server =====
#define SERVERID 7 // unique ID for the server in order for clinets to send the messages to

typedef struct {
    uint16_t source;       // Source identifier (Could be the sender's socket or ID).
    uint16_t dest;         // Destination identifier (could be the reciever's socket or ID).
    uint8_t flags;         // Flags for different types of messages (connect, data, ack).
    uint16_t seq;          // The sequence number of the data or control message. 
    uint16_t ack;          // Acknowledgment number (used to confirm receipt of a message).
    uint16_t length;       // Length of the data (Payload).
    char data[1024];       // Payload - the actual data being sent.
} Toji;

/* Connection codes:
 * Connect request = 0x01
 * Connect response = 0x02
 * Data message = 0x04 
 * Acknowledgment = 0x08
 * Close connection = 0x10
 * */


// ===== Client =====
typedef struct {
    int fd;
    uint16_t id;
} ClientInfo;

#endif  
