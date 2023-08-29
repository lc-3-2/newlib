#include <_syslist.h>
#include <errno.h>
#include <sys/stat.h>

// Remember, all system-level code uses the global variable errno
// See: newlib/libc/reent/reent.c
#undef errno
extern int errno;

// We don't have a filesystem, so fail on using paths
int _stat(const char *file, struct stat *pstat) {
  errno = ENOSYS;
  return -1;
}
