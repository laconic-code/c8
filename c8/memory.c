#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <rxi/log.h>
#include <c8/error.h>
#include <c8/memory.h>

void c8_memory_init(c8_memory* memory) {
  memset(memory->memory, 0, C8_MEM_SIZE);
  memset(memory->gpRegisters, 0, C8_REGISTERS);
  memory->addressRegister = 0;
  memory->programCounter = C8_START_ADDR;
  memory->stackPointer = 0;
}

c8_error c8_memory_load(c8_memory* memory, char* path) {
  FILE* src = fopen(path, "r");
  if (src == -1) {
    char* msg = strerror(errno);
    log_error("failed to open file %s\n", path);
    log_error(msg);
    return C8_ERROR_FILE_OPEN;
  }

  uint8_t* start = memory->memory + C8_START_ADDR;
  uint16_t bytes = C8_MEM_SIZE - C8_START_ADDR;
  int read = fread(start, 1, bytes, src);
  fclose(src);

  log_info("loaded %d bytes from source %s", read, path);
  return C8_ERROR_SUCCESS;
}
