#include <_syslist.h>
#include <errno.h>

#include "internal/stdfd-state.h"

// Remember, all system-level code uses the global variable errno
// See: newlib/libc/reent/reent.c
#undef errno
extern int errno;

int _isatty(int fd) {

  // If the input is not a standard file descriptor, it cannot exist. Similarly,
  // if the input is a standard file descriptor that was previously closed, it
  // does not currently exist. Handle these cases.
  if (!__lc32_is_open_stdfd(fd)) {
    errno = EBADF;
    return 0;
  }

  // All standard file descriptors are TTYs
  return 1;
}
