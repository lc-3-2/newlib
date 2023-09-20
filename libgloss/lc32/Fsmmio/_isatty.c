#include <_syslist.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>

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

extern volatile struct __lc32_mmio_fs_t __lc32_mmio_fs;

int _isatty(int fd) {

  if (fd < 0) {
    errno = EBADF;
    return -1;
  }

  // Put our isatty data in first
  __lc32_mmio_fs.fd = fd;

  // Assigning the mode will start peripheral (isatty = 7)
  __lc32_mmio_fs.mode = 7;

  // isatty occurs on peripheral

  // Filesystem peripheral will return the error code
  int error = __lc32_mmio_fs.data2;
  if (error != 0) {
    errno = error;
    return 0;
  }

  return 1;
}
