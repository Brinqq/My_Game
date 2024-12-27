#include "global.h"
#include "defines.h"

#include "application.h"
#include "engine.h"
#include "game.h"
#include "platform.h"
#include "events/events.h"
#include "renderer.h"

static void exitProgramCallback(){
  gExitFlag = 1;
}

static void loop(){
  while(!gExitFlag){
    platformGlobalUpdate();
    Engine::EngineUpdateSystems();
    Game::gameUpdate();
  }
}

void applicationStart(){
  platformInitialize();
  if(Engine::initialEngineSystems()){return;};
  Game::gameInitialize();
  subscribeToStaticEvent(KEY_PRESSED_ESC_EVENT, exitProgramCallback);
  loop();
}
