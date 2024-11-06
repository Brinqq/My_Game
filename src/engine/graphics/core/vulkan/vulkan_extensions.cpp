#include "vulkan_extensions.h"
#include "platform.h"
#include "vulkandefines.h"


VKError VulkanInitDeviceAndInstanceEXT(){
  pvGetRequiredInstanceExtensions(gVulkanExtensions.instanceExtensions);
  return VULKAN_SUCCESS;
}

