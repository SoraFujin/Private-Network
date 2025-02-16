#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    uint16_t source;
    uint16_t dest;
    uint8_t connect_req;
    uint8_t connect_acc;
    uint8_t final_acknow;
    uint8_t data_flg;
    uint16_t seq;
    uint16_t rec_seq;
} toji;

int main()
{

    return 0;
}

