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
  sprintf(dst, "");
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
    sprintf(dst, "r%x = 0x%02x (%03d)", n[1], b[1], b[1]);
  } else if (7 == n[0]) {
    sprintf(dst, "r%x = r%x + 0x%02x (%03d)", n[1], n[1], b[1], b[1]);
  } else if (8 == n[0]) {
    switch (n[3]) {
      case 0x0:
        sprintf(dst, "r%x = r%x", n[1], n[2]);
        break;
      case 0x1:
        sprintf(dst, "r%x = r%x | r%x", n[1], n[1], n[2]);
        break;
      case 0x2:
        sprintf(dst, "r%x = r%x & r%x", n[1], n[1], n[2]);
        break;
      case 0x3:
        sprintf(dst, "r%x = r%x ^ r%x", n[1], n[1], n[2]);
        break;
      case 0x4:
        sprintf(dst, "r%x = r%x + r%x; rf = carry ? 1 : 0", n[1], n[1], n[2]);
        break;
      case 0x5:
        sprintf(dst, "r%x = r%x - r%x; rf = borrow ? 0 : 1", n[1], n[1], n[2]);
        break;
      case 0x6:
        sprintf(dst, "r%x = r%x << 1; rf = shift out", n[1], n[2]);
        break;
      case 0x7:
        sprintf(dst, "r%x = r%x - r%x; rf = borrow ? 0 : 1", n[1], n[2], n[1]);
        break;
      case 0xe:
        sprintf(dst, "r%x = r%x >> 1; rf = shift out", n[1], n[2], n[1]);
        break;
    }
  } else if (0x9 == n[0] && 0x0 == n[3]) {
    sprintf(dst, "skip next if r%x != r%x", n[1], n[2]);
  } else if (0xa == n[0]) {
    sprintf(dst, "I = %03x", address);
  } else if (0xb == n[0]) {
    sprintf(dst, "jump with offset, PC = r0 + %03x", address);
  } else if (0xc == n[0]) {
    sprintf(dst, "r%x = rand() & %02x", n[1], b[1]);
  } else if (0xd == n[0]) {
    sprintf(dst, "draw(r%x, r%x, 0x%02x)", n[1], n[2], n[3]);
  } else if (0xe == n[0]) {
    if (0x9e == b[1]) {
      sprintf(dst, "skip next if key id in r%x pressed", n[1]);
    } else if (0xa1 == b[1]) {
      sprintf(dst, "skip next if key id in r%x not pressed", n[1]);
    }
  } else if (0xf == n[0]) {
    switch (b[1]) {
      case 0x07:
        sprintf(dst, "r%x = delay timer", n[1]);
        break;
      case 0x0a:
        sprintf(dst, "r%x = value of next key pressed (blocking)", n[1]);
        break;
      case 0x15:
        sprintf(dst, "delay timer = r%x", n[1]);
        break;
      case 0x18:
        sprintf(dst, "sound timer = r%x", n[1]);
        break;
      case 0x1e:
        sprintf(dst, "I = I + r%x", n[1]);
        break;
      case 0x29:
        sprintf(dst, "I = character_sprite(%x)", n[1]);
        break;
      case 0x33:
        sprintf(dst, "I[0,2] = BCD(r%x)", n[1]);
        break;
      case 0x55:
        sprintf(dst, "I[0,%x] = r0 - r%x", n[1], n[1]);
        break;
      case 0x65:
        sprintf(dst, "r0 - r%x = I[0,%x]", n[1], n[1]);
        break;
    }
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
