#include "engine.h"
#include "window.h"
#include "vulkan.h"
#include "log.h"
#include "error.h"


void engineDeinitialize(){

}

void engineInit(){
  VulkanTestStart();
}

void testUpdate(){
  VulkanTestUpdate();
}

