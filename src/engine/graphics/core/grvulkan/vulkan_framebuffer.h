#pragma once
#include "vulkandefines.h"

struct VulkanContext;

inline std::vector<VkFramebuffer> gFramebuffers;

void learnFrameBuffer(const VulkanContext& context);
