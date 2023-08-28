#pragma once

#include <stdbool.h>

// Keep track of whether the standard file descriptors have been closed
extern bool __lc32Trap_stdin_closed;
extern bool __lc32Trap_stdout_closed;
extern bool __lc32Trap_stderr_closed;
