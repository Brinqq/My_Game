#pragma once

#include "vulkandefines.h"


struct SwapchainConfig{
  VkSurfaceFormatKHR* formats;
  VkPresentModeKHR* presentationModes;
};

VKError vulkanInitializeView(const VkInstance& instance);
void vulkanViewCleanup(const VkInstance& instance);

class VulkanView{
private:
  VulkanDevice& deviceHandle;
  VkInstance& instanceHandle;
  VkSurfaceKHR surface;
  SwapchainConfig swapchain;
  int validateSwapchainSupport();
public:
  VKError initialize();
  void cleanup();
  VulkanView(VkInstance& instance,  VulkanDevice& device):deviceHandle(device), instanceHandle(instance){};
  VulkanView() = delete;
  VulkanView(const VulkanView& other) = delete;
  ~VulkanView();
};


