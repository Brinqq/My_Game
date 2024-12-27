#pragma once

#include <memory>
#include <assert.h>

#include <stdio.h>

//TODO: support moves
//TODO: Add support for alignment asap(defualt to system size and support custom)

#define DEFAULT_MIN_ALIGN 8

namespace bstl{

template<class _type, uint32_t _align=DEFAULT_MIN_ALIGN, class _allocator=std::allocator<_type>>
class Vector{
public:
  typedef _type value_type;
  typedef uint32_t size_type;
  typedef  _type& reference;
  typedef const _type& const_reference;
  typedef _type* iterator;
  typedef const _type* const_iterator;

private:
  _type* m_pData;
  uint32_t m_capacity;
  uint32_t m_size;
  float m_expanse = 1.5;
  size_t m_bytes;
  bool isTrivial;
  _allocator allocator;
  
  void initialize(){
    isTrivial = std::is_trivial<_type>::value;
  }

  void reallocateMemory(const long bytes){
    _type* tmp = allocator.allocate(bytes);
    memcpy(tmp, m_pData, m_size * sizeof(_type));
    deallocateMemory();
    m_pData = (_type*)tmp;
    m_bytes = bytes;
  }

  void allocateMemory(const long bytes){
    m_pData = allocator.allocate(bytes);
    m_bytes = bytes;
    assert(m_pData != nullptr);
  }
  
  void deallocateMemory(){
    allocator.deallocate(m_pData, m_bytes);
  }

  void popNoDeconstruct(){
    _type* v = end();
    memset(v, 0, sizeof(_type));
    m_size--;
  }

  void construct(_type* object){
    object->_type();
  }

  void construct(_type* start, const _type* end){
    while(start != end){
      start->_type();
      start++;
    }
  }

  template<class ... _args> void construct(_type* object, _args&& ... args){
   new (object) _type(args ...);
  }

  void deconstruct(_type* object){
    object->~_type();
  }

  void deconstruct(_type* start, _type* end){
    while(start <= end){
      start->~_type();
      start++;
    }
  }

  void move(){

  }

  void copy(){

  }



public:

  Vector():m_capacity(5), m_size(0), m_pData(nullptr){
    allocateMemory(sizeof(_type)*m_capacity);
    initialize();
    assert(m_pData != nullptr);
  }

  Vector(const uint32_t capacity){
      m_capacity = capacity;
      m_size = 0;
      allocateMemory(capacity * sizeof(_type));
      initialize();
      assert(m_pData != nullptr);
  }

  ~Vector(){
    if(!isTrivial){
      deconstruct(begin(), end()-1);
    }
    deallocateMemory();
  }

  Vector(Vector& other){
    m_size =  other.m_size;
    m_capacity = other.m_capacity;
    m_expanse = other.m_expanse;
    m_bytes = other.m_bytes;
    allocator = other.allocator;
    isTrivial = other.isTrivial;
    allocateMemory(m_capacity * sizeof(_type));
    memcpy(other.m_pData, m_pData, m_capacity * sizeof(_type));
  }

  Vector(Vector&& other){
    m_size =  other.m_size;
    m_capacity = other.m_capacity;
    m_expanse = other.m_expanse;
    m_bytes = other.m_bytes;
    allocator = other.allocator;
    isTrivial = other.isTrivial;
    allocateMemory(m_capacity * sizeof(_type));
    memcpy(other.m_pData, m_pData, m_capacity * sizeof(_type));
    
    other.deallocateMemory();
    other.m_pData = nullptr;
    other.m_capacity = 0;
    other.m_size = 0;
    other.m_expanse = 0;
    other.m_bytes = 0;
    other.allocator = 0x0;
    other.isTrivial = 0;
  }

  void operator=(Vector& other){ 
    m_size =  other.m_size;
    m_capacity = other.m_capacity;
    m_expanse = other.m_expanse;
    m_bytes = other.m_bytes;
    allocator = other.allocator;
    isTrivial = other.isTrivial;
    allocateMemory(m_capacity * sizeof(_type));
    memcpy(other.m_pData, m_pData, m_capacity * sizeof(_type));
  }

  void operator=(std::initializer_list<_type> list){
    assert(false);
    reset();
    size_t ilistSize = list.size();
    if(ilistSize >= m_capacity){
      m_capacity = ilistSize;
      reallocateMemory();
    }

    m_size =  ilistSize;
    for(const _type& v : list){
    }
  }


  _type& operator[](const int index){
    assert(index > m_size); //maybe error in release mode also;
    return *(m_pData + index);
  }

  _type* begin(){
    return m_pData;
  }

  _type* end(){
    if(m_size == 0){return m_pData;}
    return m_pData + m_size;
  }

  _type* data(){
    return m_pData;
  }

  uint32_t size(){return m_size;}
  uint32_t capacity(){return m_capacity;}
  bool empty(){return m_size == 0;}
  
  void pop(){
    if(!isTrivial){
      deconstruct(end()-1);
    }

    memset (m_pData + m_size, 0, sizeof(_type));
    m_size--;
  }

 template<class ... _args> void appendEmplace(_args&& ... args){
    if(m_size >= m_capacity){
      m_capacity *= m_expanse;
      reallocateMemory(m_capacity * sizeof(_type));
    }

    construct(m_pData + m_size, args ...);
    m_size++;
 }

  void append(const _type& value){
    if(m_size >= m_capacity){
      m_capacity *= m_expanse;
      reallocateMemory(m_capacity * sizeof(_type));
    }

    *(m_pData + m_size) = value;
    m_size++;
  }

  void append(_type&& value){
    if(m_size >= m_capacity){
      m_capacity *= m_expanse;
      reallocateMemory(m_capacity * sizeof(_type));
    }

    *(m_pData + m_size) = value;
    m_size++;
  }

  void erase(_type* pos){
    if(!isTrivial){
      deconstruct(pos);
    }

    memmove(pos, pos+1, (char*)(end()-1) - (char*)(pos+1));   
    popNoDeconstruct();
  }


  void swapBack(_type* pos){
    if(!isTrivial){
      deconstruct(pos);
    }

    *pos = *(end()-1);
    popNoDeconstruct();
  }

  void swapBack(const uint32_t index){
  assert(false);

  }

  const _type& front()const{
    return *begin();
  }

  const _type& back()const{
    return *end()-1;
  }

  _type& front(){
    return *begin();
  }

  _type& back(){
    return *(end()-1);

  }


  void reset(){
    if(!isTrivial){
      deconstruct(begin(), end()-1);
    }

    memset(m_pData, 0, m_capacity * sizeof(_type));
    m_size = 0;
  }

  void resize(const uint32_t size){
    assert(false);
    m_capacity = size;
    deallocateMemory();
    allocateMemory(m_capacity * sizeof(_type));
  }
  
  void shrink(){
    m_capacity = m_size;
    reallocateMemory(sizeof(_type)*m_capacity);
  }

  void setExpanse(const float expanse){m_expanse = expanse;}


};

}
