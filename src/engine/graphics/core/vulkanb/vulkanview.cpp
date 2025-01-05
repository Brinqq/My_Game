#include "vulkanview.h"
#include "vulkandevice.h"

#include "platform.h"
#include "bstl/vector.h"
#include "vulkanerror.h"

//TODO: switch to using custom containers instead of stl
//TODO: add support for find best surface settings

//NOTE: for now while we get vulkan up and running
//we use a preset surface format and presentMode we know
//that is available.

static VkSurfaceKHR g_surface;
static VkSurfaceCapabilitiesKHR g_capabilites;
static VkPresentModeKHR g_mainPresentMode;
static VkSurfaceFormatKHR g_surfaceFormat;
static VkExtent2D g_extent;
static VkSwapchainKHR g_swapchain;

static std::vector<VkSurfaceFormatKHR> g_formats;
static std::vector<VkPresentModeKHR> g_presentModes;
static std::vector<VkImage> g_images;
static std::vector<VkImageView> g_imageViews;

//NOTE: Most of the swapchain creation function is going to be refactored to support dynamic initialization
//based on what the specific user gpu driver has and requires.
static int vulkanCreateSwapchain(VkPhysicalDevice gpu){
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(gpu, g_surface, &g_capabilites);
  uint32_t nFormats;
  uint32_t nPresentModes;
  vkGetPhysicalDeviceSurfaceFormatsKHR(gpu, g_surface, &nFormats, nullptr);
  VKCALL(vkGetPhysicalDeviceSurfacePresentModesKHR(gpu, g_surface, &nPresentModes, nullptr));
  g_formats.resize(nFormats);
  g_presentModes.resize(nPresentModes);
  VKCALL(vkGetPhysicalDeviceSurfaceFormatsKHR(gpu, g_surface, &nFormats, g_formats.data()));
  VKCALL(vkGetPhysicalDeviceSurfacePresentModesKHR(gpu, g_surface, &nPresentModes, g_presentModes.data()))
  if(g_formats.size() == 0 || g_presentModes.size() == 0){ return 1;}

  g_surfaceFormat.format = VK_FORMAT_B8G8R8A8_SRGB;
  g_surfaceFormat.colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
  g_mainPresentMode = VK_PRESENT_MODE_FIFO_KHR;
  g_extent.width = 1920;
  g_extent.height = 1080;

  uint32_t nBuffers = g_capabilites.minImageCount + 1;

  VkSwapchainCreateInfoKHR sci{};
  sci.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  sci.surface = g_surface;
  sci.minImageCount = nBuffers;
  sci.imageFormat = g_surfaceFormat.format;
  sci.imageColorSpace = g_surfaceFormat.colorSpace;
  sci.imageExtent = g_extent;
  sci.imageArrayLayers = 1;
  sci.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
  sci.preTransform = g_capabilites.currentTransform;
  sci.presentMode = g_mainPresentMode;
  sci.clipped = VK_TRUE;
  sci.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  sci.oldSwapchain = VK_NULL_HANDLE;

  //NOTE: might not work
  sci.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
  sci.queueFamilyIndexCount = 0; // Optional
  sci.pQueueFamilyIndices = nullptr; // Optional
  //
  VKCALL(vkCreateSwapchainKHR(vulkanLogicalDeviceGet(), &sci, nullptr, &g_swapchain))

  uint32_t nImages;
  vkGetSwapchainImagesKHR(vulkanLogicalDeviceGet(), g_swapchain, &nImages, nullptr);
  g_images.resize(nImages);
  vkGetSwapchainImagesKHR(vulkanLogicalDeviceGet(), g_swapchain, &nImages, g_images.data());
  
  g_imageViews.resize(g_images.size());
  VkImageViewCreateInfo ivci{};
  ivci.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
  ivci.format = g_surfaceFormat.format;
  ivci.viewType = VK_IMAGE_VIEW_TYPE_2D;

  ivci.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
  ivci.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
  ivci.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
  ivci.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;

  ivci.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  ivci.subresourceRange.baseMipLevel = 0;
  ivci.subresourceRange.levelCount = 1;
  ivci.subresourceRange.baseArrayLayer = 0;
  ivci.subresourceRange.layerCount = 1;

  for(int i = 0; i < g_images.size(); i++){
    ivci.image = g_images[i];
    VKCALL(vkCreateImageView(vulkanLogicalDeviceGet(), &ivci, nullptr, &g_imageViews[i]))
  }



  
  return 0;
}

int vulkanViewCreate(VkInstance instance, VkPhysicalDevice gpu){

  platformVulkanSurfaceCreate(instance, &g_surface);
  vulkanCreateSwapchain(gpu);
  return 0;
}

void vulkanViewDestroy(VkInstance instance){
  for(VkImageView& v : g_imageViews){
    vkDestroyImageView(vulkanLogicalDeviceGet(), v, nullptr);
  }
  vkDestroySurfaceKHR(instance, g_surface, nullptr);
  vkDestroySwapchainKHR(vulkanLogicalDeviceGet(), g_swapchain, nullptr);
}
