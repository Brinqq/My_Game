#pragma once

#include "global.h"
#include "vulkan/vulkan.h"
#include "log.h"

struct VulkShaderProgram{
  VkShaderModule fragment;
  VkShaderModule vertex;
};


VulkShaderProgram testShader(const VkDevice& device);


