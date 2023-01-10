#pragma once
#include <GL/glew.h>

class Mesh {
 public:
  Mesh();
  ~Mesh();

  void Create(GLfloat* iVertices, unsigned int* iIndices,
              unsigned int iNbVertices, unsigned int iNbIndices);

  void Render();
  void Clear();

 private:
  GLuint mVAO = 0;
  GLuint mVBO = 0;
  GLuint mIBO = 0;
  GLsizei mNbIndices = 0;
};