#include <errno.h>
#include <signal.h>

#include "internal/sigset-state.h"

// Remember, all system-level code uses the global variable errno
// See: newlib/libc/reent/reent.c
#undef errno
extern int errno;

int _kill(int pid, int sig) {

  // Check whether we're trying to send a signal to a process that doesn't
  // exist. We only have the init process.
  if (!(pid == 1 || pid == 0 || pid == -1)) {
    errno = ESRCH;
    return -1;
  }

  // We tried to send a signal to PID 1. But, init only handles signals that
  // have been explicitly registered. Trying to send any other signal fails.
  errno = EPERM;
  return -1;
}
