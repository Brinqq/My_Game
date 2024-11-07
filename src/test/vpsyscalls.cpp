#include "vpsyscalls.h"
#include "log.h"

#if __UNIX || __APPLE__
#define _GNU_SOURCE
#include "sys/mman.h"
#include <unistd.h>
#include <fcntl.h>
#include "cerrno"

const long PAGE_SIZE =  sysconf(_SC_PAGESIZE);

void sys_assert(const char* msg, long code){
  if(code == -1){
    LOG_ERROR(msg);
    exit(1);

  }
}

void vpSysCallStart(){
  void* m = malloc(4);
  void* s = sbrk(8);
  free(m);
  (char*)s+5;
}

#endif
