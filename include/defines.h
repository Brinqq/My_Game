#pragma once

#define GAME_NAME "Game"
#define __DEBUG 1
#define __GAME_BUILD_DEBUG 1

#define BACKEND_API_VULKAN 

#if _WIN64 == 0 && _WIN32 == 1
#error "64 bit required on windows"

#endif


