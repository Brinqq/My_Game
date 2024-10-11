#include "vulkan.h"
#include "global.h"
#include "log.h"
#include "vulkanerr.h"

#include "vulkan/vulkan.h"
#include "vulkan/vulkan_core.h"
#include "GLFW/glfw3.h"



struct GlfwInfo{
  uint32_t glfwExtensionCount = 0;
  const char** glfwExtension;
};

struct QueueFamilies{

};

static VkInstance gInstanceHandle;
static VkPhysicalDevice gDevice;
static VkApplicationInfo gInfo;
static QueueFamilies gQueues;
static VkResult result;
static GlfwInfo glfwInfo;

std::array<const char*, 1> validationLayers= {
  "VK_LAYER_KHRONOS_validation"
};


int validateLayerAvailability(){
  int requestedLayers = validationLayers.size();
  int layersFound = 0; 
  uint32_t layerCount;
  vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
  VkLayerProperties* layerProperties = (VkLayerProperties*)alloca(sizeof(VkLayerProperties) * layerCount);
  vkEnumerateInstanceLayerProperties(&layerCount, layerProperties);
    for(const char* layer: validationLayers){
      for(int i = 0; i < layerCount;i++){
        if(strcmp(layer, layerProperties[i].layerName) == 0){
        layersFound++;
        break;
    }
  }
}
if(layersFound == requestedLayers) {return 1;}
return 0;

}

void createVulkanInstance(){
  if(!validateLayerAvailability()){
    LOG(CRITICAL, "Unable to find all request validation layers");
    gExitFlag = true;

  }

  //setting up app info struct to required for vk init function
  gInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO; 
  gInfo.pApplicationName = "Game";
  gInfo.pEngineName = "none";
  gInfo.apiVersion = VK_API_VERSION_1_0;
  gInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  gInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);

  //Vulkan Requires extensions to interface with windows api so we use glfw function that returns that
  glfwInfo.glfwExtension = glfwGetRequiredInstanceExtensions(&glfwInfo.glfwExtensionCount);

  //vk init struct also Requires a create info struct
  VkInstanceCreateInfo instanceInfo{};
  instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  instanceInfo.pApplicationInfo = &gInfo;
  instanceInfo.enabledLayerCount = validationLayers.size();
  instanceInfo.ppEnabledLayerNames = validationLayers.data();
  
  //Required extension setup for mac to avoid driver protability error
  std::vector<const char*> extensions;
  for(uint32_t i = 0; i < glfwInfo.glfwExtensionCount; i++){
    extensions.emplace_back(glfwInfo.glfwExtension[i]);
  }
  extensions.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
  instanceInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
  instanceInfo.enabledExtensionCount = (uint32_t)extensions.size();
  instanceInfo.ppEnabledExtensionNames = extensions.data();

  //finally we initialize the vulkan instance passing the two structs we filled
  VKCALL(vkCreateInstance(&instanceInfo, nullptr, &gInstanceHandle))

}

void queryAndInitPhysicalDevice(){
  uint32_t deviceCount;
  VKCALL(vkEnumeratePhysicalDevices(gInstanceHandle, &deviceCount, nullptr))
  if(deviceCount == 0){
    LOG(CRITICAL, "Could not find suitble graphics device with vulkan support!");
    gExitFlag = true;
  }
  VKCALL(vkEnumeratePhysicalDevices(gInstanceHandle, &deviceCount, &gDevice))
  
}

void initQueueFamilies(){
  uint32_t queueCount = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(gDevice, &queueCount, nullptr);

}


void cleanup(){
  vkDestroyInstance(gInstanceHandle, nullptr);
}


void testTraingle(){
  createVulkanInstance();
  queryAndInitPhysicalDevice();
  initQueueFamilies();
}



