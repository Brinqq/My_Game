#include "global.h"
#include "defines.h"
#include "log.h"
#include "application.h"

int main(void){
#if __DEBUG
  LOG_INFO("Current game build running in debug mode!");
#endif
  applicationStart();
  return 0;
}

