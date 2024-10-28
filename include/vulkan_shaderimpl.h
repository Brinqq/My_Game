#pragma once

#include "vulkan/vulkan.h"

struct VulkShaderProgram{
  VkShaderModule fragment;
  VkShaderModule vertex;
};


VulkShaderProgram testShader(const VkDevice& device);


