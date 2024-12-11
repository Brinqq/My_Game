#include "input.h"
#include "log.h"
#include "keycodes.h"

#include "events/events.h"

#include <vector>

//TODO: incoprate with the event system if or when i create that

struct KeyInfo{
  int code;
  int action;
  int modifier;
};

//TODO: find a better container or method so we are not reorganizing the buffer every time we release a key
std::vector<int> keyBuffer; 

#if _WINDOW_API_GLFW
int inputSystemInitialize(){
  keyBuffer.reserve(15);
  LOG_INFO("Input system intialized successfully!");
  return 0;
}
void windowKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods){
  if(action == 2){return;}
  if(action == 1){
    keyBuffer.push_back(key);
    return;
  }

  if(action == 0){
    int i = 0;
    for(int lfk : keyBuffer){
      if(lfk == key){
        keyBuffer.erase(keyBuffer.begin() + i);
        break;
      }
      i++;
    }
  }

}

void inputSystemUpdate(){
  for(int& key : keyBuffer){
    if(key == INPUT_KEY_W){staticEventTick(KEY_PRESSED_W_EVENT);continue;}
    if(key == INPUT_KEY_A){staticEventTick(KEY_PRESSED_A_EVENT);continue;}
    if(key == INPUT_KEY_S){staticEventTick(KEY_PRESSED_S_EVENT);continue;}
    if(key == INPUT_KEY_D){staticEventTick(KEY_PRESSED_D_EVENT);continue;}
    // if(key == INPUT_KEY_ESC){staticEventTick(KEY_PRESSED_ESC_EVENT);continue;}
  }
}

void inputSystemShutDown(){
  LOG_INFO("shutting down input system");
}

#endif
