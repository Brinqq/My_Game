#include "global.h"
#include "defines.h"
#include "log.h"
#include "application.h"
#include "sysinfo.h"
#include "unistd.h"

int main(void){
#if __DEBUG
  LOG_INFO("Current game build running in debug mode!");
#endif
  initializeRequiredSystemInfo(1);
  applicationStart();
  return 0;
}

