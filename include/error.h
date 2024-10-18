#pragma once

#include "global.h"
#include "log.h"

inline void programErrorOut(){
  LOGGING::update();
  std::exit(1);
}
