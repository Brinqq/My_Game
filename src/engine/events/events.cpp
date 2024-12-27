#include "events.h"
#include "bstl/array.h"

#include <vector>

typedef void(*callback)();

struct StaticEvent{
  std::vector<callback> callbacks;
};

// BSTL::Array<StaticEvent, STATIC_EVENTS_COUNT> staticEvents;
std::vector<StaticEvent> staticEvents;

int eventSystemInitialize(){
  staticEvents.reserve(STATIC_EVENTS_COUNT);
  return 0;
}

int subscribeToStaticEvent(const StaticEventType type, const callback function){
  staticEvents[type].callbacks.emplace_back(function);
  return 0;
}

void staticEventTick(StaticEventType type){
  for(callback& c: staticEvents[type].callbacks){
    c();
  }
}
