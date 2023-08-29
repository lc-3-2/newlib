#pragma once

#include <stdbool.h>
#include <unistd.h>

// Keep track of whether the standard file descriptors have been closed. We
// initialize these to `false` so that they go in the `.bss` section.
extern bool __lc32_stdin_closed;
extern bool __lc32_stdout_closed;
extern bool __lc32_stderr_closed;

// Return whether the argument is a standard file descriptor that is still open
static inline bool __lc32_is_open_stdfd(int fd) {
  return (fd == STDIN_FILENO && !__lc32_stdin_closed) ||
         (fd == STDOUT_FILENO && !__lc32_stdout_closed) ||
         (fd == STDERR_FILENO && !__lc32_stderr_closed);
}
