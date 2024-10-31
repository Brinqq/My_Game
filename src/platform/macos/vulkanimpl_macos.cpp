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

void pvEnableQueues(VulkanContext* context){
  vkGetDeviceQueue(context->device, context->queueFamilyIndices.graphicQueueFamilyIndex, 0, &context->queues.graphicQueue);
  vkGetDeviceQueue(context->device, context->queueFamilyIndices.computeQueueFamilyIndex, 0, &context->queues.computeQueue);
  vkGetDeviceQueue(context->device, context->queueFamilyIndices.tranferQueueFamilyIndex, 0, &context->queues.transferQueue);
  vkGetDeviceQueue(context->device, context->queueFamilyIndices.presentQueueFamilyIndex, 0, &context->queues.presentQueue);
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
  
int pvInitializeQueueFamilies(VkQueueFamilyProperties* properties, QueueFamilyIndices& indexStruct, uint32_t count){
  int x = 0;
  for(int i = 0; i < count;i++){
    if(properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT|VK_QUEUE_TRANSFER_BIT|VK_QUEUE_COMPUTE_BIT){
       x++;
    }
  }

  if(x == count){
    indexStruct.graphicQueueFamilyIndex = 0;
    indexStruct.computeQueueFamilyIndex = 1;
    indexStruct.tranferQueueFamilyIndex = 2;
    indexStruct.presentQueueFamilyIndex = 3;
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
