#pragma once

/*
Bucket Layout: [size - 4 bytes] [stride - 4 bytes][Data - {Requested bytes}]
*/



struct LinearAllocator{
  unsigned long bufferSize;
  unsigned long remainingBytes;
  void* pStart;
  void* pBump;
};

LinearAllocator linearAllocBufferNew(const unsigned long bytes);
void LinearAllocDebugLogBuffer(const LinearAllocator& buffer);
void linearAllocBufferDeallocate(LinearAllocator& buffer);
void* const __linearAllocVecBucketNew__(LinearAllocator& buffer, unsigned long bytes, const unsigned int stride);
void* const __linearAllocBucketNew__(LinearAllocator& buffer, unsigned long bytes);
void linearAllocDebugLogBucket(const void* const bucket);
void linearAllocBufferFlush(LinearAllocator& buffer);
// void LinearAllocDeallocate(void* const buffer);

#define linearAllocVecBucketNew(_buffer_, _type_, _elements_) (_type_*)__linearAllocVecBucketNew__(_buffer_, sizeof(_type_)*_elements_, sizeof(_type_))
#define linearAllocBucketNewAbstract(_buffer_, _bytes_) __linearAllocBucketNew__(_buffer_, _bytes_)
