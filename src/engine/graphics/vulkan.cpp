#include "vulkan.h"
#include "global.h"
#include "log.h"
#include "vulkanerr.h"
#include "window.h"
#include "vulkandefines.h"
#include "vulkan/vulkan.h"
#include "vulkan/vulkan_core.h"

#include "platform.h"

#define REQ_QUEUE_FAMILIES VK_QUEUE_GRAPHICS_BIT || VK_QUEUE_TRANSFER_BIT


int getValidatedDeviceExtensions();

#ifdef __APPLE__
#include "GLFW/glfw3.h"
const uint32_t gDefaultQueueFamilyIndice = 0;

struct QueueFamilies{
  uint32_t graphicfamily = 0;
  uint32_t computeFamily = 1;
  uint32_t transferFamily = 2;
};

struct VulkanQueues{
  VkQueue graphicQueue;
  VkQueue computeQueue;
  VkQueue transferQueue;
};

#endif

struct GlfwInfo{
  uint32_t glfwExtensionCount = 0;
  const char** glfwExtension;
};

#ifdef _WIN32
struct VulkanQueues{
  VkQueue graphicQueue;
  VkQueue computeQueue;
  VkQueue transferQueue;
};

struct QueueFamilies{
  uint32_t graphicfamily = 0;
  uint32_t computeFamily = 1;
  uint32_t transferFamily = 2;
};

int validateRequiredQueueFamilies(){
  return 0;
}

#endif

static GlfwInfo glfwInfo;




static VkInstance gInstanceHandle;
static VkPhysicalDevice gDevice;
static VkDevice gLogicalDevice;
static VulkanQueues gVulkanQueues;
static VkSurfaceKHR gSurface;

static QueueFamilies gQueues;
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

void createLogicalDevice(){
const uint32_t queueCount = 3;
const float priorties[3] = {1.0, 0.99, 0.97};

VkDeviceCreateInfo dci{};
VkPhysicalDeviceFeatures deviceFeatures{};
VkDeviceQueueCreateInfo* pQueues = (VkDeviceQueueCreateInfo*)alloca(sizeof(VkDeviceQueueCreateInfo)*queueCount);

pQueues[0] = VkDeviceQueueCreateInfo{};
pQueues[1] = VkDeviceQueueCreateInfo{};
pQueues[2] = VkDeviceQueueCreateInfo{};
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

dci.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
dci.queueCreateInfoCount = 3;
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


void initSwapchain(){
  

}

void initializeSurface(){
}

void cleanup(){
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

  createLogicalDevice();
  pvCreateSurface(gInstanceHandle, &gSurface);
  cleanup();
  // initSwapchain();
}



