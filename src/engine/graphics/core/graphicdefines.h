#pragma once

#include "defines.h"


#if _GR_BACKEND_VULKAN
#endif
#if _GR_BACKEND_OPENGL
#include "opengl.h"
typedef GLView RenderView;
typedef GLDevice RenderDevice;
typedef GLPipeline RenderPipeline;
#endif
#if _GR_BACKEND_METAL
#endif
