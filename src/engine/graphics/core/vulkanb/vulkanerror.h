#pragma once

#include "vulkan/vulkan.h"
#include "log.h"


#define VKCALL(x) VkResult result; result = x; if(result != 0){vkResultWrap(result, __LINE__, __FILE__);};

inline void vkResultWrap(int code ,int line , const char* file){ 
  LOG_CRITICAL("VKCALL resulted in error code %i on LINE %i: in FILE %s", code, line, file);
  std::exit(2);
}
