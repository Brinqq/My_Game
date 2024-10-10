#include "engine.h"
#include "window.h"
#include "vulkan.h"
#include "log.h"

void loop(){
  Window window;
  if(!window.validateVulkanLayer()) return;
  testTraingle();
  while(!window.exitFlag){
    window.update();
    LOGGING::logToOutputStdout();
    LOGGING::flushLogs();
  }
  
}
