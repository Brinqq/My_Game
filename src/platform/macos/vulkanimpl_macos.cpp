#include "global.h"
#include "platform.h"
#include "GLFW/glfw3.h"
#include "defines.h"
#include "log.h"

#include "vulkandefines.h"


int pvGetRequiredInstanceExtensions(std::vector<const char*>& ext){
  uint32_t extensionCount;
  const char** extensionArray = glfwGetRequiredInstanceExtensions(&extensionCount);
  if(extensionCount == 0){
    LOG_CRITICAL( "GLFW found 0 required instance extensions");
    return 1;
  }
  ext.reserve(extensionCount + 4);
  for(int i = 0; i < extensionCount; i++){
    ext.emplace_back(extensionArray[i]);
  }
  ext.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
  ext.emplace_back("VK_KHR_get_physical_device_properties2");
  ext.emplace_back("VK_EXT_metal_surface");
  ext.emplace_back("VK_KHR_surface");
  return 0;
}

void pvAppendRequiredDeviceExtension(std::vector<const char*>& extensions){
  extensions.emplace_back("VK_KHR_portability_subset");
}

void pvEnableQueues(const VkDevice& device, const QueueFamilyIndices& indices, VulkanQueues& queues){
  vkGetDeviceQueue(device, indices.graphicQueueFamilyIndex, 0, &queues.graphicQueue);
  vkGetDeviceQueue(device, indices.computeQueueFamilyIndex, 0, &queues.computeQueue);
  vkGetDeviceQueue(device, indices.tranferQueueFamilyIndex, 0, &queues.transferQueue);
  vkGetDeviceQueue(device, indices.presentQueueFamilyIndex, 0, &queues.presentQueue);
}

void pvInitializeQueue(){

}

void pvSetQueueCreateInfo(VkDeviceQueueCreateInfo* pQueues, uint32_t count, const float priorityArray[4]){
  for(int i = 0; i < count; i++){
    pQueues[i] = VkDeviceQueueCreateInfo{};
    pQueues[i].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    pQueues[i].pQueuePriorities = &priorityArray[i];
    pQueues[i].queueFamilyIndex = i; 
    pQueues[i].queueCount = 1;
  }
}
  
int pvInitializeQueueFamilies(VkQueueFamilyProperties* properties, VulkanQueueConfig& config, uint32_t count){
  int x = 0;
  for(int i = 0; i < count;i++){
    if(properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT|VK_QUEUE_TRANSFER_BIT|VK_QUEUE_COMPUTE_BIT){
       x++;
    }
  }

  if(x == count){
    config.indices.graphicQueueFamilyIndex = 0;
    config.indices.computeQueueFamilyIndex = 1;
    config.indices.tranferQueueFamilyIndex = 2;
    config.indices.presentQueueFamilyIndex = 3;
    config.queueCount = 4;
    config.pPriorityBuffer = (float*)malloc(sizeof(float)*4);
    config.pPriorityBuffer[0] = 1.0f;
    config.pPriorityBuffer[1] = 0.97f;
    config.pPriorityBuffer[2] = 0.98f;
    config.pPriorityBuffer[2] = 0.99f;
    return 0;
  }
  
  return 1;
}

void pvCreateLogicalDevice(){

}

int pvInitialize(){
  return 0;
}

void pvDeInitialize(){
}
