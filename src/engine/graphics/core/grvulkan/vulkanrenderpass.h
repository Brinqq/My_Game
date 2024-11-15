#pragma once

struct VulkanContext;
#include "vulkandefines.h"

enum VulkanRenderPassType{
  VULKAN_RENDERPASS_TYPE_DEFAULT = 0,
};

struct VulkanRenderPassConfig{
  VkFormat format;
};


void vulkanInitializeRenderpassSystem(const VulkanRenderPassConfig& config);
int initializeRenderpass(VulkanContext& context);

int VulkanNewRenderPass(const VkDevice& device, VkRenderPass& renderpass, const VulkanRenderPassType type);

