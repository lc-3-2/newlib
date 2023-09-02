#include <_syslist.h>
#include <errno.h>
#include <stdint.h>

// Remember, all system-level code uses the global variable errno
// See: newlib/libc/reent/reent.c
#undef errno
extern int errno;

// Address of the RNG register. Set by the linker script.
volatile uint32_t __lc32_mmio_rng;

int _getentropy(void *buffer, size_t length) {

  // Error checking
  if (buffer == NULL) {
    errno = EFAULT;
    return -1;
  }

  // Convenience variables - copies of the parameters we modify
  char *buf = buffer;
  size_t to_go = length;

  // Copy one byte at a time. This way, we don't have to worry about alignment
  //
  // FIXME: Performance
  // See: newlib/libc/string/memset.c
  while (to_go--)
    *buf++ = (char)__lc32_mmio_rng;

  return 0;
}
