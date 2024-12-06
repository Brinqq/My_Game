#pragma once

#include "defines.h"

class GLView;
class GLDevice;
class GLPipeline;

#if _GR_BACKEND_VULKAN
#endif
#if _GR_BACKEND_OPENGL
typedef GLView RenderView;
typedef GLDevice RenderDevice;
typedef GLPipeline RenderPipeline;
#endif
#if _GR_BACKEND_METAL
#endif
