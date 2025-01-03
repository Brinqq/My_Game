#include "renderer.h"


//tmp
#include "vulkan.h"
#include "stdio.h"


static void updateVulkan(){
  vulkanUpdate();
  
}

static void setupVulkan(){
  vulkanInitialize();
}

//







//--------------------------------------------------------
namespace Engine{
int rendererInitialize(){
  setupVulkan(); 
  return 1;
}


static void bindRenderableToScene(){
}

void addRenderObject(){

};


void drawScene(){
  updateVulkan(); 
}


}
