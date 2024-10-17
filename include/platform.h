#include "global.h"


#ifdef _WIN32
#define ALLEGRO_NO_MAGIC_MAIN
  int platformStartUp();
  int initialize();
#endif

#ifdef __APPLE__ 
#include "windowimpl_macos.h"

  struct StateHandles{
    WindowState windowState;
  };
  
  void windowUpdate(WindowState& state);
  int windowCreate(WindowState& state);

  inline StateHandles* pStateHandles;


  inline int platformInitialize(){
    pStateHandles = (StateHandles*)malloc(sizeof(StateHandles));
    windowCreate(pStateHandles->windowState);
    return 0;
  }

  inline void platformGlobalUpdate(){
    windowUpdate(pStateHandles->windowState);
  }
  
  inline void platformDeinitialize(){
    free(pStateHandles);
  }
#endif

