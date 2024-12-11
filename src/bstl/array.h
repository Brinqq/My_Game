#pragma once

#include "internal/defines.h"
#include "allocator.h"

#include <memory>

// Fixed Sized array allocated at runtime similar to std::vector but unable to be resized
// and stores the actual size unlike std::array which returns the fixed size for both size() and max_size() :( wtf

namespace BSTL{
template<class _Type, int _Size>
class Array{
private:

  typedef _Type value_type;
  typedef int size_type;
  typedef int difference_type;
  typedef _Type& reference;
  typedef const _Type& const_reference;
  typedef _Type* pointer;
  typedef const _Type* const_pointer;

  _Type* pBuffer;
  int capacity_t;
  int size_t = 0; 
  BSTL::Allocator allocator = {nullptr, nullptr};

public:

Array(){
   allocator = {malloc, free};
  capacity_t = _Size;
  pBuffer = (_Type*)allocator.alloc(sizeof(_Type)*_Size);
}

Array(BSTL::Allocator allocator):allocator(allocator){
  capacity_t = _Size;
  pBuffer = (_Type*)allocator.alloc(sizeof(_Type)*_Size);
}

Array(const Array& other){
  capacity_t = other.capacity_t;
  size_t = other.size_t;
  pBuffer = malloc(sizeof(_Type) * capacity_t);
  memcpy(pBuffer, other.pBuffer, sizeof(_Type) * capacity_t);
}

Array(Array&& other){
  capacity_t = other.capacity_t;
  size_t = other.size_t;
  pBuffer = other.pBuffer;

  other.pBuffer = nullptr;
  other.capacity_t = 0;
  other.size_t = 0;

}

~Array(){
  allocator.free(pBuffer); // up to the allocator impl weather it actual is freed for ex, malloc() 
}


_Type& operator[](int index){
  return empty() ? *pBuffer : *(pBuffer + (index));
}

_Type& at(int index){
  if(index > size_t ){int* ptr = 0x00000; *ptr = 2;} // TODO: throw error
  return empty() ? *(pBuffer + index) : *(pBuffer + (index - 1));
}

_Type& front(){return pBuffer[0];}
_Type& back(){return pBuffer[capacity_t - 1];}
const _Type& front()const{return pBuffer[0];}
const _Type& back()const{return pBuffer[capacity_t - 1];}
_Type* data(){return pBuffer;}


int empty()const{return size_t == 0 ? true: false;}
inline int size(){ return size_t;}
inline int max_size(){return capacity_t;}


void fill(const _Type& data){
  for(int i = 0; i < capacity_t; i++){ // maybe use iterator when implemented
    pBuffer[i] = data;
    
  } 
}



//TODO: implement these
template<class _Array>
void swap(_Array& other){
  
}

template<class _Array>
void operator=(_Array& other){

}


// non standard lib custom methods start here
void push_back(_Type& data){
  if(size_t >= capacity_t){int* ptr = nullptr; *ptr = 2;} // TODO: throw error
  *(pBuffer + size_t) = data;
  size_t++;
}

void emplace_back(_Type data){
  if(size_t >= capacity_t){int* ptr = nullptr; *ptr = 2;} // TODO: throw error
  pBuffer[size_t] = data;
  size_t++;
}

void insert(const _Type data, int index){
*(pBuffer + index) = data;
}

void clear(){
  memset(pBuffer, 0, capacity_t * sizeof(_Type));
  size_t = 0;
}


};

}

