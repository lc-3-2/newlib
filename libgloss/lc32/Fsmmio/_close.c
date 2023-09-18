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

int _close(int fd) {

  // Put our close data in first
  __lc32_mmio_fs.fd = fd;

  // Assigning the mode (close = 2) will start the filesystem peripheral
  __lc32_mmio_fs.mode = 2;

  // close occurs on peripheral

  // Filesystem peripheral will return the success code to data3 member
  int error = __lc32_mmio_fs.data2;
  if (error != 0) {
    errno = error;
    return -1;
  }

  return 0;
}
