#include "application.h"

#include "engine.h"
#include "game.h"
#include "log.h"

static void loop(){
  while(!gExitFlag){

    LOGGING::logToStdout();
    LOGGING::flushLogs();
  }
}


void applicationStart(){
  LOGGING::init();  
  loop();
  LOGGING::logToStdout();
  LOGGING::flushLogs();
}
