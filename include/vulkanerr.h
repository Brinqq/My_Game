#pragma once
#include "vulkan/vulkan.h"
#include "global.h"

#if __DEBUG
inline VkResult gVkResult;

#define VKCALL(x) gVkResult = x; if(gVkResult != VK_SUCCESS){throwVkError(gVkResult, __LINE__, __FILE__);}

inline void throwVkError(VkResult code, int line, const char* file){
printf("\033[31m\033[1mVKERROR:\033[0m%i \033[1m\033[32mFILE:\033[0m%s \033[33m\033[1mLINE:\033[0m%i\n",code, file, line );
gExitFlag = true;
}
#else
#define VKCALL(x) x;
#endif

