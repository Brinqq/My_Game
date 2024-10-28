#include "global.h"
#include "application.h"
#include "engine.h"
#include "game.h"
#include "log.h"
#include "platform.h"

static void loop(){
  while(!gExitFlag){
    platformGlobalUpdate();
  }
}

void applicationStart(){
  platformInitialize();
  engineInit();
  loop();
}
