#include "vulkan_pipline.h"
#include "vulkan_shader.h"
#include "vulkandefines.h"
#include "vulkanerr.h"

#include "log.h"


//TODO: Unhard code width height of window to obtain viewport
static const float w = 1920;
static const float h = 1080;

struct PipelineObjects{
  VulkShaderProgram shader;
};

static const VkDevice* gDeviceRef;
static PipelineObjects gPipelineObjects;
static VkPipelineLayout gPipelineLayout{};


VkPipelineShaderStageCreateInfo gShaderStageInfo[2]{};


static VkDynamicState enableDynamicStates[2] = {VK_DYNAMIC_STATE_SCISSOR, VK_DYNAMIC_STATE_VIEWPORT};

static void shader(){
  gPipelineObjects.shader = testShader(*gDeviceRef);
}

void initializeVulkanPipeline(const VkDevice& device){
  gDeviceRef = &device;
  shader();

  //Create and store creation info for shaders
  VkPipelineShaderStageCreateInfo pvs{};
  VkPipelineShaderStageCreateInfo pps{};
  pvs.stage = VK_SHADER_STAGE_VERTEX_BIT;
  pvs.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  pvs.module = gPipelineObjects.shader.vertex;
  pvs.pName = "main";
  pps.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
  pps.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  pps.module = gPipelineObjects.shader.fragment;
  pps.pName = "main";
  gShaderStageInfo[0] = pvs;
  gShaderStageInfo[1] = pps;

  //Dynamic state
  VkPipelineDynamicStateCreateInfo pds{};
  pds.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
  pds.dynamicStateCount = 2;
  pds.pDynamicStates = enableDynamicStates;

  //Vertex input stage
  VkPipelineVertexInputStateCreateInfo vis{};
  vis.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  vis.pVertexBindingDescriptions = nullptr;
  vis.vertexBindingDescriptionCount = 0;
  vis.pVertexAttributeDescriptions = nullptr;
  vis.vertexAttributeDescriptionCount = 0;

  //Input assembler stage
  VkPipelineInputAssemblyStateCreateInfo ias{};
  ias.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  ias.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  ias.primitiveRestartEnable = VK_TRUE;

  //view port and sissor initalization
  VkViewport vp{};
  VkRect2D sissor{};
  vp.x = 0;
  vp.y =0;
  vp.width = w;
  vp.height = h;
  vp.maxDepth = 1.0f;
  vp.minDepth = 0.0f;
  sissor.extent = {1920, 1080};
  sissor.offset = {0,0};
  VkPipelineViewportStateCreateInfo vpc{};
  vpc.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  vpc.scissorCount = 1;
  vpc.viewportCount = 1;
  vpc.pViewports =&vp;
  vpc.pScissors =&sissor;

  //Rastrizer State
  VkPipelineRasterizationStateCreateInfo rsc{};
  rsc.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  rsc.depthClampEnable = VK_FALSE;
  rsc.rasterizerDiscardEnable = VK_FALSE;
  rsc.polygonMode = VK_POLYGON_MODE_FILL;
  rsc.lineWidth = 1.0f;
  rsc.cullMode = VK_CULL_MODE_NONE;
  rsc.frontFace = VK_FRONT_FACE_CLOCKWISE;
  rsc.depthBiasEnable = VK_FALSE;
  rsc.depthBiasConstantFactor = 0.0f;
  rsc.depthBiasClamp = 0.0f;
  rsc.depthBiasSlopeFactor = 0.0f;

  //multisample
  VkPipelineMultisampleStateCreateInfo multisampling{};
  multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  multisampling.sampleShadingEnable = VK_FALSE;
  multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
  multisampling.minSampleShading = 1.0f;
  multisampling.pSampleMask = nullptr;
  multisampling.alphaToCoverageEnable = VK_FALSE;
  multisampling.alphaToOneEnable = VK_FALSE;

  //Color blending and depth stencil goes here
  
  //pipeline layout
  VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
  pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipelineLayoutInfo.setLayoutCount = 0;
  pipelineLayoutInfo.pSetLayouts = nullptr;
  pipelineLayoutInfo.pushConstantRangeCount = 0;
  pipelineLayoutInfo.pPushConstantRanges = nullptr;
  
  if(vkCreatePipelineLayout(*gDeviceRef, &pipelineLayoutInfo, nullptr, &gPipelineLayout) != VK_SUCCESS){
    LOG_ERROR("fuck");
  }



}
