#include <_syslist.h>
#include <errno.h>
#include <stdint.h>
#include <sys/time.h>

#include <stdio.h>

// Remember, all system-level code uses the global variable errno
// See: newlib/libc/reent/reent.c
#undef errno
extern int errno;

// Register structure of the clock peripheral.
struct __lc32_mmio_clock_t {
  uint32_t status;
  uint32_t mil;
  uint32_t sec;
} __attribute__((packed));

// Address of the clock register. Set by the linker script.
extern volatile struct __lc32_mmio_clock_t __lc32_mmio_clock;

int _gettimeofday(struct timeval *ptimeval, void *ptimezone) {

  // NULL argument checking
  if (ptimeval == NULL) {
    errno = EFAULT;
    return -1;
  }

  // Query the current time via hardware
  __lc32_mmio_clock.status = 0;

  // Set the time and return
  ptimeval->tv_sec = __lc32_mmio_clock.sec;
  ptimeval->tv_usec = __lc32_mmio_clock.mil * 1000;
  return 0;
}
