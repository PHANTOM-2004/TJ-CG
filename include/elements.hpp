#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <common.hpp>
#include <memory>

namespace mygldraw {

class Elements {
public:
  void Draw();
  Elements(int width = SCR_W, int height = SCR_H);
  virtual ~Elements();

private:
  static constexpr int SCR_W = 800;
  static constexpr int SCR_H = 600;

protected:
  int const width_, height_;
  GLFWwindow *window_ = nullptr;

  protected:
  // non-static member
  virtual void prepareShader() = 0;

  virtual void prepareTexture() { Log << "unused" << std::endl; };

  virtual void linkShader() = 0;

  virtual void programDataSet() = 0;

  virtual void updateVertices() = 0;

  virtual void prepareBuffer() = 0;

  virtual void shaderClean() = 0;

  virtual void processInput(GLFWwindow *w);

  virtual void drawMainloop() = 0;

  void prepareWindow();

  void prepareGLAD();

  // static member
  static void prepareFragShader(unsigned &v, char const *src);

  static void prepareVertexShader(unsigned &v, char const *src);

  static void prepareProgram(unsigned &p, unsigned vshader, unsigned fshader);

  static void prepareTexture(unsigned &texture, int pixel_type,
                             char const *path) {
    Log << "function unused" << std::endl;
  };

  static void FrameCount();
};

}; // namespace mygldraw
