#include "vulkandevice.h"
#include "vulkandefines.h"
 
struct QueueFamilyIndices{
  uint32_t graphicFamily = 0;
  uint32_t presentFamily = 2;
};


#define VULKAN_DEVICE_EXT_COUNT 2
const char* requestedDeviceExtensions[2] = {
  "VK_KHR_portability_subset",
  VK_KHR_SWAPCHAIN_EXTENSION_NAME,
};

struct VulkanQueues{
  VkQueue graphics;
  VkQueue present;
};

//return error as -1 or the index of the gpu
// for macos choose the first gpu found because they dont matter anyways
int chooseMostSuitableGPU(const VkPhysicalDevice* device, uint32_t nDevices){
  #if __APPLE__
  return 0;
  #endif

  for(int i = 0; i < nDevices; i++){
       
  }

}


static int queueFamilyCreate(){
  return 0;
}

static VkDevice g_device{};
VulkanQueues queues;

int vulkanCreateLogicalDevice(const VkPhysicalDevice& gpu){
  uint32_t c;
  vkGetPhysicalDeviceQueueFamilyProperties(gpu, &c, nullptr);
  VkQueueFamilyProperties* a = (VkQueueFamilyProperties*)alloca(sizeof(VkQueueFamilyProperties)* c);
  vkGetPhysicalDeviceQueueFamilyProperties(gpu, &c, a);
  

  QueueFamilyIndices familyIndices{};
  VkDeviceQueueCreateInfo* qci;
  VkPhysicalDeviceFeatures features{};
  VkDeviceCreateInfo deviceInfo{};
  float queuePriorty = 1.0f;
  float presentPriorty = 0.96f;

  qci = (VkDeviceQueueCreateInfo*)alloca(sizeof(VkDeviceQueueCreateInfo) * 2);
  memset(qci, 0, sizeof(VkDeviceQueueCreateInfo)*2);
  qci[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
  qci[0].queueCount = 1;
  qci[0].queueFamilyIndex = familyIndices.graphicFamily;
  qci[0].pQueuePriorities = &queuePriorty;
  qci[1].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
  qci[1].queueCount = 1;
  qci[1].queueFamilyIndex = familyIndices.presentFamily;
  qci[1].pQueuePriorities = &presentPriorty;

  deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  deviceInfo.queueCreateInfoCount = 2;
  deviceInfo.pQueueCreateInfos = qci;
  deviceInfo.pEnabledFeatures = &features;
  deviceInfo.enabledExtensionCount = VULKAN_DEVICE_EXT_COUNT;
  deviceInfo.ppEnabledExtensionNames = requestedDeviceExtensions;
  VKCALL(vkCreateDevice(gpu, &deviceInfo, nullptr, &g_device));
  vkGetDeviceQueue(g_device, familyIndices.graphicFamily, 0, &queues.graphics);
  vkGetDeviceQueue(g_device, familyIndices.presentFamily, 0, &queues.present);
  return 0;
}

int vulkanCreatePhysicalDevice(const VkInstance& instance, VkPhysicalDevice& device){
  uint32_t nDevices;
  VkPhysicalDevice* devices;
  vkEnumeratePhysicalDevices(instance, &nDevices, nullptr);
  if(nDevices == 0){
    VULKAN_ERROR_THROW(VULKAN_ERROR_NO_GPU_FOUND);
    return 1;
  }

  devices = (VkPhysicalDevice*)alloca(sizeof(VkPhysicalDevice)*nDevices);
  vkEnumeratePhysicalDevices(instance, &nDevices, devices);
  uint32_t index = chooseMostSuitableGPU(devices, nDevices);

  if(index == -1){
    VULKAN_ERROR_THROW(VULKAN_ERROR_NO_GPU_FOUND);
    return 1;
  }

  device = devices[index];

  return 0;
}

void vulkanDeviceDestroy(){
  vkDestroyDevice(g_device, nullptr);
}

VkDevice& vulkanLogicalDeviceGet(){return g_device;}



