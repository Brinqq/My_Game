#include "global.h"
#include "defines.h"

#include "application.h"
#include "engine.h"
#include "game.h"
#include "platform.h"
#include "events/events.h"

static void exitProgramCallback(){
  gExitFlag = 1;
}

static void loop(){
  while(!gExitFlag){
    platformGlobalUpdate();
    testUpdate();
  }
}

void applicationStart(){
  platformInitialize();
  engineInit();
  subscribeToStaticEvent(KEY_PRESSED_ESC_EVENT, exitProgramCallback);
  loop();
  //cleanup()
  engineDeinitialize();
}
