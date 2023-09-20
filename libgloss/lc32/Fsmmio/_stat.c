#include <_syslist.h>
#include <errno.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>

#include "internal/stat.h"

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

int _stat(const char *file, struct stat *pstat) {

    if (file == NULL) {
        errno = EFAULT;
        return -1;
    }

    if (pstat == NULL) {
        errno = EFAULT;
        return -1;
    }
    
    // call other functions for stat
    int fd = _open(file, O_RDONLY, 0666);
    if (fd == -1) {
        // errno already set by _open()
        return -1;
    }

    struct stat stat_struct;
    int success = _fstat(fd, &stat_struct);
    if (success == -1) {
        _close(fd);
        // errno already set by _fstat()
        return -1;
    }

    _close(fd);

    return 0;
}





// printf("sizeof file pointer: %lu\n", sizeof(file));
//     printf("sizeof stat pointer: %lu\n", sizeof(pstat));
//     printf("sizeof(pstat->st_dev): %lu\n", sizeof(pstat->st_dev));
//     printf("sizeof(pstat->st_ino): %lu\n", sizeof(pstat->st_ino));
//     printf("sizeof(pstat->st_mode): %lu\n", sizeof(pstat->st_mode));
//     printf("sizeof(pstat->st_nlink): %lu\n", sizeof(pstat->st_nlink));
//     printf("sizeof(pstat->st_uid): %lu\n", sizeof(pstat->st_uid));
//     printf("sizeof(pstat->st_gid): %lu\n", sizeof(pstat->st_gid));
//     printf("sizeof(pstat->st_size): %lu\n", sizeof(pstat->st_size));
//     printf("sizeof(pstat->st_atim.tv_sec): %lu\n", sizeof(pstat->st_atim.tv_sec));
//     printf("sizeof(pstat->st_mtim.tv_nsec): %lu\n", sizeof(pstat->st_mtim.tv_nsec));
//     printf("sizeof(pstat->st_ctim): %lu\n", sizeof(pstat->st_ctim));
//     printf("sizeof(pstat->st_blksize): %lu\n", sizeof(pstat->st_blksize));
//     printf("sizeof(pstat->st_blocks): %lu\n", sizeof(pstat->st_blocks));
//     // printf("sizeof(pstat->st_spared): %lu\n", sizeof(pstat->st_spared));
//     // printf("sizeof(pstat->st_spared[0]): %lu\n", sizeof(pstat->st_spared[0]));
    // printf("sizeof(struct stat_convert): %lu\n", sizeof(struct lc32_stat));
// 
