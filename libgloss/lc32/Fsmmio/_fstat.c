#include <_syslist.h>
#include <errno.h>
#include <sys/stat.h>
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

struct lc32_timespec {
    time_t tv_sec;
    long tv_nsec;
} __attribute__((packed));

struct lc32_stat {
    dev_t		st_dev;
    ino_t		st_ino;
    mode_t	st_mode;
    nlink_t	st_nlink;
    uid_t		st_uid;
    gid_t		st_gid;
    dev_t		st_rdev;
    off_t		st_size;
    struct lc32_timespec st_atim;
    struct lc32_timespec st_mtim;
    struct lc32_timespec st_ctim;
    blksize_t     st_blksize;
    blkcnt_t	st_blocks;
} __attribute__((packed));

int _fstat(int fd, struct stat *pstat) {

    if (fd < 0) {
        errno = EBADF;
        return -1;
    }

    if (pstat == NULL) {
        errno = EFAULT;
        return -1;
    }

    
    // create our own struct stat
    struct lc32_stat copy;

    // provide data to peripheral
    __lc32_mmio_fs.fd = fd;
    __lc32_mmio_fs.data1 = (uint32_t) &copy;

    // start operation
    __lc32_mmio_fs.mode = 6;

    // fstat operation occurs

    // get info back
    int result = __lc32_mmio_fs.data3;
    int error = __lc32_mmio_fs.data2;

    if (error != 0) {
        errno = error;
        return -1;
    }

    // copy over to system stat struct
    // printf("stat->st_dev): %hd\n", copy.st_dev);
    // printf("stat->st_ino): %hu\n", copy.st_ino);
    // printf("stat->st_mode): %u\n", (copy.st_mode));
    // printf("stat->st_nlink): %hu\n", (copy.st_nlink));
    // printf("stat->st_uid): %u\n", (copy.st_uid));
    // printf("stat->st_gid): %u\n", (copy.st_gid));
    // printf("stat->st_rdev): %hd\n", (copy.st_rdev));
    // printf("stat->st_size): %lu\n", (copy.st_size));
    // printf("stat->st_atim): %lld\n", (copy.st_atim.tv_sec));
    // printf("stat->st_mtim): %lld\n", (copy.st_mtim.tv_sec));
    // printf("stat->st_ctim): %lld\n", (copy.st_ctim.tv_sec));
    // printf("stat->st_blksize): %lu\n", (copy.st_blksize));
    // printf("stat->st_blocks): %lu\n", (copy.st_blocks));

    pstat->st_dev = copy.st_dev;
    pstat->st_ino = copy.st_ino;
    pstat->st_mode = copy.st_mode;
    pstat->st_nlink = copy.st_nlink;
    pstat->st_uid = copy.st_uid;
    pstat->st_gid = copy.st_gid;
    pstat->st_rdev = copy.st_rdev;
    pstat->st_size = copy.st_size;
    pstat->st_atim.tv_sec = copy.st_atim.tv_sec;
    pstat->st_atim.tv_nsec = copy.st_atim.tv_nsec;
    pstat->st_mtim.tv_sec = copy.st_mtim.tv_sec;
    pstat->st_mtim.tv_nsec = copy.st_mtim.tv_nsec;
    pstat->st_ctim.tv_sec = copy.st_ctim.tv_sec;
    pstat->st_ctim.tv_nsec = copy.st_ctim.tv_nsec;
    pstat->st_atime = copy.st_atim.tv_sec;
    pstat->st_mtime = copy.st_atim.tv_sec;
    pstat->st_ctime = copy.st_atim.tv_sec;

    return 0;
}