#include <_syslist.h>
#include <errno.h>

// Remember, all system-level code uses the global variable errno
// See: newlib/libc/reent/reent.c
#undef errno
extern int errno;

int _kill(int pid, int sig) {

  // Check whether we're trying to send a signal to a process that doesn't
  // exist. We only have the init process.
  if (!(pid == 1 || pid == 0)) {
    errno = ESRCH;
    return -1;
  }

  // We tried to send a signal to PID 1. But, init only handles signals that
  // have been explicitly registered. Trying to send any other signal fails.
  //
  // FIXME: We don't actually check if we registered a signal handler for the
  // incoming signal. Therefore, this code only works when called from `raise`,
  // which does the checking for us. For now, use `raise` instead of `kill`.
  errno = EPERM;
  return -1;
}
