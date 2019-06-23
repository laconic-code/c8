#pragma once
#include <stdint.h>

typedef uint8_t c8_error;

enum c8_error {
  C8_ERROR_SUCCESS = 0x00,
  C8_ERROR_FILE_OPEN = 0x01,
};

void c8_print_error(c8_error error);
