#pragma once

void* scratchBufferCreate(const long bytes);
void scratchBufferDestroy(void* pBuffer);
void* scratchBufferAllocate(void* const pBuffer, const long bytes);
void scratchBufferFlush(const void* pBuffer);
long scratchBufferAvailableBytes(const void* const pBuffer);



