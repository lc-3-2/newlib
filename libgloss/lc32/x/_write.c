#include <_syslist.h>
#include <errno.h>

// Remember, all system-level code uses the global variable errno
// See: newlib/libc/reent/reent.c
#undef errno
extern int errno;

// We can't open any files, and we don't start with any file descriptors.
// Therefore, any file descriptor passed will be invalid.
_ssize_t _write(int fd, const void *buf, size_t cnt) {
  // Early check for zero - do nothing in that case
  if (cnt == 0)
    return 0;

  errno = EBADF;
  return -1;
}
