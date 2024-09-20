
#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <common.hpp>
#include <memory>

class Cube {
public:
  void Draw();
  Cube();
  virtual ~Cube();

protected:
  static void prepareFragShader(unsigned &v, char const *src);

  static void prepareVertexShader(unsigned &v, char const *src);

  static void prepareProgram(unsigned &p, unsigned const vshader,
                             unsigned const fshader);

  static void prepareTexture(unsigned &texture, int pixel_type,
                             char const *path) {
    Log << "function unused" << std::endl;
  };

  virtual void prepareShader();

  virtual void prepareTexture() { Log << "unused" << std::endl; };

  virtual void linkShader();

  virtual void programDataSet();

  virtual void processInput(GLFWwindow *w);

  void prepareWindow();

  void prepareGLAD();

  virtual void prepareBuffer();

  virtual void shaderClean();

  virtual void drawMainloop();

  void updateVertices();

protected:
  GLFWwindow *window = nullptr;

  unsigned VBO = -1;
  unsigned VAO = -1;
  unsigned EBO = -1;

  unsigned vertexShader = -1;
  unsigned fragmentShader = -1;
  unsigned shaderProgram = -1;

  static constexpr int SCR_W = 800;
  static constexpr int SCR_H = 600;

private:
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
