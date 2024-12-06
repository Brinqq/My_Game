#include "timing.h"
#include "log.h"

//TODO: intergrate with custom timing lib when created
#include <chrono>

#define MAX_TIMEPOINTS 20

std::array<std::chrono::time_point<std::chrono::high_resolution_clock>, MAX_TIMEPOINTS> timepoints;
static int timePointCount = 0;

struct FrameTimingState{
  std::chrono::duration<float, std::milli> lastFrameDuration;
  std::chrono::time_point<std::chrono::high_resolution_clock> startTP;
};

struct GlobalTimingState{
  std::chrono::time_point<std::chrono::high_resolution_clock> startTP;

};

static FrameTimingState g_frameTimerState;
static GlobalTimingState g_globalTimerState;

void timingInitialize(){
  g_frameTimerState.startTP = std::chrono::high_resolution_clock::now();
  g_globalTimerState.startTP = std::chrono::high_resolution_clock::now();
}

void frameTimingUpdate(){
  g_frameTimerState.lastFrameDuration = std::chrono::high_resolution_clock::now()- g_frameTimerState.startTP;
  g_frameTimerState.startTP = std::chrono::high_resolution_clock::now();
}

float frameTimingGetElapsedTime(){
  return 3.0f;
}

inline float frameTimingGetLastFrameTime(){ return g_frameTimerState.lastFrameDuration.count();}

int globalTimePointCheck(int timepoint, float durationCheck){
  if(std::chrono::duration<float>(std::chrono::high_resolution_clock::now() - timepoints[timepoint]).count() >= durationCheck){
    timepoints[timepoint] = std::chrono::high_resolution_clock::now();
    return true;
  }
return false;
}

int globalTimingNewTimePoint(){
  int c = timePointCount;
  if( c == timepoints.max_size()){
    LOG_CRITICAL("Max time points reached");
    return 9999;
  }
  int index = c;
  timepoints[c] =  std::chrono::high_resolution_clock::now();
  timePointCount++;
  return index;
}
