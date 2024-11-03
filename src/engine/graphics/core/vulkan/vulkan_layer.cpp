#include "vulkan_layer.h"

VKError vulkanValidateLayers(){
  uint32_t queriedLayerCount;
  unsigned int layersFound = 0;
  vkEnumerateInstanceLayerProperties(&queriedLayerCount, nullptr);
  VkLayerProperties* layerProperties = (VkLayerProperties*)alloca(sizeof(VkLayerProperties) * queriedLayerCount);
  vkEnumerateInstanceLayerProperties(&queriedLayerCount, layerProperties);
      for(int i = 0; i <= gRequiredInstanceLayers.layerCount - 1;i++){
        for(int x = 0; i < queriedLayerCount; x++){
          if(strcmp(gRequiredInstanceLayers.layers[i], layerProperties[x].layerName) == 0){
            layersFound++;
            break;
        }
    }
  }
  if(layersFound != gRequiredInstanceLayers.layerCount){return VULKAN_REQUIRED_LAYERS_NOT_FOUND;}
  return VULKAN_SUCCESS;
}

