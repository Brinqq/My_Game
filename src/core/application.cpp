#include "global.h"
#include "application.h"
#include "engine.h"
#include "game.h"
#include "log.h"
#include "platform.h"

static void loop(){
  while(!gExitFlag){
    platformGlobalUpdate();
    LOGGING::update();
  }
}

void applicationStart(){
  LOGGING::init();  
  platformInitialize();

  engineInit();
  LOGGING::update();
  loop();
}
