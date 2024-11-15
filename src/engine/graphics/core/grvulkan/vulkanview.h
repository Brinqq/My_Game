#pragma once

#include "vulkandefines.h"


struct SwapchainConfig{
  VkSurfaceFormatKHR* formats;
  VkPresentModeKHR* presentationModes;
  unsigned int formatsAvailable;
  unsigned int presentationModesAvailable;
};

struct VulkanImageView{
  std::vector<VkImage> images;
  std::vector<VkImageView> views;
};

struct VulkanPresentation{
  VkPresentModeKHR presentMode;
  VkSurfaceFormatKHR format;
  VkExtent2D extent; 
};

VKError vulkanInitializeView(const VkInstance& instance);
void vulkanViewCleanup(const VkInstance& instance);

class VulkanView{
private:
  VulkanDevice& deviceHandle;
  VkInstance& instanceHandle;
  VkSurfaceKHR surface;
  SwapchainConfig swapchainConfig;
  VulkanPresentation presentation;
  VkSwapchainKHR swapchain;
  VkSurfaceCapabilitiesKHR surfaceInfo;
  VulkanImageView imageView;
  bool indepthLogging = false;
  int validateSwapchainSupport();
  int initializePresentation();
  int initializeSwapchain();
  int initializeViewImage();
public:
  VKError initialize();
  void cleanup();
  VulkanView(VkInstance& instance,  VulkanDevice& device):deviceHandle(device), instanceHandle(instance){};
  VulkanView() = delete;
  VulkanView(const VulkanView& other) = delete;
  ~VulkanView();
  inline const VkFormat format() const{return presentation.format.format;}

};


