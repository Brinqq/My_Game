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

void initializeVulkanPipeline(const VkDevice& device, const VkRenderPass& renderPass, VkPipeline& pipeline){
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
  vis.pVertexBindingDescriptions = &gVertexDescription;
  vis.vertexBindingDescriptionCount = 0;
  vis.pVertexAttributeDescriptions = &gAttribDescription;
  vis.vertexAttributeDescriptionCount = 0;

  //Input assembler stage
  VkPipelineInputAssemblyStateCreateInfo ias{};
  ias.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  ias.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  ias.primitiveRestartEnable = VK_FALSE;

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
  VkPipelineColorBlendAttachmentState colorBlendAttachment{};
  colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
  colorBlendAttachment.blendEnable = VK_FALSE;
  colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
  colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
  colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
  colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
  colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
  colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional


  VkPipelineColorBlendStateCreateInfo colorStateInfo{};
  colorStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
  colorStateInfo.logicOpEnable = VK_FALSE;
  colorStateInfo.logicOp = VK_LOGIC_OP_COPY;
  colorStateInfo.attachmentCount = 1;
  colorStateInfo.pAttachments = &colorBlendAttachment;

  //pipeline layout
  VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
  pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipelineLayoutInfo.setLayoutCount = 0;
  pipelineLayoutInfo.pSetLayouts = nullptr;
  pipelineLayoutInfo.pushConstantRangeCount = 0;
  pipelineLayoutInfo.pPushConstantRanges = nullptr;
  
  VKCALL(vkCreatePipelineLayout(*gDeviceRef, &pipelineLayoutInfo, nullptr, &gPipelineLayout))

  VkGraphicsPipelineCreateInfo pipelineInfo{};
  pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  pipelineInfo.stageCount = 2;
  pipelineInfo.pStages = gShaderStageInfo;
  pipelineInfo.pVertexInputState = &vis;
  pipelineInfo.pInputAssemblyState = &ias;
  pipelineInfo.pViewportState = &vpc;
  pipelineInfo.pRasterizationState = &rsc;
  pipelineInfo.pMultisampleState = &multisampling;
  pipelineInfo.pColorBlendState = &colorStateInfo;
  pipelineInfo.pDepthStencilState = nullptr;
  pipelineInfo.pDynamicState = &pds;
  pipelineInfo.layout = gPipelineLayout;
  pipelineInfo.renderPass = renderPass;
  pipelineInfo.subpass = 0;
  pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
  pipelineInfo.basePipelineIndex = 1;
  VKCALL(vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1 , &pipelineInfo, nullptr, &pipeline));

  }

void destroyPipeline(const VkDevice& device, const VkPipeline& pipeline){
  
  vkDestroyPipeline(device, pipeline, nullptr);
}



