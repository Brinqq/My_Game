#include "vulkan.h"
#include "platform.h"
#include "vulkanerror.h"
#include "log.h"

#include "bstl/vector.h"


static VkInstance g_instance;

static int vulkanCreateInstance(){
  VkApplicationInfo appInfo{};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = "Game";
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.pEngineName = "engine";
  appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.apiVersion = VK_API_VERSION_1_0;

  VkInstanceCreateInfo instanceInfo{};
  instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  instanceInfo.pApplicationInfo = &appInfo;
  instanceInfo.enabledLayerCount = 0;
  instanceInfo.ppEnabledLayerNames = nullptr;
  instanceInfo.flags = PLATFORM_VULKAN_REQUIRED_INSTANCE_FLAGS;

  bstl::Vector<const char*> ext;
  platformGetRequiredVulkanExtensions(ext);
  instanceInfo.enabledExtensionCount = ext.size();
  instanceInfo.ppEnabledExtensionNames = ext.data();

  VKCALL(vkCreateInstance(&instanceInfo, nullptr, &g_instance))
  return 0;
}

int vulkanInitialize(){
  vulkanCreateInstance();
  return 0;
}
