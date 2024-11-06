#include "vulkan_device.h"


struct VulkanDeviceState{
  VkPhysicalDeviceProperties GPUProperties;
};

//TODO: heap alloc state objects maybe?
VulkanDeviceState pState;

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

void vulkanDeviceCleanUp(){

}
