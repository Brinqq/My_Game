#include "vulkan_renderpass.h"
#include "vulkanerr.h"
#include "vulkandefines.h"



int initializeRenderpass(VulkanContext& context){
  VkAttachmentDescription colorAttachment{};
  VkAttachmentReference colorAttachmentRef{};
  VkSubpassDescription subpassDescription{};
  VkRenderPassCreateInfo rp{};

  colorAttachment.format = context.presentationInfo.format.format;
  colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
  colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
  colorAttachment.flags = 0;

  colorAttachmentRef.attachment = 0;
  colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
  subpassDescription.colorAttachmentCount =1;
  subpassDescription.pColorAttachments = &colorAttachmentRef;
  
  rp.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  rp.pAttachments = &colorAttachment;
  rp.attachmentCount = 1;
  rp.subpassCount = 1;
  rp.pSubpasses = &subpassDescription;


 
  VKCALL(vkCreateRenderPass(context.device, &rp, nullptr, &context.renderPass));
  return 0;
}

static void cleanup(VulkanContext context){
  vkDestroyRenderPass(context.device, context.renderPass, nullptr);
}

