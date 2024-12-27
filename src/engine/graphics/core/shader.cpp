#include "shader.h"
#include "graphicdefines.h"
#include "gldevice.h"
#include "log.h"

#include <filesystem>
#include <array>
#include "unordered_map"

enum ShaderType{
  VERTEX_SHADER = 0,
  FRAGMENT_SHADER = 1
};

std::unordered_map<std::string, int> compiledShaders;

namespace Graphics{

int shadersInit(const char* pResourcePath, RenderDevice& device){ 
  unsigned int id;
  std::filesystem::path path = pResourcePath;
  std::array<std::filesystem::path, 2> subDirectories{path, path};
  subDirectories[VERTEX_SHADER].append("vertex/");
  subDirectories[FRAGMENT_SHADER].append("fragment/");
  int type = 0;
  for(std::filesystem::path& path : subDirectories ){
    std::filesystem::directory_iterator itr(path);
    for(const std::filesystem::directory_entry& entry: itr){
      if(entry.is_regular_file()){
        if(device.CompileShaderFromSource(id, entry.path().c_str(), type)){return 1;};
        compiledShaders.emplace(entry.path().filename().string(), id);
      }
    }
    type++;
  }

  return 0;
}

}
