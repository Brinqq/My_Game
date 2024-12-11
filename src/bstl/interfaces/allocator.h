#pragma once

// custom allocator interface for now is just going to take in a a struct that contains
// a alloc and free function pointer with the params (long bytes), might change in future.

namespace BSTL{
struct Allocator{
  void*(*alloc)(unsigned long bytes);
  void (*free)(void*);

};

}
