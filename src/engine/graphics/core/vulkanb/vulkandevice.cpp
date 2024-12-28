#include "vulkandevice.h"
#include "vulkandefines.h"
 
//TODO: setup queue family null atm tell needed
struct QueueFamilyIndices{
  uint32_t graphicFamily = 0;
};

const char* requestedDeviceExtensions[1] = {
  "VK_KHR_portability_subset"
};

struct VulkanQueues{
  VkQueue graphics;
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
  
}

static VkDevice g_device{};
VulkanQueues queues;

int vulkanCreateLogicalDevice(const VkPhysicalDevice& gpu){
  QueueFamilyIndices familyIndices{};
  VkDeviceQueueCreateInfo queueCreateInfo{};
  VkPhysicalDeviceFeatures features{};
  VkDeviceCreateInfo deviceInfo{};
  float queuePriorty = 1.0f;
  queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
  queueCreateInfo.queueCount = 1;
  queueCreateInfo.queueFamilyIndex = familyIndices.graphicFamily;
  queueCreateInfo.pQueuePriorities = &queuePriorty;
  deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  deviceInfo.queueCreateInfoCount = 1;
  deviceInfo.pQueueCreateInfos = &queueCreateInfo;
  deviceInfo.pEnabledFeatures = &features;
  deviceInfo.enabledExtensionCount = 1;
  deviceInfo.ppEnabledExtensionNames = requestedDeviceExtensions;
  VKCALL(vkCreateDevice(gpu, &deviceInfo, nullptr, &g_device));
  vkGetDeviceQueue(g_device, familyIndices.graphicFamily, 0, &queues.graphics);
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
