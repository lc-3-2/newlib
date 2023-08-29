#include <_syslist.h>
#include <errno.h>
#include <limits.h>
#include <unistd.h>

#include "internal/stdfd-state.h"

// Remember, all system-level code uses the global variable errno
// See: newlib/libc/reent/reent.c
#undef errno
extern int errno;

_ssize_t _write(int fd, const void *buf, size_t cnt) {
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
  // can only write to stdout and stderr. Check this.
  if (fd != STDOUT_FILENO && fd != STDERR_FILENO) {
    errno = EBADF;
    return -1;
  }

  // Write the entire buffer to the console. Cap it at the maximum value for
  // `int` since we may not be able to return more than that.
  const char *b = buf;
  const _ssize_t c = cnt > INT_MAX ? INT_MAX : cnt;
  // Do the write
  for (_ssize_t i = 0; i < c; i++)
    asm volatile ("OUT" : : "a"(b[i]) :);
  // Return successfully
  return c;
}
