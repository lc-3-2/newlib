#include <_syslist.h>
#include <errno.h>
#include <sys/stat.h>

// Remember, all system-level code uses the global variable errno
// See: newlib/libc/reent/reent.c
#undef errno
extern int errno;

// We can't open any files, and we don't start with any file descriptors.
// Therefore, any file descriptor passed will be invalid.
int _fstat(int fd, struct stat *pstat) {
  errno = EBADF;
  return -1;
}
