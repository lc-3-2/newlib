#include <errno.h>
#include <signal.h>

// Remember, all system-level code uses the global variable errno
// See: newlib/libc/reent/reent.c
#undef errno
extern int errno;

// It doesn't really make sense for us to block signals, since we don't have an
// operating system. So, we just return an error, setting errno to something.
//
// Since we aren't using the posix subsystem, none of the library code actually
// cares about the result.
int sigprocmask(int how, const sigset_t *restrict set,
                sigset_t *restrict oset) {
  errno = ENOSYS;
  return -1;
}
