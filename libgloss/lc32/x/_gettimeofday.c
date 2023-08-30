#include <_syslist.h>
#include <errno.h>
#include <stddef.h>
#include <sys/time.h>

// Remember, all system-level code uses the global variable errno
// See: newlib/libc/reent/reent.c
#undef errno
extern int errno;

// We don't have a way to get the current time, so return failure. This function
// isn't expected to fail though, so be sure to put a sensible value in the
// return parameters.
int _gettimeofday(struct timeval *ptimeval, void *ptimezone) {
  // Zero out the result
  if (ptimeval != NULL) {
    ptimeval->tv_sec = 0;
    ptimeval->tv_usec = 0;
  }
  // Return failure
  errno = ENOSYS;
  return -1;
}
