#include "vulkanpipeline.h"

struct DynamicState{
  const VkDynamicState vp = VK_DYNAMIC_STATE_VIEWPORT;
  const VkDynamicState scissor = VK_DYNAMIC_STATE_SCISSOR;
};

const DynamicState g_dyanmicState{};

int vulkanPipelineCreate(){
  VkPipelineDynamicStateCreateInfo dsi{};
  VkPipelineVertexInputStateCreateInfo vissi{};
  VkPipelineInputAssemblyStateCreateInfo iasci{};
  VkPipelineRasterizationStateCreateInfo rsci{};
  VkViewport vp{};
  VkRect2D sissor;

  dsi.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
  dsi.pDynamicStates = (VkDynamicState*)&g_dyanmicState;
  dsi.dynamicStateCount = 2;

  vissi.sType  = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  vissi.pVertexBindingDescriptions = nullptr;
  vissi.vertexBindingDescriptionCount = 0;
  vissi.pVertexAttributeDescriptions = nullptr;
  vissi.vertexAttributeDescriptionCount = 0;

  iasci.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  iasci.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  iasci.primitiveRestartEnable = VK_FALSE;

  vp.x = 0;
  vp.y = 0;
  vp.width = 1920;
  vp.height = 1080;
  vp.minDepth = 0;
  vp.maxDepth = 1;

  sissor.offset = {0, 0};

  // view port dynamic state init
  //MUST DO: make no defualt when done
  sissor.extent = VkExtent2D{};
  //

  rsci.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  rsci.depthClampEnable = VK_FALSE;
  rsci.rasterizerDiscardEnable = VK_FALSE;
  rsci.polygonMode = VK_POLYGON_MODE_FILL;
  rsci.lineWidth = 1.0f;
  rsci.cullMode = VK_CULL_MODE_BACK_BIT;
  rsci.frontFace = VK_FRONT_FACE_CLOCKWISE;
  rsci.depthBiasEnable = VK_FALSE;
  rsci.depthBiasConstantFactor = 0.0f; // Optional
  rsci.depthBiasClamp = 0.0f; // Optional
  rsci.depthBiasSlopeFactor = 0.0f; // Optional;
  
  VkPipelineMultisampleStateCreateInfo multisampling{};
  multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  multisampling.sampleShadingEnable = VK_FALSE;
  multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
  multisampling.minSampleShading = 1.0f; // Optional
  multisampling.pSampleMask = nullptr; // Optional
  multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
  multisampling.alphaToOneEnable = VK_FALSE; // Optional

                                    
          

  return 0; 
}
