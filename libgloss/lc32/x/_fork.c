#include <_syslist.h>
#include <errno.h>

// Remember, all system-level code uses the global variable errno
// See: newlib/libc/reent/reent.c
#undef errno
extern int errno;

// No process management
int _fork(void) {
  errno = ENOSYS;
  return -1;
}
