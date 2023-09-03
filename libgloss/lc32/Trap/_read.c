#include <_syslist.h>
#include <errno.h>
#include <unistd.h>

#include "internal/stdfd-state.h"

// Remember, all system-level code uses the global variable errno
// See: newlib/libc/reent/reent.c
#undef errno
extern int errno;

_ssize_t _read(int fd, void *buf, size_t cnt) {
  // Early check for zero - do nothing in that case
  if (cnt == 0)
    return 0;

  // If the input is not a standard file descriptor, it cannot exist. Similarly,
  // if the input is a standard file descriptor that was previously closed, it
  // does not currently exist. Handle these cases.
  if (!__lc32_is_open_stdfd(fd)) {
    errno = EBADF;
    return -1;
  }

  // The input is a standard file descriptor that is currently open. However, we
  // can only read from stdin. Check this.
  if (fd != STDIN_FILENO) {
    errno = EBADF;
    return -1;
  }

  // NULL argument checking
  if (buf == NULL) {
    errno = EFAULT;
    return -1;
  }

  // We don't have a way to read from the console without blocking. Therefore,
  // we're forced to read one at most one byte at a time.
  char *b = buf;
  asm volatile ("GETC" : "=a"(*b) : :);
  return 1;
}
