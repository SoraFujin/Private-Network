#include "include/data_link.h"

int main(void) 
{
    size_t large_data_size = 1ULL * 1024 * 1024 * 1024 + 900;
    uint8_t *large_data = malloc(large_data_size);
    if (large_data == NULL) {
        perror("Failed to allocate test data");
        return 1;
    }

    for (size_t i = 0; i < large_data_size; i++) {
        large_data[i] = (uint8_t)((i % 26) + 'A');  // Cycles through A-Z
    }

    allocate_payload((uint8_t*)large_data, large_data_size);

    // const char big_test_data[] =
    //     "BLOCK0001_BLOCK0002_BLOCK0003_BLOCK0004_BLOCK0005_BLOCK0006_BLOCK0007_BLOCK0008_BLOCK0009_BLOCK0010_"
    //     "BLOCK0011_BLOCK0012_BLOCK0013_BLOCK0014_BLOCK0015_BLOCK0016_BLOCK0017_BLOCK0018_BLOCK0019_BLOCK0020_"
    //     "BLOCK0021_BLOCK0022_BLOCK0023_BLOCK0024_BLOCK0025_BLOCK0026_BLOCK0027_BLOCK0028_BLOCK0029_BLOCK0030_"
    //     "BLOCK0031_BLOCK0032_BLOCK0033_BLOCK0034_BLOCK0035_BLOCK0036_BLOCK0037_BLOCK0038_BLOCK0039_BLOCK0040_"
    //     "BLOCK0041_BLOCK0042_BLOCK0043_BLOCK0044_BLOCK0045_BLOCK0046_BLOCK0047_BLOCK0048_BLOCK0049_BLOCK0050_"
    //     "BLOCK0051_BLOCK0052_BLOCK0053_BLOCK0054_BLOCK0055_BLOCK0056_BLOCK0057_BLOCK0058_BLOCK0059_BLOCK0060_"
    //     "BLOCK0061_BLOCK0062_BLOCK0063_BLOCK0064_BLOCK0065_BLOCK0066_BLOCK0067_BLOCK0068_BLOCK0069_BLOCK0070_"
    //     "BLOCK0071_BLOCK0072_BLOCK0073_BLOCK0074_BLOCK0075_BLOCK0076_BLOCK0077_BLOCK0078_BLOCK0079_BLOCK0080_"
    //     "BLOCK0081_BLOCK0082_BLOCK0083_BLOCK0084_BLOCK0085_BLOCK0086_BLOCK0087_BLOCK0088_BLOCK0089_BLOCK0090_"
    //     "BLOCK0091_BLOCK0092_BLOCK0093_BLOCK0094_BLOCK0095_BLOCK0096_BLOCK0097_BLOCK0098_BLOCK0099_BLOCK0100_"
    //     "BLOCK0101_BLOCK0102_BLOCK0103_BLOCK0104_BLOCK0105_BLOCK0106_BLOCK0107_BLOCK0108_BLOCK0109_BLOCK0110_"
    //     "BLOCK0111_BLOCK0112_BLOCK0113_BLOCK0114_BLOCK0115_BLOCK0116_BLOCK0117_BLOCK0118_BLOCK0119_BLOCK0120_"
    //     "BLOCK0121_BLOCK0122_BLOCK0123_BLOCK0124_BLOCK0125_BLOCK0126_BLOCK0127_BLOCK0128_BLOCK0129_BLOCK0130_"
    //     "BLOCK0131_BLOCK0132_BLOCK0133_BLOCK0134_BLOCK0135_BLOCK0136_BLOCK0137_BLOCK0138_BLOCK0139_BLOCK0140_"
    //     "BLOCK0141_BLOCK0142_BLOCK0143_BLOCK0144_BLOCK0145_BLOCK0146_BLOCK0147_BLOCK0148_BLOCK0149_BLOCK0150_"
    //     "BLOCK0151_BLOCK0152_BLOCK0153_BLOCK0154_BLOCK0155_BLOCK0156_BLOCK0157_BLOCK0158_BLOCK0159_BLOCK0160_"
    //     "BLOCK0161_BLOCK0162_BLOCK0163_BLOCK0164_BLOCK0165_BLOCK0166_BLOCK0167_BLOCK0168_BLOCK0169_BLOCK0170_"
    //     "BLOCK0171_BLOCK0172_BLOCK0173_BLOCK0174_BLOCK0175_BLOCK0176_BLOCK0177_BLOCK0178_BLOCK0179_BLOCK0180_"
    //     "BLOCK0181_BLOCK0182_BLOCK0183_BLOCK0184_BLOCK0185_BLOCK0186_BLOCK0187_BLOCK0188_BLOCK0189_BLOCK0190_"
    //     "BLOCK0191_BLOCK0192_BLOCK0193_BLOCK0194_BLOCK0195_BLOCK0196_BLOCK0197_BLOCK0198_BLOCK0199_BLOCK0200_";

    // size_t size_data = strlen(big_test_data);
    // printf("Size of data: %zu\n", size_data);
    // allocate_payload((uint8_t*)big_test_data, strlen(big_test_data));

    return 0;
}

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
    
    uint32_t length_field;
    if(sequence_number == 0) {
        length_field = (uint32_t)payload_size;
    } else {
        length_field = (uint32_t)chunk_size;
    }
    
    int bytes_length;
    if(length_field <= 0xFF)
        bytes_length = 1;
    else if(length_field <= 0xFFFF)
        bytes_length = 2;
    else if(length_field <= 0xFFFFFF)
        bytes_length = 3;
    else 
        bytes_length = 4;
    
    size_t length_pos = SEQ_INDEX + bytes_seq;
    size_t flag_pos = length_pos + bytes_length;
    size_t dest_pos = flag_pos + 1;
    size_t src_pos = dest_pos + 1;
    size_t payload_pos = src_pos + 1;
    size_t checksum_pos = payload_pos + chunk_size;
    
    size_t frame_size = checksum_pos + sizeof(checksum);
    uint8_t *frame = malloc(frame_size);
    if(frame == NULL) {
        perror("Error allocating memory for frame");
        return;
    }
    
    frame[SOF_INDEX] = SOF;
    frame[SEQ_BYTE] = bytes_seq;
    frame[LENGTH_BYTE] = bytes_length;
    
    memcpy(&frame[SEQ_INDEX], &sequence_number, bytes_seq);

    memcpy(&frame[length_pos], &length_field, bytes_length);
    
    if(sequence_number == 0) {
        frame[flag_pos] = 0xA5;
    } else if(sequence_number == number_of_frames - 1) {
        frame[flag_pos] = 0xCC;
    } else {
        frame[flag_pos] = 0xFE;
    }

    frame[dest_pos] = 1;
    frame[src_pos] = 2;
    memcpy(&frame[payload_pos], payload, chunk_size);
    memcpy(&frame[checksum_pos], &checksum, sizeof(checksum));
    
    printf("\n======= FRAME IN TEXT =======\n");
    printf("SOF: 0x%02X\n", frame[SOF_INDEX]);
    printf("SEQ BYTE LENGTH: %u\n", frame[SEQ_BYTE]);
    printf("LENGTH BYTE COUNT: %u\n", frame[LENGTH_BYTE]);
    printf("SEQUENCE NUMBER: ");
    for(int i = 0; i < bytes_seq; i++) {
        printf("%02X ", frame[SEQ_INDEX + i]);
    }
    printf("\n");
    printf("LENGTH: %u\n", length_field);
    printf("FLAG: 0x%02X\n", frame[flag_pos]);
    printf("DEST: %u\n", frame[dest_pos]);
    printf("SRC: %u\n", frame[src_pos]);
    printf("PAYLOAD (as text): \"");
    fwrite(&frame[payload_pos], 1, chunk_size, stdout);
    printf("\"\n");
    printf("CHECKSUM: ");
    for(int i = 0; i < sizeof(checksum); i++) {
        printf("%02X ", frame[checksum_pos + i]);
    }
    printf("\n");
    printf("========= END OF FRAME =======\n");
    printf("\n");
    printf("======= HEX DUMP FOR FRAME =======\n");
    for(size_t i = 0; i < frame_size; i++)
        printf("%02X ", frame[i]);
    printf("\n");
    printf("Frame size: %zu bytes\n", frame_size);
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

