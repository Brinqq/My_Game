#include "vulkan.h"
#include "vulkan_pipline.h"
#include "global.h"
#include "log.h"
#include "vulkanerr.h"
#include "window.h"
#include "vulkandefines.h"
#include "vulkan/vulkan.h"
#include "vulkan/vulkan_core.h"
#include "platform.h"
#include "vulkan_renderpass.h"

int getValidatedDeviceExtensions();

//Sorted from most Desirable to least
const std::array<int, 2>DesirablePresentationModes{VK_PRESENT_MODE_FIFO_KHR, VK_PRESENT_MODE_FIFO_RELAXED_KHR};
const std::array<int, 2>DesirableFormatsModes{VK_PRESENT_MODE_FIFO_KHR, VK_PRESENT_MODE_FIFO_RELAXED_KHR};


static VulkanContext* context;



std::array<const char*, 1> requiredVulkanLayers= {
  "VK_LAYER_KHRONOS_validation"
};

const std::array<const char*, 1> requiredDeviceExtensions = {
  "VK_KHR_swapchain",
};

std::array<const char*, 0> requiredInstanceExtensions = {
};

VKError validateLayerAvailability(){
  int requestedLayers = requiredVulkanLayers.size();
  int layersFound = 0; 
  uint32_t layerCount;
  vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
  VkLayerProperties* layerProperties = (VkLayerProperties*)alloca(sizeof(VkLayerProperties) * layerCount);
  vkEnumerateInstanceLayerProperties(&layerCount, layerProperties);
    for(const char*& layer : requiredVulkanLayers){
      for(int i = 0; i < layerCount;i++){
        if(strcmp(layer, layerProperties[i].layerName) == 0){
        layersFound++;
        break;
    }
  }
}
if(layersFound == requestedLayers){return VULKAN_SUCCESS;}
  return VULKAN_REQUIRED_LAYERS_NOT_FOUND;
}

VKError validateInstanceExtension(){
  return VULKAN_SUCCESS;
}

VKError getValidatedDeviceExtensions(const std::vector<const char*>& ext){
  uint32_t deviceExtensionCount = 0;
  int requiredExtenstions = ext.size();
  int foundExtensions = 0;
  VKCALL(vkEnumerateDeviceExtensionProperties(context->physicalDevice, nullptr, &deviceExtensionCount, nullptr))
  VkExtensionProperties* extensions = (VkExtensionProperties*)alloca(sizeof(VkExtensionProperties)*deviceExtensionCount);
  VKCALL(vkEnumerateDeviceExtensionProperties(context->physicalDevice, nullptr, &deviceExtensionCount, extensions))
  for(int i = 0; i < deviceExtensionCount; i++){
    for(const char* requiredExtension: ext){
      if(!strcmp(requiredExtension, extensions[i].extensionName)){foundExtensions++;}
      if(requiredExtenstions == foundExtensions){return VULKAN_SUCCESS;}
    }
  }
  return VULKAN_REQUIRED_DEVICE_EXTENSIONS_MISSING;
}

VKError validateSwapchainSupport(){
  uint32_t numFormats;
  uint32_t numPresentModes;

  VKCALL(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(context->physicalDevice, context->surface, &context->swapChainDetails.capabilites))
  VKCALL(vkGetPhysicalDeviceSurfaceFormatsKHR(context->physicalDevice, context->surface, &numFormats, nullptr))
  VKCALL(vkGetPhysicalDeviceSurfacePresentModesKHR(context->physicalDevice, context->surface, &numPresentModes, nullptr))

  if(numFormats > FORMATS_PREALLOC_ARRAY_SIZE || numPresentModes > PRESENTMODES_PREALLOC_ARRAY_SIZE){
    LOG_ERROR( "Format or PresentMode count larger than container size!");
  }

  VKCALL(vkGetPhysicalDeviceSurfaceFormatsKHR(context->physicalDevice, context->surface, &numFormats, context->swapChainDetails.formatArr.data()))
  VKCALL(vkGetPhysicalDeviceSurfacePresentModesKHR(context->physicalDevice, context->surface, &numPresentModes, context->swapChainDetails.presentModeArr.data()))
  if(numFormats != 0 && numPresentModes != 0 ) return VULKAN_SUCCESS;
    return VULKAN_SWAP_CAPABILITIES_NOT_FOUND;
  }


int isDesirableGPU(VkPhysicalDevice device){
  return 1;
  return 0;
}


void createVulkanInstance(){
  VKCHECK(validateLayerAvailability());

  VkApplicationInfo aci{};
  VkInstanceCreateInfo ici{};

  aci.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO; 
  aci.pApplicationName = "Game";
  aci.pEngineName = "none";
  aci.apiVersion = VK_API_VERSION_1_0;
  aci.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  aci.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  ici.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  ici.pApplicationInfo = &aci;
  ici.enabledLayerCount = requiredVulkanLayers.size();
  ici.ppEnabledLayerNames = requiredVulkanLayers.data();
  ici.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
  
  std::vector<const char*> extensions{};
  pvGetRequiredInstanceExtensions(extensions);

  ici.enabledExtensionCount = (uint32_t)extensions.size();
  ici.ppEnabledExtensionNames = extensions.data();

  VKCALL(vkCreateInstance(&ici, nullptr, &context->instanceHandle))
}

VKError initGPUContext(){
  uint32_t deviceCount;
  VKCALL(vkEnumeratePhysicalDevices(context->instanceHandle, &deviceCount, nullptr))
  VkPhysicalDevice* devices =  (VkPhysicalDevice*)alloca(sizeof(VkPhysicalDevice)*deviceCount);
  VKCALL(vkEnumeratePhysicalDevices(context->instanceHandle, &deviceCount, devices))
  for(int i = 0; i < deviceCount; i++){
    if(isDesirableGPU(devices[i])){
      context->physicalDevice = devices[i];
      return VULKAN_SUCCESS;
    };
  }
  return VULKAN_NO_SUITABLE_GRAPHIC_DEVICE;
}

VKError initializeQueuefamilies(){
  uint32_t queueFamilyCount;
  vkGetPhysicalDeviceQueueFamilyProperties(context->physicalDevice, &queueFamilyCount, nullptr);
  VkQueueFamilyProperties* availableQueueFamilies = (VkQueueFamilyProperties*)alloca(sizeof(VkQueueFamilyProperties)*queueFamilyCount);
  vkGetPhysicalDeviceQueueFamilyProperties(context->physicalDevice, &queueFamilyCount, availableQueueFamilies);
  if(!pvInitializeQueueFamilies(availableQueueFamilies, context->queueFamilyIndices, queueFamilyCount)){return VULKAN_SUCCESS;}
  return VULKAN_QUEUE_FAMILIY_INITIALIZATION_ERROR;
}

void createLogicalDevice(){
  const uint32_t queueCount = 4;
  const float priorties[4] = {1.0, 0.99, 0.97, 0.96};

  VkDeviceCreateInfo dci{};
  VkPhysicalDeviceFeatures deviceFeatures{};
  VkDeviceQueueCreateInfo* pQueues = (VkDeviceQueueCreateInfo*)alloca(sizeof(VkDeviceQueueCreateInfo)*queueCount);

  std::vector<const char*> extensions;
  pvAppendRequiredDeviceExtension(extensions);
  for(const char* extension : requiredDeviceExtensions){extensions.emplace_back(extension);}
  pvSetQueueCreateInfo(pQueues, queueCount, priorties);

  dci.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  dci.queueCreateInfoCount = queueCount;
  dci.pQueueCreateInfos =  pQueues;
  dci.pEnabledFeatures = &deviceFeatures;

  VKCHECK(getValidatedDeviceExtensions(extensions))

  dci.enabledExtensionCount = extensions.size();
  dci.ppEnabledExtensionNames = extensions.data();
  vkCreateDevice(context->physicalDevice, &dci, nullptr, &context->device);
  pvEnableQueues(context);

}

int ChoosePresentationMode(VkPresentModeKHR& presentMode){
  for(VkPresentModeKHR &mode : context->swapChainDetails.presentModeArr){
    for(int desiredMode : DesirablePresentationModes){
      if(mode == desiredMode){
        presentMode = mode;
        return 0;
      }
    } 
  }
  LOG_CRITICAL("Unable to find a suitable presentation mode!");
  return 1;
}

int chooseSwapExtent(VkExtent2D& extent){
  int resolutionX;
  int resolutionY;

  pwGetPresentationSize(resolutionX, resolutionY);
  
  extent.height = static_cast<uint32_t>(resolutionX);
  extent.width = static_cast<uint32_t>(resolutionY);

  extent.height = std::clamp(extent.height, context->swapChainDetails.capabilites.minImageExtent.height, context->swapChainDetails.capabilites.maxImageExtent.height);
  extent.width = std::clamp(extent.width, context->swapChainDetails.capabilites.minImageExtent.width, context->swapChainDetails.capabilites.maxImageExtent.width);
  return 0;
}

int chooseFormatType(VkSurfaceFormatKHR& surfaceFormat){
  int formatFlag = 0;
  int colorspaceFlag = 0;
  int loops = 0;
  for(VkSurfaceFormatKHR& format : context->swapChainDetails.formatArr){
    if(format.format == VK_FORMAT_B8G8R8A8_SRGB ){
      surfaceFormat.format = VK_FORMAT_B8G8R8A8_SRGB;
      formatFlag = 1;
    }

   if(format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR){
      surfaceFormat.colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
      colorspaceFlag = 1;
    }

    if (colorspaceFlag && formatFlag)return 0;
  }

  LOG_CRITICAL("Unable to find a suitable surface format!");
  return 1;
}


  static VkPresentModeKHR presentMode;
  static VkSurfaceFormatKHR format;
  static VkExtent2D extent; 
  static VkFormat vkFormat;

void initSwapchain(){
  VKCHECK(validateSwapchainSupport())

  if(ChoosePresentationMode(context->presentationInfo.presentMode) || chooseFormatType(context->presentationInfo.format)
  || chooseSwapExtent(context->presentationInfo.extent)){
    LOG_CRITICAL("could not find either presentation mode, extent, or format!");
    return;
  }


  VkSwapchainCreateInfoKHR ci{};
  ci.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  ci.minImageCount = 2;
  ci.presentMode = context->presentationInfo.presentMode;
  ci.surface =  context->surface;
  ci.imageFormat = context->presentationInfo.format.format;
  ci.imageColorSpace = context->presentationInfo.format.colorSpace;
  ci.imageExtent =  context->presentationInfo.extent;
  ci.imageArrayLayers = 1;
  ci.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

  //TODO: Queue family setup
  
  
  //

  ci.preTransform = context->swapChainDetails.capabilites.currentTransform;
  ci.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  ci.clipped = VK_TRUE;
  ci.oldSwapchain = VK_NULL_HANDLE;
  VKCALL(vkCreateSwapchainKHR(context->device, &ci, nullptr, &context->swapchain))

}

int querySwapImages(){
uint32_t imageCount;
VKCALL(vkGetSwapchainImagesKHR(context->device, context->swapchain, &imageCount, nullptr))
  context->swapChainImages.reserve(imageCount);
VKCALL(vkGetSwapchainImagesKHR(context->device, context->swapchain, &imageCount, context->swapChainImages.data()))
  return 0;
  }

void createImageViewObject(){
  int images =  context->swapChainImages.size();
  context->swapImageViews.reserve(context->swapChainImages.size());
  for(int i = 0; i < images; i++){
  VkImageViewCreateInfo ici{};
  ici.sType =  VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
  ici.image  = context->swapChainImages[i];
  ici.viewType = VK_IMAGE_VIEW_TYPE_2D;
  ici.format = context->presentationInfo.vkFormat;
  ici.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
  ici.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
  ici.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
  ici.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
  ici.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  ici.subresourceRange.baseMipLevel = 0;
  ici.subresourceRange.levelCount = 1;
  ici.subresourceRange.baseArrayLayer = 0;
  ici.subresourceRange.layerCount = 1;

  VKCALL(vkCreateImageView(context->device, &ici, nullptr, &context->swapImageViews[i]));
  }

};

void vulkanDeinitialize(){
  for(VkImageView& view : context->swapImageViews){
    vkDestroyImageView(context->device, view, nullptr);
  }
  vkDestroySwapchainKHR(context->device, context->swapchain, nullptr);
  vkDestroySurfaceKHR(context->instanceHandle, context->surface, nullptr);
  vkDestroyDevice(context->device ,nullptr);
  vkDestroyInstance(context->instanceHandle, nullptr);
  delete(context);
}

int initializeVulkan(){
  context = new VulkanContext;
  pvInitialize();
  createVulkanInstance();
  VKCHECK(initGPUContext())
  VKCHECK(initializeQueuefamilies());
  createLogicalDevice();
  pvCreateSurface(context->instanceHandle, &context->surface);
  initSwapchain();
  querySwapImages();
  createImageViewObject();
  return 0;
}



void testTraingle(){
  initializeVulkan();
  initializeRenderpass(*context);
  initializeVulkanPipeline(context->device, context->renderPass, context->pipeline);
  
}




