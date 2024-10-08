
#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "elements.hpp"

class Cube : public mygldraw::Elements {
public:
  Cube();

protected:
  void drawMainloop() override;

  void prepareShader() override;

  void prepareTexture() override { Log << "unused" << std::endl; };

  void linkShader() override;

  void programDataSet() override;

  void prepareBuffer() override;

  void shaderClean() override;

  void updateVertices() override;

private:
  unsigned VBO_ = -1;
  unsigned VAO_ = -1;
  unsigned EBO_ = -1;

  unsigned vertex_shader_ = -1;
  unsigned frag_shader_ = -1;
  unsigned shader_program_ = -1;

  // 在这里添加其他的点， 并且制定buffer， 显然可以得到更多的三角形
  std::unique_ptr<float[]> dynamic_vertices;
  static constexpr float static_vertices[] = {
      /*point1*/ -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, 0.0f, /*color*/
      /*point2*/ -0.5f, 0.5f,  -0.5f, 0.0f, 0.0f, 0.5f, /*color*/
      /*point3*/ 0.5f,  0.5f,  -0.5f, 0.0f, 0.5f, 0.0f, /*color*/
      /*point4*/ 0.5f,  0.5f,  0.5f,  0.0f, 0.5f, 0.5f, /*color*/
      /*point5*/ -0.5f, -0.5f, 0.5f,  0.5f, 0.0f, 0.0f, /*color*/
      /*point6*/ -0.5f, -0.5f, -0.5f, 0.5f, 0.0f, 0.5f, /*color*/
      /*point7*/ 0.5f,  -0.5f, -0.5f, 0.5f, 0.5f, 0.0f, /*color*/
      /*point8*/ 0.5f,  -0.5f, 0.5f,  0.5f, 0.0f, 0.5f, /*color*/
  };
  static constexpr int vertices_size =
      sizeof(static_vertices) / sizeof(static_vertices[0]);
  static constexpr int indices[] = {
      // totally 6 * 2 triangles
      /*面1234:*/ 0, 2, 1, 0, 3, 2,
      /*面1265:*/ 0, 1, 4, 1, 5, 4,
      /*面1485:*/ 0, 7, 3, 0, 4, 7,

      /*面3487:*/ 2, 3, 7, 2, 7, 6,
      /*面2376:*/ 1, 6, 5, 1, 2, 6,
      /*面5678:*/ 4, 5, 7, 5, 6, 7,
  };
};
