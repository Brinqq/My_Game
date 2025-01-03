#pragma once

#define GAME_NAME "Game"


#if __APPLE__
#define __GAME_BUILD_DEBUG 1
#define _GR_BACKEND_VULKAN 1
#define _GR_BACKEND_OPENGL 0 // opengl blows bro wtf
#define _GR_BACKEND_METAL 0

#define _WINDOW_API_GLFW 1
#define _WINDOW_API_NATIVE 0
#endif

#if _GR_BACKEND_OPENGL && __APPLE__
#define GL_SILENCE_DEPRECATION
#endif

#if __DEBUG
#if __clang__
#define DEBUG_TRAP  __builtin_trap()
#endif
#else
#define DEBUG_TRAP 
#endif
//Assertion for 64 bit arch
static_assert(sizeof(char) == 1, "Application requires compilation with 64bit compiler!");
static_assert(sizeof(short) == 2, "Application requires compilation with 64bit compiler!");
static_assert(sizeof(int) == 4, "Application requires compilation with 64bit compiler!");
static_assert(sizeof(long) == 8, "Application requires compilation with 64bit compiler!");
static_assert(sizeof(float) == 4, "Application requires compilation with 64bit compiler!");
static_assert(sizeof(double) == 8, "Application requires compilation with 64bit compiler!");



