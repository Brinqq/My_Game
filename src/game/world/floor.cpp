#include "floor.h"
#include "renderer.h"

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
