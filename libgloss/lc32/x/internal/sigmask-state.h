#pragma once

#include <signal.h>

// Keep track of which signals are currently masked
extern sigset_t __lc32_sigmask;
