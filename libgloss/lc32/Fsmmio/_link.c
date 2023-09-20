#include <_syslist.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <fcntl.h>

// Remember, all system-level code uses the global variable errno
// See: newlib/libc/reent/reent.c
#undef errno
extern int errno;

// Register structure of the filesystem peripheral
struct __lc32_mmio_fs_t {
  uint16_t mode;
  uint16_t fd;
  uint32_t data1;
  uint32_t data2;
  uint32_t data3;
} __attribute__((packed));

// Address of the filesystem peripheral. Set by linker script.
extern volatile struct __lc32_mmio_fs_t __lc32_mmio_fs;


int _link(const char *oldpath, const char *newpath) {
  
  if (oldpath == NULL || newpath == NULL) {
    errno = EFAULT;
    return -1;
  }

  // Give link args to peripheral
  __lc32_mmio_fs.data1 = (uint32_t) oldpath;
  __lc32_mmio_fs.data2 = (uint32_t) newpath;

  // Assigning the mode will start the filesystem peripheral
  __lc32_mmio_fs.mode = 8;

  // link occurs on peripheral

  // check if error occurred
  uint32_t error = __lc32_mmio_fs.data2;

  // Peripheral will set fd to 0 and data2 to error code on failure
  if (error != 0) {
    errno = error;
    return -1;
  }

  return 0;
}
