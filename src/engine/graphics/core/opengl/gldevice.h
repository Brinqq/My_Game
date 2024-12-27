#pragma once

enum BufferDataType{
  GPU_BUFFER_VERTEX_DATA = 0,
  GPU_BUFFER_INDICE_DATA = 1,
};

namespace Math{
struct Vector3f;
}

class GLDevice{
private:
public:
  int initialize();
  void shutdown();
  int newShaderProgramFromSource(unsigned int& program, const char* vertex, const char* pixel);
  int CompileShaderFromSource(unsigned int& shaderid, const char* source, int type);
  int newGPUBufferWithData(unsigned int& id, const void* data, const BufferDataType type, const long bytes);
  void addConstUniform4f(const unsigned int shader, const char* semantic, const void*const data);
  void addConstUniform3f(const unsigned int shader, const char* semantic, Math::Vector3f data);
  int addUniform3f(const unsigned int shader, const char* semantic, Math::Vector3f data);
  int addUniformMat4f(const unsigned int shader, const char* semantic, const float* const data);
  void updateUniformMat4(const unsigned int shader, const unsigned int uniform, const float* const data);
};

