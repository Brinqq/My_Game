#pragma once

void timingInitialize();
void frameTimingUpdate();
float frameTimingGetElapsedTime();
float frameTimingGetLastFrameTime();
int globalTimingNewTimePoint();
int globalTimePointCheck(int timepoint, float durationCheck);
