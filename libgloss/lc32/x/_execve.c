#include <_syslist.h>
#include <errno.h>

// Remember, all system-level code uses the global variable errno
// See: newlib/libc/reent/reent.c
#undef errno
extern int errno;

// We don't have a filesystem, and we don't have a kernel either
int _execve(const char *name, const char *const argv[],
            const char *const env[]) {
  errno = ENOSYS;
  return -1;
}
