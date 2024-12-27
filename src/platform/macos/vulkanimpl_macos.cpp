#include "platform.h"
#include "defines.h"

#include "bstl/vector.h"

#define PLATFORM_VULKAN_EXT_COUNT_MAX 3

void platformGetRequiredVulkanExtensions(bstl::Vector<const char*>& extensions){
  uint32_t count;
  const char** ppNames;
  ppNames = glfwGetRequiredInstanceExtensions(&count);
  for(int i = 0; i < count; i++){
    extensions.append(ppNames[i]);
  }
  extensions.append(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
}
