#include "scratchallocator.h"

#include <memory>

ScratchAllocator::ScratchAllocator(long bytes){
  totalBytes = bytes;
  remainingBytes = bytes;
  pBlock = malloc(totalBytes);
}

ScratchAllocator::~ScratchAllocator(){
  free(pBlock);
}

void* const ScratchAllocator::alloc(const long bytes){
  remainingBytes -= bytes;
  if(remainingBytes <= 0){ return nullptr;};
  void* ret = (char*)pBlock + offset;
  offset += bytes;
  return ret;
};

void ScratchAllocator::flush(){
  offset = 0;
}

