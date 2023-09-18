#include <_syslist.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>

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

_ssize_t _write(int fd, const void *buf, size_t cnt) {
    // if cnt is 0, we are done
    if (cnt == 0) {
        return 0;
    }

    // check that our buffer is non-null
    if (buf == NULL) {
        errno = EFAULT;
        return -1;
    }

    // Input our data into fs peripheral
    __lc32_mmio_fs.fd = (uint16_t) fd;
    __lc32_mmio_fs.data1 = (uint32_t) buf;
    __lc32_mmio_fs.data2 = cnt; 

    // Tell peripheral to write
    __lc32_mmio_fs.mode = 4;

    // write occurs on peripheral

    // get return data and error code from peripheral
    int num_write = __lc32_mmio_fs.data3;
    int error = __lc32_mmio_fs.data2;
    if (error != 0) {
        errno = error;
        return -1;
    }

    return num_write;
}