#pragma once

#include "global.h"

namespace GLShader{
int checkCompileStatus(unsigned int shader);
int getShaderData(std::string& data, const char* filePath);
}
