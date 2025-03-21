#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "../../include/Protocols/protocol.h"

/* 
 * Main code that handles the data sent and recieved and the connections established 
 * When connecting to another socket or network the client needs to send a connection request to the server 
 * The server responds with a connect_acc and an acknowledgment sequence number 
 * After both sides agree they send the final ack 
 * (Three Way Handshake)
 *
 *
 * Define how the data is transfered
 * The data is sent in chunks each with a seq number 
 * The receiver will send back and ack with the expected sequence number (rec_seq)
 *
 *
 * Connection Termination
 * When one side wants to close the connection it sends a termination message and the other side acknowledges it
 *
 * Messages in the Protocol:
 * Each message should have a header that contains:
 * 1. source 
 * 2. destination
 * 3. sequnce number 
 * 4. flags 
 * This is done using the struct above (Toji) 
 * The Header will contain essential information:
 * Source (who is sending the message (socket or user)
 * Destination (who is receiving the message)
 * Sequence Number for tracking the order of messages 
 * Flags indicate the type of message (connect request, data, ack)
 * Length How long the data portion is 
 * Acknowledgment for confirming the receipt of a message 
 *
 * Each message should also have a payload that is the actual data being transmitted
 * */


/*======= Server Side functions ========*/
// Server needs to handle connection request and client info (managing clients)
// In addition the server has a unique id that the clients can connect and send message to in order to be able to establish a connection
// A server assigns an ID to each client in order to be able to manage them 
// If the clients connecting to ID becomes more, resize the array so it can manage more clients


/* Connection codes:
 * Connect request = 0x01
 * Connect response = 0x02
 * Data message = 0x04 
 * Acknowledgment = 0x08
 * Close connection = 0x10
 * */

/*====== Client Side Functions ====== */

/* typedef struct { */
/*     uint16_t source;       // Source identifier (Could be the sender's socket or ID). */
/*     uint16_t dest;         // Destination identifier (could be the reciever's socket or ID). */
/*     uint8_t flags;         // Flags for different types of messages (connect, data, ack). */
/*     uint16_t seq;          // The sequence number of the data or control message. */ 
/*     uint16_t ack;          // Acknowledgment number (used to confirm receipt of a message). */
/*     uint16_t length;       // Length of the data (Payload). */
/*     char data[1024];       // Payload - the actual data being sent. */
/* } Toji; */


/* ====== Server & Client Functions ===== */
void message()
{
}


int main()
{
    return 0;
}

