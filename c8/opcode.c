#include <stdint.h>
#include <c8/opcode.h>


void c8_opcode_unpack(
    c8_opcode* dst,
    uint8_t msb,
    uint8_t lsb) {
  dst->code = (msb << 8) | lsb;
  dst->byte[0] = msb;
  dst->byte[1] = lsb;
  dst->nibble[0] = (msb >> 4) & 0x0f;
  dst->nibble[1] = msb & 0x0f;
  dst->nibble[2] = (lsb >> 4) & 0x0f;
  dst->nibble[3] = lsb & 0x0f;
  dst->address = dst->code & 0x0fff;
}


