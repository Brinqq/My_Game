#include "vulkan.h"
#include "platform.h"
#include "vulkanerror.h"
#include "vulkandevice.h"
#include "log.h"


#include "bstl/vector.h"



static VkInstance g_instance;
static VkPhysicalDevice g_gpu;


std::array<const char*, 1> requestedLayers{
  "VK_LAYER_KHRONOS_validation",
};


int checkForRequiredExtensionSupport(){
  uint32_t extensionCount;
  vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
  VkExtensionProperties* pAvailableExtensions = (VkExtensionProperties*)alloca(sizeof(VkExtensionProperties)*extensionCount);
  vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, pAvailableExtensions);

  for(int i = 0; i < extensionCount; i++){
  }
  
  return 0;
}

int checkForRequiredLayerSupport(){

}

static VulkanError vulkanCreateInstance(){

  if(checkForRequiredExtensionSupport()){
    return VULKAN_ERROR_MISSING_EXTENSION;
  };

  bstl::Vector<const char*> ext;
  ext.append("VK_KHR_get_physical_device_properties2");
  platformGetRequiredVulkanExtensions(ext);
  VkInstanceCreateInfo instanceInfo{};
  VkApplicationInfo appInfo{};
  int x = 3 + ext.size();
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = "Game";
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.pEngineName = "engine";
  appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.apiVersion = VK_API_VERSION_1_0;
  instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  instanceInfo.pApplicationInfo = &appInfo;
  instanceInfo.enabledLayerCount = requestedLayers.size();
  instanceInfo.ppEnabledLayerNames = requestedLayers.data();
  instanceInfo.enabledExtensionCount = ext.size();
  instanceInfo.ppEnabledExtensionNames = ext.data();
  instanceInfo.flags = PLATFORM_VULKAN_REQUIRED_INSTANCE_FLAGS;
  VKCALL(vkCreateInstance(&instanceInfo, nullptr, &g_instance))
  return VULKAN_ERROR_NONE;
}


static void cleanup(){
  vkDestroyInstance(g_instance, nullptr);
}


int vulkanInitialize(){
  vulkanCreateInstance();
  vulkanCreatePhysicalDevice(g_instance, g_gpu);
  vulkanCreateLogicalDevice(g_gpu);
  return 0;
}
