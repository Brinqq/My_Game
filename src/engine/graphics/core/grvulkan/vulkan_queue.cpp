#include "vulkan_queue.h"
#include "platform.h"
#include "mem.h"

//TODO: move queue family stuct inside file once refactor is done
QueueFamilyIndices familyIndices;

static void logOutQueueFamilyInfo(QueueFamilyIndices indices){
  LOG_INFO("Graphic Queue Family Indice: %i", indices.graphicQueueFamilyIndex);
  LOG_INFO("Trnsfer Queue Family Indice: %i", indices.tranferQueueFamilyIndex);
  LOG_INFO("compute Queue Family Indice: %i", indices.computeQueueFamilyIndex);
  LOG_INFO("Present Queue Family Indice: %i", indices.presentQueueFamilyIndex);
}

int VulkanInitializeQueueFamilies(const VkPhysicalDevice& gpu, VulkanQueueConfig& config){
  uint32_t queueFamilyCount;
  vkGetPhysicalDeviceQueueFamilyProperties(gpu, &queueFamilyCount, nullptr);
  VkQueueFamilyProperties* availableQueueFamilies = BAlloca(VkQueueFamilyProperties, queueFamilyCount);
  vkGetPhysicalDeviceQueueFamilyProperties(gpu, &queueFamilyCount, availableQueueFamilies);
  if(pvInitializeQueueFamilies(availableQueueFamilies, config, queueFamilyCount)){
    return 1;
  }
  
  #if __DEBUG
  logOutQueueFamilyInfo(config.indices);
  #endif
  return 0;
}
