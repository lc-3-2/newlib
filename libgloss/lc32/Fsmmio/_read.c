#include <_syslist.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>

// Remember, all system-level code uses the global variable errno
// See: newlib/libc/reent/reent.c
#undef errno
extern int errno;

struct __lc32_mmio_fs_t {
  uint16_t mode;
  uint16_t fd;
  uint32_t data1;
  uint32_t data2;
  uint32_t data3;
} __attribute__((packed));

extern volatile struct __lc32_mmio_fs_t __lc32_mmio_fs;

// We can't open any files, and we don't start with any file descriptors.
// Therefore, any file descriptor passed will be invalid.
_ssize_t _read(int fd, void *buf, size_t cnt) {

  // Early check for zero - do nothing in that case
  if (cnt == 0)
    return 0;

  // Check if buffer is non-null
  if (buf == NULL) {
    errno = EFAULT;
    return -1;
  }
  
  // Input our data into fs peripheral
  __lc32_mmio_fs.fd = (uint16_t) fd;
  __lc32_mmio_fs.data1 = (uint32_t) buf;
  __lc32_mmio_fs.data2 = cnt; 

  // Tell peripheral to read
  __lc32_mmio_fs.mode = 3;

  // peripheral doing read

  // get return value for read
  int num_read = __lc32_mmio_fs.data3;
  int error = __lc32_mmio_fs.data2;
  if (error != 0) {
    errno = error;
    return -1;
  }

  return num_read;
}
