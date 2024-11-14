#pragma once
#include "log.h"
#include <cstddef>


#if __unix__ || __APPLE__ 
#include "unistd.h"
#include <limits.h>
#include <sys/sysctl.h>

//call converted to bytes
inline long SYS_PAGE_SIZE;
inline long SYS_CACHE_LINE_SIZE = HW_CACHELINE;

inline void  initializeRequiredSystemInfo(int log = 0){
  SYS_PAGE_SIZE = sysconf(_SC_PAGESIZE);

  if(__DEBUG && log){
    LOG_SYS("Page Size %lu bytes", SYS_PAGE_SIZE);
    LOG_SYS("Cache Line Size %lu bytes\033[0m\n", SYS_CACHE_LINE_SIZE);
  }


}

#endif
