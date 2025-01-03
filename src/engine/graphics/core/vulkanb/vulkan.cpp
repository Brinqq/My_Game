#include "vulkan.h"
#include "platform.h"
#include "vulkanerror.h"
#include "vulkandevice.h"
#include "vulkanview.h"
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
  return 0; 
}


static VulkanError vulkanCreateInstance(){

  if(checkForRequiredExtensionSupport()){
    return VULKAN_ERROR_MISSING_EXTENSION;
  };

 
  
  VkDebugUtilsMessengerCreateInfoEXT callbackCreateInfo{};
  callbackCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT; 
  callbackCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
  callbackCreateInfo.messageSeverity =  VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
  callbackCreateInfo.pfnUserCallback = vulkanInternalDebugMessageCallback;


  bstl::Vector<const char*> ext(6);
  platformGetRequiredVulkanExtensions(ext);
  ext.append("VK_KHR_get_physical_device_properties2");
  ext.append("VK_EXT_metal_surface");
  ext.append("VK_KHR_surface");
  ext.append("VK_EXT_debug_utils");
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
  instanceInfo.pNext = (void*)&callbackCreateInfo;
  VKCALL(vkCreateInstance(&instanceInfo, nullptr, &g_instance))
  return VULKAN_ERROR_NONE;
}


static void cleanup(){
  vkDestroyInstance(g_instance, nullptr);
}

void vulkanUpdate(){
  vulkanFlushInternalLogs();
}


int vulkanInitialize(){
  vulkanErrorsInit();
  vulkanCreateInstance();
  vulkanCreatePhysicalDevice(g_instance, g_gpu);
  vulkanCreateLogicalDevice(g_gpu);
  return 0;
}
