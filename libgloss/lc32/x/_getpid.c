#include <_syslist.h>

// We don't have process handling. Therefore, we only have the root process,
// with PID 1.
int _getpid(void) { return 1; }
