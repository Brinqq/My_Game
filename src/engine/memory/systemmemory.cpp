#include "systemmemory.h"

#include "stdlib.h"

void* SystemMemory::allocate(const long bytes){
  return malloc(bytes);
}
