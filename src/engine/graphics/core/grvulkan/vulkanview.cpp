#include "vulkanview.h"
#include "platform.h"

#include <vulkan/vulkan.h>

struct VulkanViewState{
  VkSurfaceKHR surface;

};

VKError VulkanView::initialize(){
  pvCreateSurface(instanceHandle, &surface);
  if(validateSwapchainSupport()) return VULKAN_SWAP_CHAIN_VALIDATION_ERROR;
  LOG_INFO("View System initialized Successfully");
  return VULKAN_SUCCESS;
};

VulkanView::~VulkanView(){

}

int VulkanView::validateSwapchainSupport(){
  uint32_t numFormats;
  uint32_t numPresentModes;
  if(numFormats != 0 && numPresentModes != 0 ) return 0;
  VkSurfaceCapabilitiesKHR surfaceInfo;
  VKCALL(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(deviceHandle.gpu, surface, &surfaceInfo))
  VKCALL(vkGetPhysicalDeviceSurfaceFormatsKHR(deviceHandle.gpu, surface, &numFormats, nullptr))
  VKCALL(vkGetPhysicalDeviceSurfacePresentModesKHR(deviceHandle.gpu, surface, &numPresentModes, nullptr))
  void* buffer = malloc((sizeof(VkSurfaceFormatKHR)*numFormats) + (sizeof(VkPresentModeKHR)*numPresentModes));
  swapchain.formats = (VkSurfaceFormatKHR*)buffer;
  swapchain.presentationModes = (VkPresentModeKHR*)(VkSurfaceFormatKHR*)buffer+numFormats;
  VKCALL(vkGetPhysicalDeviceSurfaceFormatsKHR(deviceHandle.gpu, surface, &numFormats, swapchain.formats))
  VKCALL(vkGetPhysicalDeviceSurfacePresentModesKHR(deviceHandle.gpu, surface, &numPresentModes, swapchain.presentationModes))
  return 1;
}

void VulkanView::cleanup(){
  vkDestroySurfaceKHR(instanceHandle, surface, nullptr);
  LOG_INFO("View system deinitialized succesfully");
}
//
// static int validateSwapchainSupport(){
//   }
//
// void vulkanViewCleanup(const VkInstance& instance){
//   vkDestroySurfaceKHR(instance, pState->surface, nullptr);
//   LOG_INFO("View system deinitialized succesfully");
// }
