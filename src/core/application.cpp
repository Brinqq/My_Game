#include "global.h"
#include "defines.h"

#include "application.h"
#include "engine.h"
#include "game.h"
#include "platform.h"

static void loop(){
  while(!gExitFlag){
    platformGlobalUpdate();
    testUpdate();
  }
}

void applicationStart(){
  platformInitialize();
  engineInit();
  loop();
}
