#pragma once

#define GAME_NAME "Game"
#define __GAME_BUILD_DEBUG 1
#define _GR_BACKEND_VULKAN 

//Assertion for 64 bit arch
static_assert(sizeof(char) == 1, "Application requires compilation with 64bit compiler!");
static_assert(sizeof(short) == 2, "Application requires compilation with 64bit compiler!");
static_assert(sizeof(int) == 4, "Application requires compilation with 64bit compiler!");
static_assert(sizeof(long) == 8, "Application requires compilation with 64bit compiler!");
static_assert(sizeof(float) == 4, "Application requires compilation with 64bit compiler!");
static_assert(sizeof(double) == 8, "Application requires compilation with 64bit compiler!");

