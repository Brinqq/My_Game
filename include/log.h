#pragma once
#include "global.h"

#define INFO 689
#define WARN 690
#define ERROR 691
#define CRITICAL 692

#define CRITICAL_OUTPUT_FORMAT "\033[1m\033[31m" << "CRITICAL" << ": " << "\033[0m"
#define ERROR_OUTPUT_FORMAT "\033[1m\033[33m" << "ERROR" << ": " << "\033[0m"
#define WARN_OUTPUT_FORMAT "\033[1m\033[34m" << "WARN" << ": " << "\033[0m"
#define INFO_OUTPUT_FORMAT "\033[1m\033[36m" << "INFO" << ": " << "\033[0m"
#define UNKNOWN_OUTPUT_FORMAT "\033[1m\033[35m" << "UNKOWN LOG TYPE" << ": " << "\033[0m"

#define LOG(severity, msg)  LOGGING::addLog(severity, msg);
#define lOG_TICK()

namespace LOGGING{

struct LogMessage{
  const char* pMsg;
  int severity;
};

inline std::vector<LogMessage> logArray;

inline void init(){
  logArray.reserve(40);
}

inline void addLog(int severity, const char* msg){
  logArray.emplace_back(LogMessage{msg, severity});
}

inline void logToStdout(){
  for(const LogMessage& log: logArray ){
    switch(log.severity){
    case INFO:
      std::cout<< INFO_OUTPUT_FORMAT << log.pMsg << std::endl;
      break;
    case WARN:
      std::cout<< WARN_OUTPUT_FORMAT << log.pMsg << std::endl;
      break;
    case ERROR:
      std::cout<< ERROR_OUTPUT_FORMAT << log.pMsg << std::endl;
      break;
    case CRITICAL:
      std::cout<< CRITICAL_OUTPUT_FORMAT << log.pMsg << std::endl;
      break;
    default:
        std::cout<< UNKNOWN_OUTPUT_FORMAT << log.pMsg << std::endl;
        break;
    }
  }
}

inline void flushLogs(){
  logArray.clear();
}

inline void update(){
  logToStdout();
  flushLogs();
}

}
