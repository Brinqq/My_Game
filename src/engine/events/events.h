#pragma once

#define STATIC_EVENTS_COUNT 5

enum StaticEventType{
  KEY_PRESSED_W_EVENT,
  KEY_PRESSED_S_EVENT,
  KEY_PRESSED_A_EVENT,
  KEY_PRESSED_D_EVENT,
  KEY_PRESSED_ESC_EVENT 
};

int eventSystemInitialize();
int subscribeToStaticEvent(const StaticEventType type, void(*callback)());
void staticEventTick(StaticEventType type);

