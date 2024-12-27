#include "scratchbuffer.h"

#include "stdio.h"

#include <memory>
#include <assert.h>

// #if __DEBUG
// #define
// #else
// #define
// #endif


// [Fixed size: long][remaning bytes: long][bump pointer][user data]
#define SCRATCH_META_DATA_BYTES 24
#define SCRATCH_META_DATA_PTR_OFFSET 16
#define SCRATCH_META_DATA_REMAINING_BYTES_OFFSET 8

typedef char* byte;

//TODO: Add support for taking in a backing allocator interface
//for now backing allocator will be malloc

static void* scratchBufferAllocateBackingMemory(const long bytes){
  void* ret = malloc(bytes);
  assert(ret != nullptr);
  return ret;
}


void* scratchBufferCreate(const long bytes){
  size_t nBytes = bytes + SCRATCH_META_DATA_BYTES;
  void* pBlock = scratchBufferAllocateBackingMemory(nBytes);
  *(byte)pBlock = nBytes;
  *((byte)pBlock + 8) = nBytes;
  *((uintptr_t*)((byte)pBlock+SCRATCH_META_DATA_PTR_OFFSET)) = (uintptr_t)((byte)pBlock + SCRATCH_META_DATA_BYTES);
  return pBlock;
}

void scratchBufferDestroy(void* pBuffer){
  free(pBuffer);
  
}

void* scratchBufferAllocate(void* const pBuffer, const long bytes){
  if(*((long*)pBuffer+1) - bytes < 0){
    //TODO: throw runtime error
  }

  *(long*)((byte)pBuffer+SCRATCH_META_DATA_REMAINING_BYTES_OFFSET) -= bytes;
  return (void*)(uintptr_t*)(((byte)pBuffer+SCRATCH_META_DATA_PTR_OFFSET) + bytes);
}

void scratchBufferFlush(const void* pBuffer){
  *((uintptr_t*)((byte)pBuffer+SCRATCH_META_DATA_PTR_OFFSET)) = (uintptr_t)((byte)pBuffer + SCRATCH_META_DATA_BYTES);
}

long scratchBufferAvailableBytes(const void* const pBuffer){
  return *(long*)((byte)pBuffer+SCRATCH_META_DATA_REMAINING_BYTES_OFFSET);
}





