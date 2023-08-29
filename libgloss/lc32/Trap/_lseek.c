#include <_syslist.h>
#include <errno.h>

#include "internal/stdfd-state.h"

// Remember, all system-level code uses the global variable errno
// See: newlib/libc/reent/reent.c
#undef errno
extern int errno;

_off_t _lseek(int fd, _off_t pos, int whence) {

  // This is going to fail. The question is how - it depends on whether the file
  // descriptor exists. If it doesn't exist, it fails with EBADF. If it does, it
  // has to be the console, so it fails with ESPIPE.
  if (__lc32_is_open_stdfd(fd))
    errno = ESPIPE;
  else
    errno = EBADF;

  return -1;
}
