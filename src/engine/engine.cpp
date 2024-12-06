#include "engine.h"
#include "window.h"
#include "log.h"
#include "error.h"
#include "timing.h"

#include "opengl.h"
#include "glpipeline.h"
#include "glview.h"
#include "gldevice.h"
#include "gldefines.h"
#include "platform.h"
#include "math/primitives.h"
#include <glm/gtc/type_ptr.hpp>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

GLDevice g_device;
GLPipeline g_pipeline;
GLView g_view;

static const float triangle[24]{
    -0.5f, -0.5f,  0.5f,  // Bottom-left-front
     0.5f, -0.5f,  0.5f,  // Bottom-right-front
     0.5f,  0.5f,  0.5f,  // Top-right-front
    -0.5f,  0.5f,  0.5f,  // Top-left-front

    // Back face
    -0.5f, -0.5f, -0.5f,  // Bottom-left-back
     0.5f, -0.5f, -0.5f,  // Bottom-right-back
     0.5f,  0.5f, -0.5f,  // Top-right-back
    -0.5f,  0.5f, -0.5f   // Top-left-back
};

static const unsigned indices[36]{
    0, 1, 2,  0, 2, 3,
    4, 5, 6,  4, 6, 7,
    0, 3, 7,  0, 7, 4,
    1, 2, 6,  1, 6, 5,
    2, 3, 7,  2, 7, 6,
    0, 1, 5,  0, 5, 4
};


unsigned int g_buffer;
unsigned int g_indiceBuffer;
unsigned int g_vertexArray;
unsigned int g_shader;

static glm::mat4 view;
static glm::mat4 model;
static glm::mat4 proj;

static unsigned int modelUniform;
static unsigned int viewUniform;
static unsigned int projUniform;

int modelTimePoint;


void l(){
  glEnable(GL_DEPTH_TEST);
  g_device.newGPUBufferWithData(g_buffer, triangle, GPU_BUFFER_VERTEX_DATA, sizeof(triangle));
  g_device.newGPUBufferWithData(g_indiceBuffer, indices, GPU_BUFFER_INDICE_DATA, sizeof(indices));
  g_device.newShaderProgramFromSource(g_shader, "/Users/brinq/.dev/projects/game/resources/shaders/gl/testshadervert.glsl", "/Users/brinq/.dev/projects/game/resources/shaders/gl/testshaderfrag.glsl");
  g_pipeline.newVertexArrayObject(g_vertexArray, g_buffer, g_indiceBuffer, OGL_ATTRIB_P);
  g_device.addConstUniform3f(g_shader, "u_color", {0.9f, 0.3f, 0.4f});

  model = glm::mat4(1.0f);
  view = glm::mat4(1.0f);
  proj = glm::mat4(1.0f);

  view = glm::translate(view, glm::vec3(0.0f, -0.2f, -2.0f));
  proj = glm::perspective(90.0f, (float)1920/1080, 0.5f, -20.0f);

  modelUniform = g_device.addUniformMat4f(g_shader, "u_model", glm::value_ptr(model));
  viewUniform = g_device.addUniformMat4f(g_shader, "u_view", glm::value_ptr(view));
  projUniform = g_device.addUniformMat4f(g_shader, "u_proj", glm::value_ptr(proj));
}

void testUpdateModelTransform(){
  model = glm::rotate(model, glm::radians(1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  g_device.updateUniformMat4(g_shader, modelUniform, glm::value_ptr(model));
}


void engineDeinitialize(){
  g_device.shutdown();
}

void engineInit(){
  timingInitialize();
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); 
  modelTimePoint = globalTimingNewTimePoint();
  l();
}

void testUpdate(){
  frameTimingUpdate();
  if(globalTimePointCheck(modelTimePoint,(float)1/60)){
    testUpdateModelTransform();
  }
  g_pipeline.clearScreen(0.1f, 0.1f, 0.25f);
  g_pipeline.draw(g_shader, g_vertexArray, sizeof(indices), GL_TRIANGLES);
  glfwSwapBuffers(pStateHandles->windowState.windowHandle);
}

