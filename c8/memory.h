#pragma once
#include <stdint.h>

#define C8_MEM_SIZE 4096
#define C8_REGISTERS 8
#define C8_START_ADDR 0x200

typedef struct c8_memory c8_memory;
struct c8_memory {
  uint8_t memory[C8_MEM_SIZE];
  uint8_t gpRegisters[C8_REGISTERS];
  uint16_t addressRegister;
  uint16_t programCounter;
  uint8_t stackPointer;
};

void c8_memory_init(c8_memory* memory);
c8_error c8_memory_load(c8_memory* memory, char* src);
