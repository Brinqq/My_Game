#include "linearalloc.h"
#include "sysinfo.h"

#include "log.h"
#include <cstddef>

#ifdef __APPLE__
  #include <sys/mman.h>
  #define ALLOC(_bytes_) malloc(_bytes_)
#endif

#define LINEAR_ALLOC_BUCKET_METADATA_SIZE 8
#define LINEAR_ALLOC_BUCKET_SIZE_OFFSET 0
#define LINEAR_ALLOC_BUCKET_STRIDE_OFFSET 4
#define LINEAR_ALLOC_BUCKET_DATA_OFFSET 8
#define BYTE char*
static const int LINEAR_ALLOC_METADATA_SIZE = sizeof(LinearAllocator);

void LinearAllocDebugLogBuffer(const LinearAllocator& buffer){
  printf("BufferSize: %lu\n", *(unsigned long*)((char*)buffer.pStart + offsetof(LinearAllocator, bufferSize)));
  printf("RemainingBytes: %lu\n", *(unsigned long*)((char*)buffer.pStart + offsetof(LinearAllocator, remainingBytes)));
  printf("Start ptr: %p\n", (void*)((char*)buffer.pStart + offsetof(LinearAllocator, pStart)));
  printf("Bump ptr: %p\n", (void*)((char*)buffer.pStart + offsetof(LinearAllocator, pBump)));
  unsigned long bumpOffset = buffer.bufferSize - buffer.remainingBytes;
  if((char*)buffer.pBump - bumpOffset == buffer.pStart){
    printf("Bump offset correct\n");

  }
};

void linearAllocDebugLogBucket(const void* const bucket){
  int dataLength = *(int*)((BYTE)bucket+LINEAR_ALLOC_BUCKET_SIZE_OFFSET);
  dataLength -= LINEAR_ALLOC_BUCKET_METADATA_SIZE;
  int elementStride = *(int*)((BYTE)bucket+LINEAR_ALLOC_BUCKET_STRIDE_OFFSET);
  printf("Bucket size in bytes: %i\n", *(int*)((BYTE)bucket+LINEAR_ALLOC_BUCKET_SIZE_OFFSET));
  printf("Bucket data stride: %i\n", elementStride);
  printf("Data size in bytes: %i\n", dataLength);
}

LinearAllocator linearAllocBufferNew(unsigned long bytes){
   bytes += LINEAR_ALLOC_METADATA_SIZE;
  LinearAllocator ret{bytes, bytes, nullptr, nullptr};
  void* pBuffer = ALLOC(bytes);
  ret.pBump = pBuffer;
  ret.pStart = pBuffer;
  memcpy(pBuffer, &ret, LINEAR_ALLOC_METADATA_SIZE);
  return ret;
}

void linearAllocBufferDeallocate(LinearAllocator& buffer){
  buffer.pStart = nullptr;
  buffer.pBump = nullptr;
  free(buffer.pStart);
}

void* const __linearAllocVecBucketNew__(LinearAllocator& buffer, unsigned long bytes, const unsigned int stride){
  bytes += LINEAR_ALLOC_BUCKET_METADATA_SIZE;
  if((unsigned int)buffer.remainingBytes - bytes <= 0){return nullptr;};
  buffer.remainingBytes -= bytes;
  buffer.pBump = (BYTE)buffer.pBump + bytes;
  *(int*)((BYTE)buffer.pBump + LINEAR_ALLOC_BUCKET_SIZE_OFFSET) = bytes;
  *(int*)((BYTE)buffer.pBump + LINEAR_ALLOC_BUCKET_STRIDE_OFFSET) = stride;
  return buffer.pBump;
}

void linearAllocBufferFlush(LinearAllocator& buffer){
  // memset((void*)((BYTE)&buffer+LINEAR_ALLOC_METADATA_SIZE), 0, buffer.bufferSize - LINEAR_ALLOC_METADATA_SIZE);
}
