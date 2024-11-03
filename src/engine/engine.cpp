#include "engine.h"
#include "window.h"
#include "vulkan.h"
#include "log.h"
#include "error.h"

void engineInit(){
  if(vulkanInitialize()){
    LOG_CRITICAL("Failed to initialize Vulkan. No other backend api available");
    applicationThrowCritcalError();
  }
}

void testUpdate(){
}

