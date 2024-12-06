#include "pg.h"
#include "log.h"

#include "linearalloc.h"

#include "sys/mman.h"

void pgTest(){
  PROT_READ;
  PROT_WRITE;
  LinearAllocator a = linearAllocBufferNew(200);
  LinearAllocDebugLogBuffer(a);
  int* intArr = linearAllocVecBucketNew(a, int, 20);
  linearAllocDebugLogBucket(intArr);
  linearAllocBufferFlush(a);
  linearAllocBufferDeallocate(a);
}
