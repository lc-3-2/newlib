#include <errno.h>

// Remember, all system-level code uses the global variable errno
// See: newlib/libc/reent/reent.c
#undef errno
extern int errno;

// The init process cannot fork, and thus cannot have any child processes
int _wait(int *status) {
  errno = ECHILD;
  return -1;
}
