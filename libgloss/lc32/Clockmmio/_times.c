#include <_syslist.h>
#include <errno.h>
#include <stdint.h>
#include <sys/times.h>

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

clock_t _times(struct tms *ptms) {

  // Query the current time via hardware
  __lc32_mmio_clock.status = 0;

  // Populate the return structure if possible. We only have the one process,
  // and it only operates in "user".
  if (ptms != NULL) {
    ptms->tms_utime = 1000 * __lc32_mmio_clock.sec + __lc32_mmio_clock.mil;
    ptms->tms_stime = 0;
    ptms->tms_cutime = 0;
    ptms->tms_cstime = 0;
  }

  // Return the number of clock ticks that have elapsed. If we would return -1,
  // return the next value. This way, we don't accidentally signal failure.
  clock_t ret = 1000 * __lc32_mmio_clock.sec + __lc32_mmio_clock.mil;
  return ret == -1 ? 0 : ret;
}
