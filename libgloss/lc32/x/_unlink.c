#include <_syslist.h>
#include <errno.h>

// Remember, all system-level code uses the global variable errno
// See: newlib/libc/reent/reent.c
#undef errno
extern int errno;

// We don't have a filesystem, so fail on removing files
int _unlink(const char *file) {
  errno = ENOSYS;
  return -1;
}
