#include "linearalloc.h"
#include "sysinfo.h"

#include "log.h"

#ifdef __APPLE__
  #include <sys/mman.h>
#endif

