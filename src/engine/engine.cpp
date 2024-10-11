#include "engine.h"
#include "window.h"
#include "vulkan.h"
#include "log.h"

void loop(){
  LOGGING::init();
  Window window;
  if(!window.validateVulkanLayer()) return;
  testTraingle(window);
  LOGGING::logToStdout();
  LOGGING::flushLogs();
  while(!gExitFlag){
    window.update();
    LOGGING::logToStdout();
    LOGGING::flushLogs();
  }
}
