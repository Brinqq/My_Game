#include "global.h"
#include "defines.h"
#include "log.h"
#include "application.h"
#include "sysinfo.h"
#include "unistd.h"
#include "pg.h"

int main(void){
initializeRequiredSystemInfo(1);
#if __DEBUG
  LOG_INFO("Current game build running in debug mode!");
  pgTest();
#endif
  applicationStart();
  return 0;
}

