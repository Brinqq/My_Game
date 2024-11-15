#include "vulkanview.h"
#include "platform.h"

#include <vulkan/vulkan.h>

//supported arrays are sorted from most desirable to least and when iterating through to set what to use it will choose the first one available
//TODO: Maybe make this configurable
static const unsigned int g_supportedFormatCount = 1;
static const unsigned int g_supportedColorSpaceCount = 1;
static const unsigned int g_supportedPresentationModeCount = 2;
static const unsigned int g_supportedFormatModeCount = 2;
static const VkColorSpaceKHR g_supportedColorSpaces[1]{VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
static const VkFormat g_supportedFormats[1] {VK_FORMAT_B8G8R8A8_SRGB};
static const int g_supportedPresentation[2] {VK_PRESENT_MODE_FIFO_KHR, VK_PRESENT_MODE_FIFO_RELAXED_KHR};
static const int g_supportedFormatModes [2]  {VK_PRESENT_MODE_FIFO_KHR, VK_PRESENT_MODE_FIFO_RELAXED_KHR};

struct VulkanViewState{
  VkSurfaceKHR surface;

};

VKError VulkanView::initialize(){
  if(__DEBUG){ indepthLogging = true; }
  pvCreateSurface(instanceHandle, &surface);
  if(validateSwapchainSupport()) return VULKAN_SWAP_CHAIN_VALIDATION_ERROR;
  if(initializePresentation())return VULKAN_PRESENTATION_NOT_SUPPORTED_ERROR;
  if(initializeSwapchain())return VULKAN_SWAPCHAIN_INITIALIZATION_ERROR;
  if(initializeViewImage())return VULKAN_IMAGEVIEW_INITIALIZATION_ERROR;
  LOG_INFO("View System initialized Successfully");
  return VULKAN_SUCCESS;
};

VulkanView::~VulkanView(){

}

int VulkanView::validateSwapchainSupport(){
  uint32_t numFormats;
  uint32_t numPresentModes;
  VKCALL(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(deviceHandle.gpu, surface, &surfaceInfo))
  VKCALL(vkGetPhysicalDeviceSurfaceFormatsKHR(deviceHandle.gpu, surface, &numFormats, nullptr))
  VKCALL(vkGetPhysicalDeviceSurfacePresentModesKHR(deviceHandle.gpu, surface, &numPresentModes, nullptr))
  void* buffer = malloc((sizeof(VkSurfaceFormatKHR)*numFormats) + (sizeof(VkPresentModeKHR)*numPresentModes));
  swapchainConfig.formats = (VkSurfaceFormatKHR*)buffer;
  swapchainConfig.presentationModes = (VkPresentModeKHR*)(VkSurfaceFormatKHR*)buffer+numFormats;
  VKCALL(vkGetPhysicalDeviceSurfaceFormatsKHR(deviceHandle.gpu, surface, &numFormats, swapchainConfig.formats))
  VKCALL(vkGetPhysicalDeviceSurfacePresentModesKHR(deviceHandle.gpu, surface, &numPresentModes, swapchainConfig.presentationModes))
  swapchainConfig.formatsAvailable = numFormats;
  swapchainConfig.presentationModesAvailable = numPresentModes;
  if(numFormats != 0 && numPresentModes != 0 ) return 0;
  return 1;
}

void VulkanView::cleanup(){
  for(int i = 0; i < imageView.views.capacity(); i++){
    vkDestroyImageView(deviceHandle.device, imageView.views[i], nullptr);
  }
  vkDestroySwapchainKHR(deviceHandle.device, swapchain, nullptr);
  vkDestroySurfaceKHR(instanceHandle, surface, nullptr);
  LOG_INFO("View system deinitialized succesfully");
}

int VulkanView::initializePresentation(){
  bool formatFound = false;
  bool colorspaceFound = false;
  bool presentationModeFound = false;
  for(int i = 0; i < swapchainConfig.formatsAvailable; i++){
    for(int x = 0; x < g_supportedFormatCount; x++){
      if (formatFound == false && swapchainConfig.formats[i].format == g_supportedFormats[x]){
        presentation.format.format = swapchainConfig.formats[i].format;
        formatFound = true;
      }
      if(colorspaceFound == false && swapchainConfig.formats->colorSpace == g_supportedColorSpaces[x]){
        swapchainConfig.formats->colorSpace = g_supportedColorSpaces[x];
        colorspaceFound = true;
      }
    }
  }
  for(int i = 0; i < swapchainConfig.presentationModesAvailable; i++){
    for(int x = 0; x < g_supportedPresentationModeCount; x++){
      if(presentationModeFound == false && swapchainConfig.presentationModes[i] == g_supportedPresentation[x]){
        presentation.presentMode = swapchainConfig.presentationModes[i];
        presentationModeFound = true;
      }
    }
  }
  int resolutionX;
  int resolutionY;
  pwGetPresentationSize(resolutionX, resolutionY);
  presentation.extent.height = static_cast<uint32_t>(resolutionX);
  presentation.extent.width = static_cast<uint32_t>(resolutionY);
  presentation.extent.height = std::clamp(presentation.extent.height, surfaceInfo.minImageExtent.height, surfaceInfo.maxImageExtent.height);
  presentation.extent.width = std::clamp(presentation.extent.width, surfaceInfo.minImageExtent.width, surfaceInfo.maxImageExtent.width);
  if(formatFound && colorspaceFound && presentationModeFound) return 0;
  return 1;
}

//TODO: Queue family setup
int VulkanView::initializeSwapchain(){
  if(indepthLogging){
  }
  VkSwapchainCreateInfoKHR swapchainInfo{};
  swapchainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  swapchainInfo.minImageCount = 2;
  swapchainInfo.presentMode = presentation.presentMode;
  swapchainInfo.surface =  surface;
  swapchainInfo.imageFormat = presentation.format.format;
  swapchainInfo.imageColorSpace = presentation.format.colorSpace;
  swapchainInfo.imageExtent =  presentation.extent;
  swapchainInfo.imageArrayLayers = 1;
  swapchainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
  swapchainInfo.preTransform = surfaceInfo.currentTransform;
  swapchainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  swapchainInfo.clipped = VK_TRUE;
  swapchainInfo.oldSwapchain = VK_NULL_HANDLE;
  VKCALL(vkCreateSwapchainKHR(deviceHandle.device, &swapchainInfo, nullptr, &swapchain))
  return 0;
}


int VulkanView::initializeViewImage(){
  uint32_t imageCount;
  VKCALL(vkGetSwapchainImagesKHR(deviceHandle.device, swapchain, &imageCount, nullptr))
  imageView.images.reserve(imageCount);
  VKCALL(vkGetSwapchainImagesKHR(deviceHandle.device, swapchain, &imageCount, imageView.images.data()))
  imageView.views.reserve(imageCount);
  for(int i = 0; i < imageCount; i++){
    VkImageViewCreateInfo ici{};
    ici.sType =  VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    ici.image  = imageView.images[i];
    ici.viewType = VK_IMAGE_VIEW_TYPE_2D;
    ici.format = presentation.format.format;
    ici.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    ici.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    ici.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    ici.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    ici.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    ici.subresourceRange.baseMipLevel = 0;
    ici.subresourceRange.levelCount = 1;
    ici.subresourceRange.baseArrayLayer = 0;
    ici.subresourceRange.layerCount = 1;
    VKCALL(vkCreateImageView(deviceHandle.device, &ici, nullptr, &imageView.views[i]));
  }

  return 0;
}





