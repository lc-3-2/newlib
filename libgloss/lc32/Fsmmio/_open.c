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
volatile struct __lc32_mmio_fs_t __lc32_mmio_fs;


int _open(const char *file, int flags, int mode) {

  if (file == NULL) {
    errno = EFAULT;
    return -1;
  }

  // Give open args to peripheral first
  __lc32_mmio_fs.data1 = (uint32_t) file;
  __lc32_mmio_fs.data2 = (uint32_t) flags;
  __lc32_mmio_fs.data3 = (uint32_t) mode;

  // Assigning the mode will start the filesystem peripheral
  __lc32_mmio_fs.mode = 1;

  // open occurs on peripheral

  // Filesystem peripheral will write file descriptor to fd field of struct
  uint32_t descriptor = __lc32_mmio_fs.fd;
  uint32_t error = __lc32_mmio_fs.data2;

  // Peripheral will set fd to 0 and data2 to error code on failure
  if (error != 0) {
    // printf("Error has occured when opening with errno: %d\n", __lc32_mmio_fs.data2);
    errno = __lc32_mmio_fs.data2;
    return -1;
  }

  return descriptor;
}
