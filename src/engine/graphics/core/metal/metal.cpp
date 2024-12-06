#if __APPLE__
#include "metal.h"
#include "metaldefines.h"
#include "log.h"

#include <fstream>
#include <string>

static MTL::Device* pDevice;
static MTL::RenderPipelineState* pPipeline;

static int readinshader(std::string& shader, const char* filepath){
  std::string line;
  std::ifstream stream(filepath);
  if(!stream.is_open()){
    LOG_CRITICAL("failed to open shader at path: %s", filepath);
    return 1;


    
  }
  while(getline(stream, line)){
    shader.append(line + "\n");
  }
  return 0;
}


static int initrasterizerstate(){
  MTL::RenderPipelineDescriptor* rpd = MTL::RenderPipelineDescriptor::alloc()->init();
  return 0;
}



void metalStart(){ 
  //metal device
  pDevice = MTLCreateSystemDefaultDevice();
  if(!pDevice){
  LOG_CRITICAL("device failed to init");

  //read in shader to  string
  std::string vs;
  std::string ps;
  readinshader(vs, "resources/shaders/metal/tri_vs.metal");
  readinshader(ps, "resources/shaders/metal/tri_ps.metal");

  //compile shader
  if(initrasterizerstate()) return;

  }
}

void metalUpdate(){

}

void metalDeinit(){
  pDevice->release();
}

#endif
