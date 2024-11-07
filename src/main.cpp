#include "global.h"
#include "defines.h"
#include "log.h"
#include "application.h"
#include "sys/mman.h"


#include "vpsyscalls.h"
void memTest();

int main(void){
#if __DEBUG
  LOG_INFO("Current game build running in debug mode!");
#endif
  // applicationStart();
  return 0;
}

