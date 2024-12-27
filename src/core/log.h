#pragma once

#include "global.h"
#include "defines.h"
#include <stdarg.h>

inline const char* logArr[8] = {"\033[31mCIRTICAL: ", "\033[33mERROR: ", "\033[32mWARN: ", "\033[36mINFO: ", "\033[34mTRACE: ", "\033[37mDEBUG: ",  "\033[1;38;5;128mPG: ", "\033[1;38;5;208mSYSTEM: "};

enum LogLevel{
  LOG_LEVEL_CRITICAL = 0,
  LOG_LEVEL_ERROR = 1,
  LOG_LEVEL_WARN = 2,
  LOG_LEVEL_INFO = 3,
  LOG_LEVEL_TRACE = 4,
  LOG_LEVEL_DEBUG = 5,
  LOG_LEVEL_PG = 6,
  LOG_LEVEL_SYS = 7
};

#if __DEBUG
#define LOG_WARN_ENABLE 1
#define LOG_INFO_ENABLE 1
#define LOG_TRACE_ENABLE 1
#define LOG_DEBUG_ENABLE 1
#define LOG_PG_ENABLE 1
#define LOG_SYS_ENABLE 1
#else
#define LOG_WARN_ENABLE 0
#define LOG_INFO_ENABLE 0
#define LOG_TRACE_ENABLE 0
#define LOG_DEBUG_ENABLE 0
#define LOG_PG_ENABLE 0
#define LOG_SYS_ENABLE 0
#endif

#define MAX_LOG_MSG_LENGTH 500 //200 characters max

inline void logToConsole(LogLevel level, const char* pMsg, ...){
  char msgBuffer[MAX_LOG_MSG_LENGTH];
  char outMsg[MAX_LOG_MSG_LENGTH];
  memset(msgBuffer, 0, MAX_LOG_MSG_LENGTH);
  memset(outMsg, 0, MAX_LOG_MSG_LENGTH);
  va_list arg;
  va_start(arg, pMsg);
  if(vsnprintf(msgBuffer, MAX_LOG_MSG_LENGTH, pMsg, arg) > MAX_LOG_MSG_LENGTH){
    printf("LOG ERROR: Log truncated due to reaching maximum character limit, characters allowed per log: %i\n", MAX_LOG_MSG_LENGTH);
  };
  va_end(arg);
  snprintf(outMsg, MAX_LOG_MSG_LENGTH, "%s%s\033[0m\n", logArr[level], msgBuffer);
  printf("%s", outMsg);
}

#define LOG_CRITICAL(msg, ...) logToConsole(LOG_LEVEL_CRITICAL, msg, ##__VA_ARGS__)
#define LOG_ERROR(msg, ...) logToConsole(LOG_LEVEL_ERROR, msg, ##__VA_ARGS__)


#if LOG_WARN_ENABLE
#define LOG_WARN(msg, ...) logToConsole(LOG_LEVEL_WARN, msg, ##__VA_ARGS__)
#else
#define LOG_WARN(msg, ...)
#endif

#if LOG_INFO_ENABLE
#define LOG_INFO(msg, ...) logToConsole(LOG_LEVEL_INFO, msg, ##__VA_ARGS__)
#else
#define LOG_INFO(msg, ...)
#endif

#if LOG_TRACE_ENABLE
#define LOG_TRACE(msg, ...) logToConsole(LOG_LEVEL_TRACE, msg, ##__VA_ARGS__)
#else
#define LOG_TRACE(msg, ...) 
#endif

#if LOG_DEBUG_ENABLE
#define LOG_DEBUG(msg, ...) logToConsole(LOG_LEVEL_DEBUG, msg, ##__VA_ARGS__)
#else
#define LOG_DEBUG(msg, ...)
#endif

#if LOG_PG_ENABLE
#define LOG_PG(msg, ...) logToConsole(LOG_LEVEL_PG, msg, ##__VA_ARGS__)
#else
#define LOG_DEBUG(msg, ...)
#endif

#if LOG_SYS_ENABLE
#define LOG_SYS(msg, ...) logToConsole(LOG_LEVEL_SYS, msg, ##__VA_ARGS__)
#else
#define LOG_DEBUG(msg, ...)
#endif



