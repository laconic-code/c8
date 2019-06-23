#include <string.h>
#include <stdio.h>
#include <c8/error.h>

void c8_print_error(c8_error error) {
  char msg[128];
  switch (error) {
    case C8_ERROR_SUCCESS:
      strcpy(&msg, "no failure");
      break;
    case C8_ERROR_FILE_OPEN:
      strcpy(&msg, "no such file or directory");
      break;
    default:
      strcpy(&msg, "unknown error");
  }
  printf(msg);
}
