#include "vulkanpipeline.h"

#include "vulkanrenderpass.h"
#include "vulkanview.h"



VKError VulkanPipeline::initialize(){;
  VulkanRenderPassConfig config{viewHandle.format()};
  vulkanInitializeRenderpassSystem(config);
  initializeRenderpasses();
  LOG_INFO("Vulkan pipeline initialized!");
  return VULKAN_SUCCESS;
}

void VulkanPipeline::cleanup(){
  for(const VkRenderPass& renderpass : renderpasses){
    vkDestroyRenderPass(deviceHandle, renderpass, nullptr);
  }
}

int VulkanPipeline::initializeRenderpasses(){
  VkRenderPass defaultRenderPass;
  if(VulkanNewRenderPass(deviceHandle, defaultRenderPass, VULKAN_RENDERPASS_TYPE_DEFAULT) == 0){
    renderpasses.push_back(defaultRenderPass);
  }else return 1;
  
  return 0;
}

void VulkanPipeline::shaderStageBind()const{
  // VkPipelineShaderStageCreateInfo pvs{};
  // VkPipelineShaderStageCreateInfo pps{};
  // pvs.stage = VK_SHADER_STAGE_VERTEX_BIT;
  // pvs.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  // pvs.module = gPipelineObjects.shader.vertex;
  // pvs.pName = "main";
  // pps.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
  // pps.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  // pps.module = gPipelineObjects.shader.fragment;
  // pps.pName = "main";
  // gShaderStageInfo[0] = pvs;
  // gShaderStageInfo[1] = pps;
}

