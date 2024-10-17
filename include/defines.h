#pragma once
#define GAME_NAME "Game"

#define BACKEND_API_VULKAN 

#if _WIN64 == 0 && _WIN32 == 1
#error "64 bit required on windows"

#endif
