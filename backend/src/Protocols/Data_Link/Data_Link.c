/**
 * @file data_link.c
 * @brief Implementation of a simple Data Link Layer 
 *
 * This file simulates a basic Data Link Layer communication mechanism.
 * It includes functions for:
 *  - Framing data for transmission (Start of Frame, Source, Destination, Length, Payload, and Checksum)
 *  - Calculating checksums using CRC for error detection
 *  - Creating frames and sending/receiving them with integrity verification
 *
 * Frame Format:
 * SOF | SEQ_BYTE | LENGTH_BYTE | SEQ_NUM | LENGTH | FLAG | DEST | SRC | PAYLOAD | CHECKSUM
 *
 * @author Ahmad Diab
 * @date 2024
 */

#include "include/data_link.h"
#include "../../utils/memory_utils.c"

/**
 * @brief Main function demonstrating the data link layer functionality
 * 
 * Creates test data and processes it through the data link layer framing system.
 * Uses a large test string to demonstrate frame fragmentation across multiple frames.
 * 
 * @return int Program exit status (0 for success)
 */
int main(void) 
{
    // Example of large data allocation (commented out for current test)
    // size_t large_data_size = 1ULL * 1024 * 1024 + 900;
    // uint8_t *large_data = xmalloc(large_data_size);

    // for (size_t i = 0; i < large_data_size; i++) {
    //     large_data[i] = (uint8_t)((i % 26) + 'A');  // Cycles through A-Z
    // }

    // allocate_payload((uint8_t*)large_data, large_data_size, 1, 2);
    // xfree(large_data);

    // Test data: Large string containing numbered blocks for testing frame fragmentation
    const char big_test_data[] =
        "BLOCK0001_BLOCK0002_BLOCK0003_BLOCK0004_BLOCK0005_BLOCK0006_BLOCK0007_BLOCK0008_BLOCK0009_BLOCK0010_"
        "BLOCK0011_BLOCK0012_BLOCK0013_BLOCK0014_BLOCK0015_BLOCK0016_BLOCK0017_BLOCK0018_BLOCK0019_BLOCK0020_"
        "BLOCK0021_BLOCK0022_BLOCK0023_BLOCK0024_BLOCK0025_BLOCK0026_BLOCK0027_BLOCK0028_BLOCK0029_BLOCK0030_"
        "BLOCK0031_BLOCK0032_BLOCK0033_BLOCK0034_BLOCK0035_BLOCK0036_BLOCK0037_BLOCK0038_BLOCK0039_BLOCK0040_"
        "BLOCK0041_BLOCK0042_BLOCK0043_BLOCK0044_BLOCK0045_BLOCK0046_BLOCK0047_BLOCK0048_BLOCK0049_BLOCK0050_"
        "BLOCK0051_BLOCK0052_BLOCK0053_BLOCK0054_BLOCK0055_BLOCK0056_BLOCK0057_BLOCK0058_BLOCK0059_BLOCK0060_"
        "BLOCK0061_BLOCK0062_BLOCK0063_BLOCK0064_BLOCK0065_BLOCK0066_BLOCK0067_BLOCK0068_BLOCK0069_BLOCK0070_"
        "BLOCK0071_BLOCK0072_BLOCK0073_BLOCK0074_BLOCK0075_BLOCK0076_BLOCK0077_BLOCK0078_BLOCK0079_BLOCK0080_"
        "BLOCK0081_BLOCK0082_BLOCK0083_BLOCK0084_BLOCK0085_BLOCK0086_BLOCK0087_BLOCK0088_BLOCK0089_BLOCK0090_"
        "BLOCK0091_BLOCK0092_BLOCK0093_BLOCK0094_BLOCK0095_BLOCK0096_BLOCK0097_BLOCK0098_BLOCK0099_BLOCK0100_"
        "BLOCK0101_BLOCK0102_BLOCK0103_BLOCK0104_BLOCK0105_BLOCK0106_BLOCK0107_BLOCK0108_BLOCK0109_BLOCK0110_"
        "BLOCK0111_BLOCK0112_BLOCK0113_BLOCK0114_BLOCK0115_BLOCK0116_BLOCK0117_BLOCK0118_BLOCK0119_BLOCK0120_"
        "BLOCK0121_BLOCK0122_BLOCK0123_BLOCK0124_BLOCK0125_BLOCK0126_BLOCK0127_BLOCK0128_BLOCK0129_BLOCK0130_"
        "BLOCK0131_BLOCK0132_BLOCK0133_BLOCK0134_BLOCK0135_BLOCK0136_BLOCK0137_BLOCK0138_BLOCK0139_BLOCK0140_"
        "BLOCK0141_BLOCK0142_BLOCK0143_BLOCK0144_BLOCK0145_BLOCK0146_BLOCK0147_BLOCK0148_BLOCK0149_BLOCK0150_"
        "BLOCK0151_BLOCK0152_BLOCK0153_BLOCK0154_BLOCK0155_BLOCK0156_BLOCK0157_BLOCK0158_BLOCK0159_BLOCK0160_"
        "BLOCK0161_BLOCK0162_BLOCK0163_BLOCK0164_BLOCK0165_BLOCK0166_BLOCK0167_BLOCK0168_BLOCK0169_BLOCK0170_"
        "BLOCK0171_BLOCK0172_BLOCK0173_BLOCK0174_BLOCK0175_BLOCK0176_BLOCK0177_BLOCK0178_BLOCK0179_BLOCK0180_"
        "BLOCK0181_BLOCK0182_BLOCK0183_BLOCK0184_BLOCK0185_BLOCK0186_BLOCK0187_BLOCK0188_BLOCK0189_BLOCK0190_"
        "BLOCK0191_BLOCK0192_BLOCK0193_BLOCK0194_BLOCK0195_BLOCK0196_BLOCK0197_BLOCK0198_BLOCK0199_BLOCK0200_";

    // Process the test data: source=1, destination=2
    allocate_payload((uint8_t*)big_test_data, strlen(big_test_data), 1, 2);

    return 0;
}

/**
 * @brief Calculate CRC checksum for error detection
 * 
 * Implements a 16-bit CRC algorithm using polynomial P for data integrity verification.
 * The CRC is initialized with 0xFFFF and processed bit by bit.
 * 
 * @param data Pointer to the data buffer for checksum calculation
 * @param length Number of bytes to include in checksum calculation
 * @return uint16_t Calculated 16-bit CRC checksum
 * 
 * @note Uses polynomial P (should be defined in header file)
 * @note Standard CRC-16 implementation with MSB-first processing
 */
uint16_t calculate_checksum(const uint8_t *data, uint8_t length) {
    uint16_t crc = 0xffff;  // Initialize CRC with all 1s

    for (int i = 0; i < length; i++) {
        crc ^= (data[i] << 8);  // XOR data byte into high byte of CRC

        // Process each bit of the current byte
        for (int j = 0; j < 8; j++) {
            if (crc & 0x8000) {  // If MSB is set
                crc = (crc << 1) ^ P;  // Shift and XOR with polynomial
            } else {
                crc <<= 1;  // Just shift left
            }
        }
    }
    return crc; 
}

/**
 * @brief Determine the number of bytes needed to represent sequence numbers
 * 
 * Calculates the minimum number of bytes required to store sequence numbers
 * up to the total number of frames. This optimizes frame header size.
 * 
 * @param number_of_frames Total number of frames that will be transmitted
 * @return int Number of bytes needed (1, 2, or 4 bytes)
 * 
 * @note Returns 0 for invalid input (number_of_frames <= 0)
 * @note Uses 1 byte for ≤255, 2 bytes for ≤65535, 4 bytes otherwise
 */
int seq_num_bytes(uint32_t number_of_frames)
{
    if (number_of_frames <= 0) {
        return 0;
    }
    if (number_of_frames <= 255) {
        return 1;  // Can fit in uint8_t
    }
    if (number_of_frames <= 65535) {
        return 2;  // Can fit in uint16_t
    }
    return 4;  // Requires uint32_t
}

/**
 * @brief Create and transmit a data frame
 * 
 * Constructs a complete data frame according to the protocol specification:
 * SOF | SEQ_BYTE | LENGTH_BYTE | SEQ_NUM | LENGTH | FLAG | DEST | SRC | PAYLOAD | CHECKSUM
 * 
 * The function dynamically sizes sequence number and length fields based on requirements,
 * sets appropriate flags for frame position (start/middle/end), and includes error detection.
 * 
 * @param payload Pointer to the payload data to be transmitted
 * @param payload_size Total size of the original payload (for calculating total frames)
 * @param sequence_number Current frame sequence number (0-based)
 * @param chunk_size Size of payload data in this specific frame
 * @param source Source address (8-bit identifier)
 * @param dest Destination address (8-bit identifier)
 * @param checksum Pre-calculated checksum for this frame's payload
 * 
 * @note Automatically determines frame type flags:
 *       - 0xA5: Start frame (sequence_number == 0)
 *       - 0xCC: End frame (sequence_number == number_of_frames - 1)
 *       - 0xFE: Middle frame (all others)
 * @note Calls read_frame() for immediate processing (simulation of transmission/reception)
 * @note Memory is automatically freed after frame processing
 */
void create_frame(const uint8_t *payload, size_t payload_size, uint32_t sequence_number, size_t chunk_size, uint8_t source, uint8_t dest, uint16_t checksum)
{
    static int frame_count = 0;
    frame_count++;
    
    // Calculate total number of frames needed for the complete payload
    uint32_t number_of_frames = (payload_size + MAX_PAYLOAD_SIZE - 1) / MAX_PAYLOAD_SIZE;
    
    // Determine optimal byte count for sequence numbers
    int bytes_seq = seq_num_bytes(number_of_frames);
    
    // Determine what length value to store
    uint32_t length_field;
    if(sequence_number == 0) {
        length_field = (uint32_t)payload_size;  // First frame stores total payload size
    } else {
        length_field = (uint32_t)chunk_size;    // Other frames store chunk size
    }
    
    // Determine optimal byte count for length field
    int bytes_length;
    if(length_field <= 0xFF)
        bytes_length = 1;      // uint8_t
    else if(length_field <= 0xFFFF)
        bytes_length = 2;      // uint16_t
    else if(length_field <= 0xFFFFFF)
        bytes_length = 4;      // uint32_t

    // Calculate field positions within the frame
    size_t length_pos = SEQ_INDEX + bytes_seq;
    size_t flag_pos = length_pos + bytes_length;
    size_t dest_pos = flag_pos + 1;
    size_t src_pos = dest_pos + 1;
    size_t payload_pos = src_pos + 1;
    size_t checksum_pos = payload_pos + chunk_size;
    
    // Calculate total frame size and allocate memory
    size_t frame_size = checksum_pos + sizeof(checksum);
    uint8_t* frame = xmalloc(frame_size);
    
    // Build frame header
    frame[SOF_INDEX] = SOF;              // Start of Frame marker
    frame[SEQ_BYTE] = bytes_seq;         // Bytes used for sequence number
    frame[LENGTH_BYTE] = bytes_length;   // Bytes used for length field
    
    // Copy variable-length sequence number
    memcpy(&frame[SEQ_INDEX], &sequence_number, bytes_seq);

    // Copy variable-length length field
    memcpy(&frame[length_pos], &length_field, bytes_length);
    
    // TODO: Find actual flags for the frame
    // Set frame type flag based on sequence position
    if(sequence_number == 0) {
        frame[flag_pos] = 0xA5; // Start frame flag
    } else if(sequence_number == number_of_frames - 1) {
        frame[flag_pos] = 0xCC; // End frame flag
    } else {
        frame[flag_pos] = 0xFE; // Middle frame flag
    }

    // Set addressing information
    frame[dest_pos] = dest;
    frame[src_pos] = source;
    
    // Copy payload data
    memcpy(&frame[payload_pos], payload, chunk_size);
    
    // Copy checksum for error detection
    memcpy(&frame[checksum_pos], &checksum, sizeof(checksum));
    
    // Simulate frame transmission and reception
    read_frame(frame, frame_size, number_of_frames);
    
    // Clean up allocated memory
    xfree(frame);
}

/**
 * @brief Fragment payload into frames and initiate transmission
 * 
 * Takes a large payload and breaks it into manageable chunks that fit within
 * the maximum payload size constraint. Each chunk is processed into a separate
 * frame with appropriate sequencing and checksums.
 * 
 * @param payload Pointer to the complete payload data to be transmitted
 * @param payload_size Total size of the payload in bytes
 * @param source Source address for all frames
 * @param dest Destination address for all frames
 * 
 * @note Automatically handles payload fragmentation based on MAX_PAYLOAD_SIZE
 * @note Generates sequential frame numbers starting from 0
 * @note Calculates individual checksums for each frame's payload chunk
 * @note Last frame may contain less data than MAX_PAYLOAD_SIZE
 */
void allocate_payload(uint8_t *payload, size_t payload_size, uint8_t source, uint8_t dest) 
{
    size_t offset = 0;               // Current position in payload
    uint8_t *chunk = 0;              // Pointer to current chunk
    size_t chunk_size = 0;           // Size of current chunk
    uint32_t sequence_number = 0;    // Frame sequence counter
    
    // Process payload in chunks until complete
    while (payload_size > offset)
    {
        // Determine chunk size for this frame
        if(payload_size - offset > MAX_PAYLOAD_SIZE) {
            chunk_size = MAX_PAYLOAD_SIZE;  // Full-size chunk
            chunk = &payload[offset];
        } else {
            chunk_size = payload_size - offset;  // Final partial chunk
            chunk = &payload[offset];
        }
        
        // Move to next chunk position
        offset += chunk_size;
        
        // Calculate checksum for this chunk
        uint16_t checksum = calculate_checksum(chunk, chunk_size);
        
        // Create and transmit the frame
        create_frame(chunk, payload_size, sequence_number, chunk_size, source, dest, checksum);
        
        // Increment sequence number for next frame
        sequence_number++;
    }
}

/**
 * @brief Parse and display received frame contents
 * 
 * Decodes a received frame according to the protocol specification and displays
 * all header fields and frame information. This function operates at the Data Link
 * Layer and focuses on frame parsing rather than data integrity verification.
 * 
 * Frame Format: SOF | SEQ_BYTE | LENGTH_BYTE | SEQ_NUM | LENGTH | FLAG | DEST | SRC | PAYLOAD | CHECKSUM
 * 
 * @param frame Pointer to the received frame buffer
 * @param frame_size Total size of the frame in bytes
 * @param number_of_frames Total expected number of frames in the transmission
 * 
 * @note This function simulates frame reception and parsing
 * @note Memory for payload is allocated and freed within the function
 * @note Outputs detailed frame information for debugging/monitoring
 * @note Does not perform checksum verification (handled by higher layers)
 * 
 * @warning Current implementation has a bug in length handling - see FIXME comments
 */
void read_frame(uint8_t *frame, size_t frame_size, size_t number_of_frames)
{
    static size_t frame_count = 0;
    frame_count++;
    
    // Extract Start of Frame marker
    size_t sof = 0;
    memcpy(&sof, &frame[SOF_INDEX], 1);
    printf("Start Of Frame (SOF): 0x%02X\n", (unsigned int)sof & 0xFF);
    
    // Extract sequence number field size
    size_t seq_bytes_pos = 0;
    memcpy(&seq_bytes_pos, &frame[SEQ_BYTE], 1);
    printf("Bytes needed to represent seq num: %zu \n", seq_bytes_pos);
    
    // Extract length field size
    size_t len_bytes = 0;
    memcpy(&len_bytes, &frame[LENGTH_BYTE], 1);
    printf("Bytes needed to represent the length: %zu \n", len_bytes);
    
    // Extract sequence number
    size_t seq_num = 0;
    memcpy(&seq_num, &frame[SEQ_INDEX], seq_bytes_pos);
    printf("Seq Number: %zu \n", seq_num);
    
    // Calculate length field position
    size_t length_pos = SEQ_INDEX + seq_bytes_pos;
    size_t length = 0;  // All frames except the first and last frame will have a length of 1024.
                           // The first will contain the length of the entire payload.
                           // The last might not fill the entire 1024 could be less.
    
    size_t last_payload_len = 0;
    memcpy(&length, &frame[length_pos], len_bytes);
    printf("Length: %zu\n", length);
    size_t payload_len = 0;
    if (seq_num == number_of_frames) {
        memcpy(&last_payload_len, &frame[length_pos], len_bytes);
        printf("Payload Length: %zu \n", last_payload_len);
    } else if(seq_num == 0) {
        memcpy(&payload_len, &frame[length_pos], len_bytes);
        printf("Payload Length: %zu\n", payload_len);
    }

    size_t full_payload_len = length + last_payload_len;

    printf("Full payload length: %zu\n", full_payload_len);

    
    // Extract frame control flag
    size_t flag_pos = length_pos + len_bytes;
    size_t flag = 0;
    memcpy(&flag, &frame[flag_pos], 1);
    printf("Flag: 0x%02X\n", (unsigned int)flag & 0xFF);
    
    // Extract destination address
    size_t dest_pos = flag_pos + 1;
    size_t dest = 0;
    memcpy(&dest, &frame[dest_pos], 1);
    printf("Destination: %zu \n", dest);
    
    // Extract source address
    size_t src_pos = dest_pos + 1;
    size_t src = 0;
    memcpy(&src, &frame[src_pos], 1);
    printf("source: %zu \n", src);
    
    // Extract payload data
    size_t payload_pos = src_pos + 1;
    uint8_t* payload = xmalloc(length);  
    memcpy(payload, &frame[payload_pos], length);
    
    // Extract checksum
    size_t checksum_pos = payload_pos + length;
    size_t checksum = 0;
    memcpy(&checksum, &frame[checksum_pos], 2);
    printf("Checksum: 0x%04X \n", (unsigned int)checksum & 0xFFFF);
    
    // Display frame summary
    printf("Frame Size = %zu \n", frame_size);
    printf("======= Frame Received %zu =======\n", frame_count);
    
    // Hex dump of entire frame
    for(size_t i = 0; i < frame_size; i++)
        printf("%02X ", frame[i]);
    printf("\n");
    printf("======= Frame End =======\n");
    
    // Clean up allocated payload memory
    xfree(payload);
}

// TODO: Add another function to send the frame to physical layer.
void send_to_physical_layer()
{

}

// TODO: MAC Addressing, instead of the 1 byte for destination and source, it should have 48-bit for mac addresses.
// TODO: Re-transmission Logic; if a packet is lost need some kind of ACK/NACK (Acknowledgment/Negative Acknowledgment) mechanism to resend the packet.
// TODO: Change the fixed MAX_PAYLOAD_SIZE to be dynamic and determined between nodes when communicating.
