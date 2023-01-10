#include "Mesh.h"

Mesh::Mesh() {}

Mesh::~Mesh() { Clear(); }

void Mesh::Create(GLfloat* iVertices, unsigned int* iIndices,
                  unsigned int iNbVertices, unsigned int iNbIndices) {
  mNbIndices = iNbIndices;

  glGenVertexArrays(1, &mVAO);
  glBindVertexArray(mVAO);

  glGenBuffers(1, &mVBO);
  glBindBuffer(GL_ARRAY_BUFFER, mVBO);
  glBufferData(GL_ARRAY_BUFFER, iNbVertices * sizeof(GLfloat), iVertices,
               GL_STATIC_DRAW);

  glGenBuffers(1, &mIBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, iNbIndices * sizeof(unsigned int),
               iIndices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void Mesh::Render() {
  glBindVertexArray(mVAO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);

  glDrawElements(GL_TRIANGLES, mNbIndices, GL_UNSIGNED_INT, 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void Mesh::Clear() {
  if (mIBO != 0) {
    glDeleteBuffers(1, &mIBO);
    mIBO = 0;
  }

  if (mVBO != 0) {
    glDeleteBuffers(1, &mVBO);
    mVBO = 0;
  }

  if (mVAO != 0) {
    glDeleteVertexArrays(1, &mVAO);
    mVAO = 0;
  }

  mNbIndices = 0;
}
