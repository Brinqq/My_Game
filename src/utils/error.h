#pragma once

#include "global.h"
#include "log.h"

inline void programErrorOut(){
  std::exit(1);
}

inline void applicationThrowCritcalError(){
  std::exit(1);
}