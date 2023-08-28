#include <_syslist.h>
#include <errno.h>
#include <stdint.h>

// Remember, all system-level code uses the global variable errno
// See: newlib/libc/reent/reent.c
#undef errno
extern int errno;

// Variables provided by the linker script
extern char __heap_start;
extern char __heap_end;

void *_sbrk(ptrdiff_t incr) {

  // Initially, we don't have any data on the heap
  // This variable should always be between the start and end
  static char *heap_cur = &__heap_start;

  // Easy case if incr == 0
  if (incr == 0)
    return heap_cur;

  // Convenience variables
  const uintptr_t heap_start_num = (uintptr_t) &__heap_start;
  const uintptr_t heap_end_num = (uintptr_t) &__heap_end;
  const uintptr_t heap_cur_num = (uintptr_t) heap_cur;

  // Check - are we increasing or decreasing the data segment size. Decreasing
  // is easier, so handle that first.
  if (incr < 0) {
    // Check we aren't going beyond the start of the data segment. Careful about
    // overflow.
    if (heap_cur_num - heap_start_num < ((uintptr_t) -incr)) {
      errno = EINVAL;
      return (void *) -1;
    }

  } else {
    // Check we aren't going beyond the end of the data segment. Careful about
    // overflow.
    if (heap_end_num - heap_cur_num < ((uintptr_t) incr)) {
      errno = ENOMEM;
      return (void *) -1;
    }
  }

  // Do the change and return
  void *ret = heap_cur;
  heap_cur += incr;
  return ret;
}
