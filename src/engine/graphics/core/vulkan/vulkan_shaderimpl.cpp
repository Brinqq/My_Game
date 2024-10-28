
#include "vulkan/vulkan.h"
#include "vulkanerr.h"
#include "vulkan_shaderimpl.h"
#include "error.h"
#include "log.h"

#include <fstream>
#include <filesystem>


static int readInShaderByteCode(const std::string filePath, std::vector<char>& buffer){
  std::ifstream file(filePath,std::ios::ate | std::ios::binary);
  if(!file.is_open()){
    return 1;
  }

  size_t bytesSize = (size_t) file.tellg();
  buffer.reserve(bytesSize);
  file.seekg(0);
  file.read(buffer.data(), bytesSize);
  file.close();
  return 0;
}

VkShaderModule createShaderModule(const VkDevice& device, const std::vector<char>& buffer){
  VkShaderModuleCreateInfo sci{};
  VkShaderModule shader;
  sci.sType =  VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  sci.codeSize = buffer.size();
  sci.pCode = reinterpret_cast<const uint32_t*>(buffer.data());
  vkCreateShaderModule(device, &sci, nullptr, &shader);
  return shader;
}

VulkShaderProgram testShader(const VkDevice& device){
  std::vector<char> vertBuffer;
  std::vector<char> fragBuffer;

  if(readInShaderByteCode("bin/shaders/spv/tt_vert.spv", vertBuffer)){
    LOG_CRITICAL("Couldnt load file");
    programErrorOut();
  }
  if(readInShaderByteCode("bin/shaders/spv/tt_frag.spv", fragBuffer)){
    LOG_CRITICAL("Couldnt load file");
    programErrorOut();
  }

  VulkShaderProgram ret;
  // ret.vertex = createShaderModule(device, vertBuffer);
  // ret.fragment = createShaderModule(device, fragBuffer);
  //

  return ret;
}
