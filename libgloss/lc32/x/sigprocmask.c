#include <errno.h>
#include <signal.h>
#include <stddef.h>

#include "internal/sigmask-state.h"

// Remember, all system-level code uses the global variable errno
// See: newlib/libc/reent/reent.c
#undef errno
extern int errno;

int sigprocmask(int how, const sigset_t *restrict set,
                sigset_t *restrict oset) {

  // Store the old value if we have to
  if (oset != NULL)
    *oset = __lc32_sigmask;

  // Do nothing if the new set is NULL
  if (set == NULL)
    return 0;

  // Do what the `how` parameter tells us to do
  switch (how) {
  case SIG_BLOCK:
    __lc32_sigmask |= *set;
    return 0;
  case SIG_UNBLOCK:
    __lc32_sigmask &= ~(*set);
    return 0;
  case SIG_SETMASK:
    __lc32_sigmask = *set;
    return 0;
  default:
    errno = EINVAL;
    return -1;
  }
}
