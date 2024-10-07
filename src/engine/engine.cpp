#include "engine.h"
#include "window.h"

void loop(){
  Window window;
  while(!window.exitFlag){
    window.update();
  }
  
}
