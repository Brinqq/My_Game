#include "vulkan_device.h"
#include "vulkandefines.h"
#include "platform.h"
#include "vulkan_queue.h"
#include "vulkan_extensions.h"
#include "vulkanerr.h"

struct VulkanDeviceState{
  VkPhysicalDeviceProperties GPUProperties;
  VulkanQueues queues;
};

const std::array<const char*, 1> reqDeviceExtensions = {
  "VK_KHR_swapchain",
};

//TODO: heap alloc state objects maybe?
static VulkanDeviceState pState{};

//TODO: Add support for finding the most suitable gpu
static int vulkanIsDesirableGPU(const VkPhysicalDevice& device){
  return 1;
}

VKError VulkanNewPhysicalDevice(const VkInstance& instance, VkPhysicalDevice& device){
  uint32_t deviceCount;
  VKCALL(vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr))
  VkPhysicalDevice* devices =  (VkPhysicalDevice*)alloca(sizeof(VkPhysicalDevice)*deviceCount);
  VKCALL(vkEnumeratePhysicalDevices(instance, &deviceCount, devices))
  for(int i = 0; i < deviceCount; i++){
    if(vulkanIsDesirableGPU(devices[i])){
      device = devices[i];
      LOG_INFO("Usable GPU found and Initialized");
      return VULKAN_SUCCESS;
    };
  }
  return VULKAN_NO_SUITABLE_GRAPHIC_DEVICE;
}

//TODO: add both validation for required device extensions and required extensions array so i can append to the 
//configurable extensions that gets passed when creating the vulkan logical device
//TODO: add configurable device feature impl

VKError VulkanNewLogicalDevice(const VkInstance& instance, const VkPhysicalDevice gpu, VkDevice& device){
  VulkanQueueConfig config{};
  if(VulkanInitializeQueueFamilies(gpu, config)){
    return VULKAN_QUEUE_FAMILY_INITIALIZATION_ERROR;
  }
  VkDeviceCreateInfo dci{};
  VkPhysicalDeviceFeatures deviceFeatures{};
  VkDeviceQueueCreateInfo* pQueues = (VkDeviceQueueCreateInfo*)alloca(sizeof(VkDeviceQueueCreateInfo)*config.queueCount);
  for(int i = 0; i < config.queueCount; i++){
    pQueues[i] = VkDeviceQueueCreateInfo{};
    pQueues[i].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    pQueues[i].pQueuePriorities = &config.pPriorityBuffer[i];
    pQueues[i].queueFamilyIndex = i; 
    pQueues[i].queueCount = 1;
  }
  for(const char* extension: reqDeviceExtensions){
    gVulkanExtensions.deviceExtensions.emplace_back(extension);
  }
  pvAppendRequiredDeviceExtension(gVulkanExtensions.deviceExtensions);
  dci.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  dci.queueCreateInfoCount = config.queueCount;
  dci.pQueueCreateInfos =  pQueues;
  dci.pEnabledFeatures = &deviceFeatures;
  dci.enabledExtensionCount = gVulkanExtensions.deviceExtensions.size();
  dci.ppEnabledExtensionNames = gVulkanExtensions.deviceExtensions.data();
  vkCreateDevice(gpu, &dci, nullptr, &device);
  pvEnableQueues(device, config.indices, pState.queues);
  LOG_INFO("Logical device and queues created!");
  return VULKAN_SUCCESS;
}

void vulkanDeviceCleanup(const VkInstance& instance, VulkanDevice& device){
  //TODO: Clean up GPU(Physical device)
  LOG_INFO("Destorying logical device and gpu context");
  vkDestroyDevice(device.device, nullptr);
}
