#include <_syslist.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>

#include "internal/stdfd-state.h"

// Remember, all system-level code uses the global variable errno
// See: newlib/libc/reent/reent.c
#undef errno
extern int errno;

// Data for the console. The `st_rdev` value has zero for the minor number. Make
// sure to set that before returning. Also note the times - they are set to
// epoch 0.
static const struct stat console_stat = {
  .st_dev = 0,
  .st_ino = 0,
  .st_mode = S_IFCHR | 0666,
  .st_nlink = 1,
  .st_uid = 0,
  .st_gid = 0,
  .st_rdev = 0x0100,
  .st_size = 0,
  .st_blksize = 0x1000,
  .st_blocks = 0,
  .st_atim = {
    .tv_sec = 0,
    .tv_nsec = 0,
  },
  .st_mtim = {
    .tv_sec = 0,
    .tv_nsec = 0,
  },
  .st_ctim = {
    .tv_sec = 0,
    .tv_nsec = 0,
  },
};

int _fstat(int fd, struct stat *pstat) {

  // If the input is not a standard file descriptor, it cannot exist. Similarly,
  // if the input is a standard file descriptor that was previously closed, it
  // does not currently exist. Handle these cases.
  if (!__lc32_is_open_stdfd(fd)) {
    errno = EBADF;
    return -1;
  }

  // NULL argument checking
  if (pstat == NULL) {
    errno = EFAULT;
    return -1;
  }

  // The input is a currently open standard file descriptor. In particular, it
  // is a terminal. So, set the data for the console.
  memcpy(pstat, &console_stat, sizeof(struct stat));
  // Make sure to set the minor number of the device
  pstat->st_rdev |= fd;
  // Return successfully
  return 0;
}
