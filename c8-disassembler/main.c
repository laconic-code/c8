#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>
#include <rxi/log.h>
#include <c8/c8.h>

void cmd(c8_opcode* opcode, char* dst) {
  uint16_t code = opcode->code;
  uint16_t address = opcode->address;
  uint8_t* n = opcode->nibble;
  uint8_t* b = opcode->byte;
  if (0x00e0 == code) {
    sprintf(dst, "clear screen");
  } else if (0x00ee == code) {
    sprintf(dst, "return");
  } else if (0 == n[0]) {
    sprintf(dst, "call rca 1802 at 0x%03x", address);
  } else if (1 == n[0]) {
    sprintf(dst, "jump to 0x%03x", address);
  } else if (2 == n[0]) {
    sprintf(dst, "call subroutine at 0x%03x", address);
  } else if (3 == n[0]) {
    sprintf(dst, "skip next if r%x == 0x%02x (%03d)", n[1], b[1], b[1]);
  } else if (4 == n[0]) {
    sprintf(dst, "skip next if r%x != 0x%02x (%03d)", n[1], b[1], b[1]);
  } else if (5 == n[0] && 0 == n[3]) {
    sprintf(dst, "skip next if r%x != r%x", n[1], n[2]);
  } else if (6 == n[0]) {
    sprintf(dst, "set r%x to 0x%02x (%03d)", n[1], b[1], b[1]);
  } else if (7 == n[0]) {
    sprintf(dst, "add 0x%02x (%03d) to r%x", b[1], b[1], n[1]);
  } else {
    sprintf(dst, "not implemented");
  }
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    printf("Usage: %s <c8 binary path>", argv[0]);
    exit(-1);
  }
  char* srcPath = argv[1];

  log_set_level(LOG_DEBUG);

  c8_memory memory;
  c8_memory_init(&memory);
  c8_memory_load(&memory, srcPath);

  c8_opcode opcode;
  char human[256];
  printf("ADDR   INSTR\n");
  for (int addr = C8_START_ADDR; addr <= C8_MEM_SIZE; addr += 2) {
    c8_opcode_unpack(&opcode, memory.memory[addr], memory.memory[addr + 1]);
    cmd(&opcode, &human);
    printf("0x%03x  %04x  %s\n", addr, opcode.code & 0xffff, human);
  }

  return 0;
}
