#include <_syslist.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <fcntl.h>


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

_off_t _lseek(int fd, _off_t pos, int whence) {
    // give args to device
    __lc32_mmio_fs.fd = fd;
    __lc32_mmio_fs.data1 = pos;
    __lc32_mmio_fs.data2 = whence;

    // start device
    __lc32_mmio_fs.mode = 5;

    // seek runs on device

    // get return value
    int result = __lc32_mmio_fs.data3;
    int error = __lc32_mmio_fs.data2;

    if (error != 0) {
        errno = error;
        return -1;
    }

    return result;
}
