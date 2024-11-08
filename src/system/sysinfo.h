#pragma once
#include "log.h"


#if __unix__ || __APPLE__ 
#include "unistd.h"
#include <limits.h>
inline long SYS_PAGE_SIZE;
inline long SYS_MAX_THREAD_COUNT;

inline void  initializeRequiredSystemInfo(int log = 0){
  SYS_PAGE_SIZE = sysconf(_SC_PAGESIZE);
  SYS_MAX_THREAD_COUNT = sysconf(_SC_THREAD_THREADS_MAX);

  if(__DEBUG && log){
    printf("\033[1;38;5;208mSYSTEM INFO: Page Size: %lu\033[0m\n", SYS_PAGE_SIZE);
    printf("\033[1;38;5;208mSYSTEM INFO: Max Threads: %lu\033[0m\n", SYS_MAX_THREAD_COUNT);
  }

}

#endif
