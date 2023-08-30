#include <_syslist.h>
#include <errno.h>
#include <stddef.h>
#include <sys/times.h>

// Remember, all system-level code uses the global variable errno
// See: newlib/libc/reent/reent.c
#undef errno
extern int errno;

// We don't have a way to get the current time, so return failure. This function
// isn't expected to fail though, so be sure to put a sensible value in the
// return parameters.
clock_t _times(struct tms *ptms) {
  // Zero out the result
  if (ptms != NULL) {
    ptms->tms_utime = 0;
    ptms->tms_stime = 0;
    ptms->tms_cutime = 0;
    ptms->tms_cstime = 0;
  }
  // Return failure
  errno = ENOSYS;
  return -1;
}
