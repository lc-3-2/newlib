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


int _mkdir(const char *pathname, int mode) {
  
  if (pathname == NULL) {
    errno = EFAULT;
    return -1;
  }

  // Give mkdir args to peripheral first
  __lc32_mmio_fs.data1 = (uint32_t) pathname;
  __lc32_mmio_fs.data2 = (uint32_t) mode;

  // Assigning the mode will start the filesystem peripheral
  __lc32_mmio_fs.mode = 10;

  // mkdir occurs on peripheral

  // Filesystem peripheral will write file descriptor to fd field of struct
  uint32_t error = __lc32_mmio_fs.data2;

  if (error != 0) {
    errno = error;
    return -1;
  }

  return 0;
}
