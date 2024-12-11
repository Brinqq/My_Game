#pragma once

#include "defines.h"
#include "assert.h"
#define __COMPILER_FORCE_INLINE__ 1

#if __CLANG__ || __GNNUC__
static_assert(__COMPILER_FORCE_INLINE_, "clang compiler option --finline-functions required!");
#endif
