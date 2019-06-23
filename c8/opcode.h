#pragma once
#include <stdint.h>

typedef struct c8_opcode c8_opcode;
struct c8_opcode {
    // full 2 byte opcode
    uint16_t code;
    // 12 bit memory address
    uint16_t address;
    // opcode as 2 bytes
    // 0 is msb, 1 is lsb
    uint8_t byte[2];
    // each nibble in the opcode
    // 0 is most significant, 3 is least significant
    uint8_t nibble[4];
};

void c8_opcode_unpack(
    c8_opcode* dst,
    uint8_t msb,
    uint8_t lsb);
