#include <_syslist.h>
#include <errno.h>

// We don't have a random number generator in hardware
int getentropy(void *buffer, size_t length) {
  errno = ENOSYS;
  return -1;
}
