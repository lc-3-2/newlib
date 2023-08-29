#include <_syslist.h>
#include <errno.h>
#include <unistd.h>

#include "internal/stdfd-state.h"

// Remember, all system-level code uses the global variable errno
// See: newlib/libc/reent/reent.c
#undef errno
extern int errno;

// Note that this function can close the standard file descriptors. They are
// made unusable after that, and attempting to do so should result in EBADF.
int _close(int fd) {

  // If the input is not a standard file descriptor, it cannot exist. Similarly,
  // if the input is a standard file descriptor that was previously closed, it
  // does not currently exist. Handle these cases.
  if (!__lc32_is_open_stdfd(fd)) {
    errno = EBADF;
    return -1;
  }

  // The input is a currently open standard file descriptor. Figure out which
  // one it is and mark it as closed.
  switch (fd) {
  case STDIN_FILENO:
    __lc32_stdin_closed = true;
    break;
  case STDOUT_FILENO:
    __lc32_stdout_closed = true;
    break;
  case STDERR_FILENO:
    __lc32_stderr_closed = true;
    break;
  }
  // Return success
  return 0;
}
