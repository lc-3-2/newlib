#include <_syslist.h>
#include <errno.h>
#include <unistd.h>

#include "internal/fsstate.h"

// Remember, all system-level code uses the global variable errno
// See: newlib/libc/reent/reent.c
#undef errno
extern int errno;

int _isatty(int fd) {

  // Check the "standard" file descriptors. Make sure they're actually open
  // first.
  if ((fd == STDIN_FILENO && !__lc32Trap_stdin_closed) ||
      (fd == STDOUT_FILENO && !__lc32Trap_stdout_closed) ||
      (fd == STDERR_FILENO && !__lc32Trap_stderr_closed))
    return 1;

  // It either wasn't one of the standard descriptors, or the standard
  // descriptor was closed. Either way, it's a bad file descriptor.
  errno = EBADF;
  return 0;
}
