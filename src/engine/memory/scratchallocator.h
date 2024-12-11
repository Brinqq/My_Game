#pragma once

//NOTE: Allocator does not support individual element alignment. rather the size of the buffer will be aligned 
//to a size divisable by a word(8 bytes/ on 64bit arch), so structs must must be padded before insertion into buffer.

class ScratchAllocator{
private:
  void* pBlock;
  long remainingBytes;
  long totalBytes;
  int offset = 0;
public:
  ScratchAllocator(const ScratchAllocator& other) = delete;
  ScratchAllocator(ScratchAllocator&& other) = delete;
  ScratchAllocator() = default;
  ScratchAllocator(const long bytes);
  void* const alloc(const long bytes);
  void flush();
  ~ScratchAllocator();
};
