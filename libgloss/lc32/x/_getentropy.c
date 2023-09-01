#include <_syslist.h>
#include <errno.h>

// Remember, all system-level code uses the global variable errno
// See: newlib/libc/reent/reent.c
#undef errno
extern int errno;

// We don't have a random number generator in hardware
int _getentropy(void *buffer, size_t length) {
  errno = ENOSYS;
  return -1;
}
