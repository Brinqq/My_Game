#include "vulkanrenderpass.h"
#include "vulkanerr.h"
#include "vulkandefines.h"

//TODO: Make confiurable renderpass and or defaults

static unsigned int g_isRenderPassSystemInit = 0;
static VulkanRenderPassConfig g_renderPassConfig;

struct VulkanDefaultRenderPass{
  const VkAttachmentDescription colorAttachment{0, g_renderPassConfig.format, VK_SAMPLE_COUNT_1_BIT, VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_STORE, 
  VK_ATTACHMENT_LOAD_OP_DONT_CARE, VK_ATTACHMENT_STORE_OP_DONT_CARE, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR};
  const VkAttachmentReference colorAttachmentRef{0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL};
  const VkSubpassDescription subpassDescription{0, VK_PIPELINE_BIND_POINT_GRAPHICS, 0, 0, 1, &colorAttachmentRef};
  const VkRenderPassCreateInfo rp{VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO, 0, 0 ,1, &colorAttachment, 1 , &subpassDescription};
};

void vulkanInitializeRenderpassSystem(const VulkanRenderPassConfig& config){
  g_renderPassConfig.format = config.format;
  g_isRenderPassSystemInit = 1;
};

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


int VulkanNewRenderPass(const VkDevice& device, VkRenderPass& renderpass, const VulkanRenderPassType type){
  if(g_isRenderPassSystemInit)return 1; 
  if(type == VULKAN_RENDERPASS_TYPE_DEFAULT){
    VulkanDefaultRenderPass renderpassConfig;
    VKCALL(vkCreateRenderPass(device, &renderpassConfig.rp, nullptr, &renderpass));
    return 0; 
  };
  return 1;
}

static void cleanup(VulkanContext context){
  vkDestroyRenderPass(context.device, context.renderPass, nullptr);
}

