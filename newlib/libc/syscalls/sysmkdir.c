/* connector for mkdir */

#include <reent.h>
#include <unistd.h>

int
mkdir (const char *path,
     int mode)
{
  return _mkdir_r (_REENT, path, mode);
}
