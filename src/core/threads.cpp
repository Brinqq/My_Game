#include "threads.h"
#include "pthread.h"

#include "stdio.h"

void threadStart(){
  int s;
  pthread_attr_t* attr;
  pthread_attr_getdetachstate(attr, &s);
  printf("here");
}
