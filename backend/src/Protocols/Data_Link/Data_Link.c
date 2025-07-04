#include "include/data_link.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#define HEADER_SIZE 6
#define FIRST_HEADER_SIZE 8


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

int seq_num_bytes(size_t payload_size, uint32_t number_of_frames)
{
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
    uint32_t number_of_frames = (payload_size + MAX_PAYLOAD_SIZE - 1) / MAX_PAYLOAD_SIZE;
    int bytes_seq = seq_num_bytes(payload_size, number_of_frames);
    size_t frame_size = HEADER_SIZE + bytes_seq + chunk_size;
    uint8_t *frame = malloc(frame_size);
    if(frame == NULL) {
        perror("Error allocating memory for frame");
        return;
    }

    frame[SOF_INDEX] = SOF;
    frame[SEQ_BYTE] = bytes_seq;
    memcpy(&frame[SEQ_INDEX], &sequence_number, sizeof(bytes_seq));
    if(sequence_number == 0)
    {
        frame[FLAG_INDEX] = 0xA5; // TODO: find a start flag 
        memcpy(&frame[LENGTH_INDEX], &payload_size, sizeof(payload_size));
        // frame[LENGTH_INDEX] = payload_size;
    } else if(sequence_number == number_of_frames) {
        frame[FLAG_INDEX] = 0x7F; // TODO: an end flag
        memcpy(&frame[LENGTH_INDEX], &chunk_size, sizeof(chunk_size));
    } else {
        frame[FLAG_INDEX] = 0xFE; // TODO: find a middle flag
        memcpy(&frame[LENGTH_INDEX], &chunk_size, sizeof(chunk_size));
        // frame[LENGTH_INDEX] = chunk_size;
    }

    frame[LENGTH_INDEX] = chunk_size;
    frame[DEST_INDEX] = 1;
    frame[SRC_INDEX] = 2;
    memcpy(&frame[PAYLOAD], payload,  chunk_size);
    memcpy(&frame[CHECKSUM_INDEX], &checksum, sizeof(checksum));

    printf("\n======= FRAME IN TEXT  =======\n");

    printf("SOF: 0x%02X\n", frame[SOF_INDEX]);
    printf("FLAG: 0x%02X\n", frame[FLAG_INDEX]);
    printf("DEST: %u\n", frame[DEST_INDEX]);
    printf("SRC: %u\n", frame[SRC_INDEX]);
    printf("SEQ BYTE LENGTH: %u\n", frame[SEQ_BYTE]);
    printf("SEQUENCE NUMBER: ");
    for(int i = 0; i < frame[SEQ_BYTE]; i++) {
        printf("%02X ", frame[SEQ_INDEX + i]);
    }
    printf("\n");
    printf("PAYLOAD (as text): \"");
    fwrite(&frame[PAYLOAD], 1, chunk_size, stdout);
    printf("\"\n");
    printf("CHECKSUM: ");
    for(int i = 0; i < sizeof(checksum); i++) {
        printf("%02X ", frame[CHECKSUM_INDEX + i]);
    }
    printf("\n");
    printf("========= END OF FRAME =======\n");
    printf("\n");
    printf("======= HEX DUMP FOR FRAME =======\n");
    for(size_t i = 0; i < frame_size; i++)
        printf("%02X ", frame[i]);
    printf("\n");
    printf("======= END OF HEX DUMP =======\n");

    free(frame);
}

void allocate_payload(uint8_t *payload, size_t payload_size) 
{
    int offset = 0;
    uint8_t *chunk = 0;
    size_t chunk_size = 0;
    uint32_t sequence_number = 0;
    while (payload_size > offset)
    {
        if(payload_size - offset > MAX_PAYLOAD_SIZE) {
            chunk_size = MAX_PAYLOAD_SIZE;
            chunk = &payload[offset];
        } else {
            chunk_size = payload_size - offset;
            chunk = &payload[offset];
        }
        offset += chunk_size;
        uint16_t checksum = calculate_checksum(chunk, chunk_size);
        create_frame(chunk, payload_size, sequence_number, chunk_size, checksum);
        sequence_number++;
    }
}

int main(void) 
{
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

    size_t size_data = strlen(big_test_data);
    printf("Size of data: %zu\n", size_data);
    allocate_payload((uint8_t*)big_test_data, strlen(big_test_data));

    return 0;
}
