#include <stdio.h>
#include "include/Network_Layer.h"

/* 
 * Custom Network layer: 
 * The Network layer does not deal with actual data.
 * Network Layer main functions:
 *  1. Logical Addressing (IP Addressing)
 *       * Assigns source and destination IP addresses to packets.
 *       * Ensures data reaches the correct network (not just the local segment).
 *
 *   2. Routing
 *       * Determines the best path for data to travel (using routing tables, protocols like OSPF, BGP, RIP).
 *       * Forwards packets between different networks (e.g., LAN → WAN → Internet).
 *
 *   3. Fragmentation & Reassembly
 *       * If a packet is too large for a network’s MTU (Maximum Transmission Unit), the Network Layer splits it into smaller fragments.
 *       * The receiving host’s Network Layer reassembles the fragments.
 *
 *   4. Error Handling (Limited)
 *       * Detects routing failures (e.g., "Destination Unreachable" in ICMP).
 *       * Does not guarantee delivery (that’s Transport Layer’s job, e.g., TCP).
 *
 *   5. Congestion Control (Basic)
 *      * Some protocols (e.g., IP) can drop packets if the network is overloaded.
 *
 *
 * ---------------------
 *  Goal:
 *      Implement a simplified network layer using custom IP-like identifiers (0-255),
 *      with dynamic device discovery and MAC address mapping. Suitable for a small network OS.
 *  Core Functionalities:
 *  1. Addressing Scheme:
 *      - Each device is assigned a unique address (0-255).
 *      - These are logical addresses, not real IPv4 addresses.
 *      - The address space is small and fixed to keep the protocol lightweight.
 *  2. Dynamic MAC Resolution:
 *      - Each logical address (ID) is mapped to a MAC address of a connected device.
 *      - This is achieved through a discovery protocol at startup or when needed.
 *      - Devices respond to a broadcast message with their ID and MAC.
 *      - The Network Layer builds and maintains a mapping structure:
 *          * Suggested: hash table, linked list, or a simple array.
 *          * Example: device_table[ID] = MAC Address
 * */


int main(void) 
{
    printf("Custom Network Layer\n");
    return 0;
}

void logical_addressing()
{

}

void routing()
{

}

void error_handling()
{

}

void congestion_control()
{

}

void send_to_data_link() 
{

}
