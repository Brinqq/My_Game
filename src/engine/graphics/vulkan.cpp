#include "vulkan.h"
#include "global.h"
#include "log.h"
#include "vulkanerr.h"
#include "window.h"
#include "vulkandefines.h"
#include "vulkan/vulkan.h"
#include "vulkan/vulkan_core.h"
#include "platform.h"

int getValidatedDeviceExtensions();

//Sorted from most Desirable to least
const std::array<int, 2>DesirablePresentationModes{VK_PRESENT_MODE_FIFO_KHR, VK_PRESENT_MODE_FIFO_RELAXED_KHR};
const std::array<int, 2>DesirableFormatsModes{VK_PRESENT_MODE_FIFO_KHR, VK_PRESENT_MODE_FIFO_RELAXED_KHR};

static VkInstance gInstanceHandle;
static VkPhysicalDevice gDevice;
static VkDevice gLogicalDevice;
static VulkanQueues gVulkanQueues;
static VkSurfaceKHR gSurface;
static VkSwapchainKHR gSwapchain;
static SwapChainDetails gSwapChainDetails;
std::vector<VkImage> gSwapChainImages;
std::vector<VkImageView> gSwapImageViews;

static QueueFamilyIndices gq;
static VkApplicationInfo gInfo;



std::array<const char*, 1> validationLayers= {
  "VK_LAYER_KHRONOS_validation"
};

std::array<const char*, 2> requiredDeviceExtensions = {
  "VK_KHR_portability_subset",
  "VK_KHR_swapchain"
}
;

int validateRequiredQueueFamilies(){ 
uint32_t queueFamilyCount;
vkGetPhysicalDeviceQueueFamilyProperties(gDevice, &queueFamilyCount, nullptr);
VkQueueFamilyProperties* availableQueueFamilies = (VkQueueFamilyProperties*)alloca(sizeof(VkQueueFamilyProperties)*queueFamilyCount);
vkGetPhysicalDeviceQueueFamilyProperties(gDevice, &queueFamilyCount, availableQueueFamilies);
if(pvInitializeQueueFamilies(availableQueueFamilies, gq, queueFamilyCount) == 0){
return 0;
};

return 1;
}



void createLogicalDeviceA(){
const uint32_t queueCount = 4;
const float priorties[4] = {1.0, 0.99, 0.97, 0.96};

VkDeviceCreateInfo dci{};
VkPhysicalDeviceFeatures deviceFeatures{};
VkDeviceQueueCreateInfo* pQueues = (VkDeviceQueueCreateInfo*)alloca(sizeof(VkDeviceQueueCreateInfo)*queueCount);

pQueues[0] = VkDeviceQueueCreateInfo{};
pQueues[1] = VkDeviceQueueCreateInfo{};
pQueues[2] = VkDeviceQueueCreateInfo{};
pQueues[3] = VkDeviceQueueCreateInfo{};
pQueues[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
pQueues[0].pQueuePriorities = &priorties[0];
pQueues[0].queueFamilyIndex = gq.graphicQueueFamilyIndex; 
pQueues[0].queueCount = 1;
pQueues[1].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
pQueues[1].pQueuePriorities = &priorties[1];
pQueues[1].queueFamilyIndex = gq.computeQueueFamilyIndex; 
pQueues[1].queueCount = 1;
pQueues[2].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
pQueues[2].pQueuePriorities = &priorties[2];
pQueues[2].queueFamilyIndex = gq.tranferQueueFamilyIndex; 
pQueues[2].queueCount = 1;
pQueues[3].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
pQueues[3].pQueuePriorities = &priorties[3];
pQueues[3].queueFamilyIndex = gq.presentQueueFamilyIndex; 
pQueues[3].queueCount = 1;

dci.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
dci.queueCreateInfoCount = 4;
dci.pQueueCreateInfos =  pQueues;
dci.pEnabledFeatures = &deviceFeatures;

if(!getValidatedDeviceExtensions()){
LOG(CRITICAL, "Could not find required device extensions")
return;
};

dci.enabledExtensionCount = requiredDeviceExtensions.size();
dci.ppEnabledExtensionNames = requiredDeviceExtensions.data();
vkCreateDevice(gDevice, &dci, nullptr, &gLogicalDevice);

vkGetDeviceQueue(gLogicalDevice, gq.graphicQueueFamilyIndex, 0, &gVulkanQueues.graphicQueue);
vkGetDeviceQueue(gLogicalDevice, gq.computeQueueFamilyIndex, 0, &gVulkanQueues.computeQueue);
vkGetDeviceQueue(gLogicalDevice, gq.tranferQueueFamilyIndex, 0, &gVulkanQueues.transferQueue);
}

void createSurface(const Window& window){
VKCALL(glfwCreateWindowSurface(gInstanceHandle, window.windowHandle, NULL, &gSurface))
  }


int validateLayerAvailability(){
  int requestedLayers = validationLayers.size();
  int layersFound = 0; 
  uint32_t layerCount;
  vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
  VkLayerProperties* layerProperties = (VkLayerProperties*)alloca(sizeof(VkLayerProperties) * layerCount);
  vkEnumerateInstanceLayerProperties(&layerCount, layerProperties);
    for(const char* layer: validationLayers){
      for(int i = 0; i < layerCount;i++){
        if(strcmp(layer, layerProperties[i].layerName) == 0){
        layersFound++;
        break;
    }
  }
}
if(layersFound == requestedLayers) {return 1;}
return 0;

}

void createVulkanInstance(){
  if(!validateLayerAvailability()){
    LOG(CRITICAL, "Unable to find all request validation layers");
    gExitFlag = true;
  }

  //setting up app info struct to required for vk init function
  gInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO; 
  gInfo.pApplicationName = "Game";
  gInfo.pEngineName = "none";
  gInfo.apiVersion = VK_API_VERSION_1_0;
  gInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  gInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);

  //Vulkan Requires extensions to interface with windows api so we use glfw function that returns that

  //vk init struct also Requires a create info struct
  VkInstanceCreateInfo instanceInfo{};
  instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  instanceInfo.pApplicationInfo = &gInfo;
  instanceInfo.enabledLayerCount = validationLayers.size();
  instanceInfo.ppEnabledLayerNames = validationLayers.data();
  
  //Get Platform requiredExtensions
  std::vector<const char*> extensions;
  pvGetRequiredInstanceExtensions(extensions);


  instanceInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
  instanceInfo.enabledExtensionCount = (uint32_t)extensions.size();
  instanceInfo.ppEnabledExtensionNames = extensions.data();

  //finally we initialize the vulkan instance passing the two structs we filled
  VKCALL(vkCreateInstance(&instanceInfo, nullptr, &gInstanceHandle))

}

void queryAndInitPhysicalDevice(){
  uint32_t deviceCount;
  VKCALL(vkEnumeratePhysicalDevices(gInstanceHandle, &deviceCount, nullptr))
  if(deviceCount == 0){
    LOG(CRITICAL, "Could not find suitble graphics device with vulkan support!");
    gExitFlag = true;
  }
  VKCALL(vkEnumeratePhysicalDevices(gInstanceHandle, &deviceCount, &gDevice))
  
}

int getValidatedDeviceExtensions(){
  uint32_t deviceExtensionCount = 0;
  int requiredExtenstions = requiredDeviceExtensions.size();
  int foundExtensions = 0;
  VKCALL(vkEnumerateDeviceExtensionProperties(gDevice, nullptr, &deviceExtensionCount, nullptr))
  VkExtensionProperties* extensions = (VkExtensionProperties*)alloca(sizeof(VkExtensionProperties)*deviceExtensionCount);
  VKCALL(vkEnumerateDeviceExtensionProperties(gDevice, nullptr, &deviceExtensionCount, extensions))
  for(int i = 0; i < deviceExtensionCount; i++){
    for(const char* requiredExtension: requiredDeviceExtensions){
      if(!strcmp(requiredExtension, extensions[i].extensionName)){
        foundExtensions++;
      }
      if(requiredExtenstions == foundExtensions){return true;}
      
    }
  }
    return false;
}

void initPresentQueueA(){
    vkGetDeviceQueue(gLogicalDevice, 4, 0, &gVulkanQueues.presentQueue);
}

int validateSwapchainSupport(){
  uint32_t numFormats;
  uint32_t numPresentModes;

    VKCALL(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(gDevice, gSurface, &gSwapChainDetails.capabilites))

    VKCALL(vkGetPhysicalDeviceSurfaceFormatsKHR(gDevice, gSurface, &numFormats, nullptr))
    VKCALL(vkGetPhysicalDeviceSurfacePresentModesKHR(gDevice, gSurface, &numPresentModes, nullptr))

    if(numFormats > FORMATS_PREALLOC_ARRAY_SIZE || numPresentModes > PRESENTMODES_PREALLOC_ARRAY_SIZE){
    LOG(ERROR, "Format or PresentMode count larger than container size!")
    printf("PresentModes: %i, Formats: %i\n", numPresentModes, numFormats);
    }

    VKCALL(vkGetPhysicalDeviceSurfaceFormatsKHR(gDevice, gSurface, &numFormats, gSwapChainDetails.formatArr.data()))
    VKCALL(vkGetPhysicalDeviceSurfacePresentModesKHR(gDevice, gSurface, &numPresentModes, gSwapChainDetails.presentModeArr.data()))
    if(numFormats != 0 && numPresentModes != 0 ) return 0;
    return 1;
}

int ChoosePresentationMode(VkPresentModeKHR& presentMode){
  for(VkPresentModeKHR &mode : gSwapChainDetails.presentModeArr){
    for(int desiredMode : DesirablePresentationModes){
      if(mode == desiredMode){
        presentMode = mode;
        return 0;
      }
    } 
  }
  LOG(ERROR, "Unable to find a suitable presentation mode!")
  return 1;
}

int chooseSwapExtent(VkExtent2D& extent){
  int resolutionX;
  int resolutionY;

  pwGetPresentationSize(resolutionX, resolutionY);
  
  extent.height = static_cast<uint32_t>(resolutionX);
  extent.width = static_cast<uint32_t>(resolutionY);

  extent.height = std::clamp(extent.height, gSwapChainDetails.capabilites.minImageExtent.height, gSwapChainDetails.capabilites.maxImageExtent.height);
  extent.width = std::clamp(extent.width, gSwapChainDetails.capabilites.minImageExtent.width, gSwapChainDetails.capabilites.maxImageExtent.width);
  return 0;
}

int chooseFormatType(VkSurfaceFormatKHR& surfaceFormat){
  int formatFlag = 0;
  int colorspaceFlag = 0;
  int loops = 0;
  for(VkSurfaceFormatKHR& format : gSwapChainDetails.formatArr){
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

  LOG(ERROR, "Unable to find a suitable surface format!")
  return 1;
}


  static VkPresentModeKHR presentMode;
  static VkSurfaceFormatKHR format;
  static VkExtent2D extent; 
  static VkFormat vkFormat;
void initSwapchain(){
  if (validateSwapchainSupport()){
    LOG(CRITICAL, "Device does not support required swapchain capabilites!")
    return;
  };
  
  if(ChoosePresentationMode(presentMode) || chooseFormatType(format) || chooseSwapExtent(extent)){
    LOG(CRITICAL, "could not find either presentation mode, extent, or format!")
    return;
  }


  VkSwapchainCreateInfoKHR ci{};
  ci.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  ci.minImageCount = 2;
  ci.presentMode = presentMode;
  ci.surface =  gSurface;
  ci.imageFormat = format.format;
  ci.imageColorSpace = format.colorSpace;
  ci.imageExtent =  extent;
  ci.imageArrayLayers = 1;
  ci.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

  //TODO: Queue family setup
  
  
  //

  ci.preTransform = gSwapChainDetails.capabilites.currentTransform;
  ci.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  ci.clipped = VK_TRUE;
  ci.oldSwapchain = VK_NULL_HANDLE;
  VKCALL(vkCreateSwapchainKHR(gLogicalDevice, &ci, nullptr, &gSwapchain))

}

int querySwapImages(){
  uint32_t imageCount;
  VKCALL(vkGetSwapchainImagesKHR(gLogicalDevice, gSwapchain, &imageCount, nullptr))
  gSwapChainImages.reserve(imageCount);
  VKCALL(vkGetSwapchainImagesKHR(gLogicalDevice, gSwapchain, &imageCount, gSwapChainImages.data()))
  return 0;
}


void createImageViewObject(){
  int images =  gSwapChainImages.size();
  gSwapImageViews.reserve(gSwapChainImages.size());
  for(int i = 0; i < images; i++){
  VkImageViewCreateInfo ici{};
  ici.sType =  VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
  ici.image  = gSwapChainImages[i];
  ici.viewType = VK_IMAGE_VIEW_TYPE_2D;
  ici.format = vkFormat;
  ici.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
  ici.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
  ici.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
  ici.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

  ici.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  ici.subresourceRange.baseMipLevel = 0;
  ici.subresourceRange.levelCount = 1;
  ici.subresourceRange.baseArrayLayer = 0;
  ici.subresourceRange.layerCount = 1;

  VKCALL(vkCreateImageView(gLogicalDevice, &ici, nullptr, &gSwapImageViews[i]));

  }

};

void cleanup(){
  for(VkImageView& view : gSwapImageViews){
    vkDestroyImageView(gLogicalDevice, view, nullptr);
  }
  vkDestroySwapchainKHR(gLogicalDevice, gSwapchain, nullptr);
  vkDestroySurfaceKHR(gInstanceHandle, gSurface, nullptr);
  vkDestroyDevice(gLogicalDevice ,nullptr);
  vkDestroyInstance(gInstanceHandle, nullptr);
}


void testTraingle(){
  pvInitialize();
  createVulkanInstance();
  queryAndInitPhysicalDevice();
  if(validateRequiredQueueFamilies()){
    LOG(CRITICAL, "Required queues families not found");
    gExitFlag = true;
  }

//TODO: Abstract into platform api
  createLogicalDeviceA();
  pvCreateSurface(gInstanceHandle, &gSurface);
  initSwapchain();
  querySwapImages();
  createImageViewObject();
  cleanup();
}



