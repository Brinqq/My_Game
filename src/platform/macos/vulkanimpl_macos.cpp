#include "global.h"
#include "platform.h"
#include "GLFW/glfw3.h"

#include "vulkandefines.h"


struct ValidationArrays{
  const std::array<const char*, 1> validationLayers= {
    "VK_LAYER_KHRONOS_validation"
  };

  const std::array<const char*, 2> requiredDeviceExtensions = {
    "VK_KHR_portability_subset",
    "VK_KHR_swapchain"
  };
  
};

static const ValidationArrays* pValidationArrays;

int pvGetRequiredInstanceExtensions(std::vector<const char*>& ext){
  uint32_t extensionCount;
  const char** extensionArray = glfwGetRequiredInstanceExtensions(&extensionCount);
  if(extensionCount == 0){
    LOG(CRITICAL, "GLFW found 0 required instance extensions")
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
    return 0;
  }
  
  return 1;
}

void pvCreateLogicalDevice(){

}

int pvInitialize(){
  pValidationArrays = new ValidationArrays();
  return 0;
}

void pvDeInitialize(){
  delete(pValidationArrays);
}
