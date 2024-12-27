#pragma once

#include <assert.h>
#include <memory>

class LinearAllocator{
private:
void* pStart; 
void* pEnd; 
void* pCurrent;

public:

LinearAllocator(LinearAllocator& other) = delete;
LinearAllocator(LinearAllocator&& other) = delete;

LinearAllocator():pStart(nullptr), pEnd(nullptr), pCurrent(nullptr){}

~LinearAllocator(){
  
}

inline void init(void* pBuffer, const long bytes){
  pStart = pBuffer;
  pCurrent = pBuffer;
  pEnd = (char*)pBuffer + bytes;
}


inline void* alloc(const long bytes){
  long alignedBytes = bytes % 8 == 0 ? bytes: (bytes - (bytes % 8) + 8);
  if((char*)pCurrent + alignedBytes >= pEnd){
    assert(false);
    return nullptr;
  };
  pCurrent = (char*)pCurrent + alignedBytes;
  return pCurrent;
}

inline void* allocWithAlignment(const long bytes, const int alignmnet){
  assert(false);
  return nullptr;
}


inline void reset(){
  pCurrent = pStart;
}

inline void safeReset(){
  memset(pStart, -1, (char*)pStart - (char*)pEnd);
  pCurrent = pStart;
  
}


inline bool isUsed(){
  return pCurrent != pStart;
}


};
