#define GL_SILENCE_DEPRECATION

#if __APPLE__
#include "glad/glad.h"
#endif

struct GLBuffer{
  unsigned int bufferID;
  int type;
};
