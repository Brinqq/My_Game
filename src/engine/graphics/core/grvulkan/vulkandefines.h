#pragma once

#include "global.h"

#include "vulkan/vulkan.h"
#include "vulkanerr.h"
#include "vulkan_device.h"



#define FORMATS_PREALLOC_ARRAY_SIZE 100
#define PRESENTMODES_PREALLOC_ARRAY_SIZE 5

inline VkVertexInputBindingDescription gVertexDescription{};
inline VkVertexInputAttributeDescription gAttribDescription{};

struct QueueFamilyIndices{
  uint32_t graphicQueueFamilyIndex;
  uint32_t computeQueueFamilyIndex;
  uint32_t tranferQueueFamilyIndex;
  uint32_t presentQueueFamilyIndex;
};

struct SwapChainDetails{
  VkSurfaceCapabilitiesKHR capabilites;
  std::array<VkSurfaceFormatKHR,  FORMATS_PREALLOC_ARRAY_SIZE> formatArr;
  std::array<VkPresentModeKHR,PRESENTMODES_PREALLOC_ARRAY_SIZE> presentModeArr;
};


struct VulkanQueues{
  VkQueue graphicQueue;
  VkQueue computeQueue;
  VkQueue transferQueue;
  VkQueue presentQueue;
};

struct PresentationInfo{
  VkPresentModeKHR presentMode;
  VkSurfaceFormatKHR format;
  VkExtent2D extent; 
  VkFormat vkFormat;
};

struct VulkanContext{
  VkPipelineLayout pipelineLayout;
  VkPipeline pipeline;
  VkRenderPass renderPass;
  VkInstance instanceHandle;
  VkPhysicalDevice physicalDevice;
  VkDevice device;
  VulkanQueues queues;
  VkSurfaceKHR surface;
  VkSwapchainKHR swapchain;
  SwapChainDetails swapChainDetails;
  QueueFamilyIndices queueFamilyIndices;
  PresentationInfo presentationInfo;

  std::vector<VkImage> swapChainImages;
  std::vector<VkImageView> swapImageViews;
};


struct VulkanState{
  VkInstance instance;
  VkPhysicalDevice GPU;

};

inline VulkanState* pVulkan;
