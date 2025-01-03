#include "defines.h"
#include "stdio.h"
#include "cstdlib"

inline void custom_assert(const char* expr, const char* file, const char* func , int line){
  printf("Assertion Failure! cassert(%s), %s:%i %s()\n", expr, file, line, func);
  fflush(stdout);
  DEBUG_TRAP;
  std::exit(79);
}

#if __DEBUG
#define cassert(_expr) const char* expr = #_expr; if(!_expr){custom_assert(expr, __FILE__, __func__, __LINE__);}
#else
  cassert()
#endif

