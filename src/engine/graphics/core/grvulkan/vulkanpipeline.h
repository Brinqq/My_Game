#pragma once

#include "vulkandefines.h"

class VulkanView;

class VulkanPipeline{
  std::vector<VkRenderPass> renderpasses;
  int initializeRenderpasses();
  VkInstance& instanceHandle;
  VkDevice& deviceHandle;
  VulkanView& viewHandle;

public:
  VKError initialize();
  void shaderStageBind()const;
  VulkanPipeline(VkInstance& instance, VkDevice& device, VulkanView& view):instanceHandle(instance),deviceHandle(device), viewHandle(view){}
  void cleanup();
};
