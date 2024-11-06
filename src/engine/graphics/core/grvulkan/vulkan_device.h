#pragma once

#include "vulkandefines.h"

class VulkanDevice{

};

VKError VulkanNewPhysicalDevice(const VkInstance& instance, VkPhysicalDevice& device);
void vulkanDeviceCleanUp();
