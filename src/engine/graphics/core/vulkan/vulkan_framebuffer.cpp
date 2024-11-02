#include "vulkan_framebuffer.h"


#include "vulkandefines.h"
#include "vulkanerr.h"

#include <vector>

void learnFrameBuffer(const VulkanContext& context){
  gFramebuffers.resize(context.swapImageViews.capacity());
  for(int i = 0; i < context.swapImageViews.capacity();i++){
    VkImageView attachments[] = {context.swapImageViews[i]};
    VkFramebufferCreateInfo framebufferInfo{};
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.renderPass = context.renderPass;
    framebufferInfo.attachmentCount = 1;
    framebufferInfo.pAttachments = attachments;
    framebufferInfo.width = context.presentationInfo.extent.width;
    framebufferInfo.height = context.presentationInfo.extent.height;
    framebufferInfo.layers = 1;
    VKCALL(vkCreateFramebuffer(context.device, &framebufferInfo, nullptr, &gFramebuffers[i]))
  }
  
}



