#include "vulkanerror.h"

#include <string>
#include <chrono>
#include <fstream>
#include <filesystem>

#include "core/assertions.h"

#define VULKAN_MAX_INTERNAL_LOG_BYTES 1000

static std::string gInternalLog;
static std::ofstream gInternalLogFileStream;
const char* gInternalLogFilePath = "bin/logs/vulkan_debug_logs.txt";
static int gTimeLogLength;

void vulkanFlushInternalLogs(){
  if(gInternalLog.empty()) return;
  gInternalLogFileStream.open(gInternalLogFilePath, std::ios::out | std::ios::app);

  cassert(gInternalLogFileStream.is_open());
  gInternalLogFileStream.write("\n\n", 2);
  int wrap = gInternalLog.length()/7;
  for(std::string::iterator itr = gInternalLog.begin() + (17 + wrap); itr + wrap < gInternalLog.end(); itr += wrap){
    while(*itr != ' '){
    if(*itr  == '\0') {break;}
      itr++;
    }
    itr = gInternalLog.insert(itr, '\n');
  }

  gInternalLogFileStream.write(gInternalLog.c_str(), gInternalLog.size());
  
  gInternalLogFileStream.close();
  gInternalLog.clear();
}


VkBool32 vulkanInternalDebugMessageCallback(
  VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
  VkDebugUtilsMessageTypeFlagsEXT messageTypes,
  const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
  void* pUserData){
  if(gInternalLog.size() >= VULKAN_MAX_INTERNAL_LOG_BYTES){
    vulkanFlushInternalLogs();
  }

  std::time_t time(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
  std::tm* localTime = localtime(&time);
  gInternalLog.append(std::asctime(localTime));
  gInternalLog.append(pCallbackData->pMessage);
  return 0;
}

int vulkanErrorsInit(){
  if(!std::filesystem::exists(gInternalLogFilePath)){
    return 1;
  }


  gInternalLog.reserve(VULKAN_MAX_INTERNAL_LOG_BYTES);
  gInternalLogFileStream.open(gInternalLogFilePath);
  gInternalLogFileStream.close();

  return 0;
}
