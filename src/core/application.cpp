#include "application.h"
#include "engine.h"
#include "game.h"
#include "log.h"
#include "platform.h"

static void loop(){
  while(!gExitFlag){
    
    LOGGING::logToStdout();
    LOGGING::flushLogs();
  }
}


void applicationStart(){
  LOGGING::init();  
  platformStartUp();
  loop();
  LOGGING::logToStdout();
  LOGGING::flushLogs();
}
