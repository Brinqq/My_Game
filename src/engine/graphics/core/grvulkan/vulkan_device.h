#pragma once

#include "vulkandefines.h"

VKError VulkanNewPhysicalDevice(const VkInstance& instance, VkPhysicalDevice& device);
VKError VulkanNewLogicalDevice(const VkInstance& instance, const VkPhysicalDevice gpu, VkDevice& device);
void vulkanDeviceCleanup(const VkInstance& instance, VulkanDevice& device);

