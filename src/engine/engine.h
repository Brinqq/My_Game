#pragma once

#include "events/events.h"
#include "input//input.h"
#include "renderer.h"
#include "timing.h"

// Engine.h is only here to initialize and update subsystems that typical game engines would be responsable to handle.
// but because this is more of game then a engine, the gameplay code will interop with the subsystems directly rather
// than go through a engine interface.
  
namespace Engine{

inline int initialEngineSystems(){
  eventSystemInitialize();
  inputSystemInitialize();
  timingInitialize();
  rendererInitialize();
  return 0;
}

inline void EngineUpdateSystems(){
  inputSystemUpdate();
  frameTimingUpdate();
  drawScene();
}

}


void engineInit();
void testUpdate();
void engineDeinitialize();

    
