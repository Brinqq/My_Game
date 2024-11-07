#pragma once

#if _UNIX || __APPLE__ 
#include "unistd.h"

inline const long SYS_PAGE_SIZE = sysconf(_SC_PAGESIZE);

#endif
