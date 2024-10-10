#include <cstddef>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "solar-sys.hpp"
#include "GLFW/glfw3.h"
#include "elements.hpp"
#include "circle.hpp"

void SolarSystem::shaderClean() {
  glDeleteShader(vertex_shader_);
  glDeleteShader(frag_shader_);
}

void SolarSystem::drawMainloop() {
  // mainloop;
  glEnable(GL_DEPTH_TEST);

  Log << "rendering" << std::endl;

  // keep drawing
  while (!glfwWindowShouldClose(window_)) {

    processInput(window_);
    FrameCount();

    // clear the screen first
    glClearColor(0.2f, 0.2f, 0.2f, 0.8f);
    glClear(GL_COLOR_BUFFER_BIT |
            GL_DEPTH_BUFFER_BIT); // 这里取决于我们希望clear什么,
                                  // 这里我们只关心 color

    // 这里进入画的loop
    programSetMVP(0); // 对于太阳，角速度为0
    // sun_.Draw();
    // 对于多个星星
    for (size_t i = 0; i < planets_.size(); i++) {
      programSetMVP(w_rate_[i]);
      planets_[i].Draw();
    }

    glfwSwapBuffers(window_);
    glfwPollEvents();
  }

  for (auto &p : planets_) {
    p.Clear();
  }
  glDeleteProgram(shader_program_);
}

void SolarSystem::programSetMVP(float const w_rate) {
  // prepare MVP data
  static auto constexpr I = glm::mat4(1.0f);
  auto const rotate_angle = static_cast<float>(glfwGetTime() * w_rate);
  // 这里让绕着太阳所在的点旋转
  static auto const rotate_axle = glm::vec3(0.0f, 0.0f, 1.0f);
  static float const aspect = static_cast<float>(1.0 * width_ / height_);

  // M 绕着 （x_0, y_0)进行旋转
  auto model = I;
  model = glm::translate(
      model, glm::vec3(-planets_.front().X(), -planets_.front().Y(), 0.0f));
  model = glm::rotate(model, rotate_angle, rotate_axle);
  model = glm::translate(
      model, glm::vec3(planets_.front().X(), planets_.front().Y(), 0.0f));

  // V we do not transform
  static auto const view = glm::translate(I, glm::vec3(0.0f, 0.0f, -2.0f));

  // P unchanged
  static auto const projection =
      glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);

  // remeber to use
  glUseProgram(this->shader_program_);

  static auto const model_location =
      glGetUniformLocation(this->shader_program_, "model");
  static auto const view_location =
      glGetUniformLocation(this->shader_program_, "view");
  static auto const projection_location =
      glGetUniformLocation(this->shader_program_, "projection");

  glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(view));
  // note: currently we set the projection matrix each frame, but since the
  // projection matrix rarely changes it's often best practice to set it
  // outside the main loop only once.
  glUniformMatrix4fv(projection_location, 1, GL_FALSE,
                     glm::value_ptr(projection));
}

void SolarSystem::programDataSet() { programSetMVP(0); }

void SolarSystem::prepareBuffer() {
  // 首先prepare Sun的buffer用于测试

  for (auto &p : planets_) {
    p.PreprareBuffer();
  }
}

void SolarSystem::prepareShader() {
  // vertex shader object
  static const char *vertexShaderSource =
#include "circleShader.vert.glsl"
      ;

  // fragment shader object
  static const char *fragmentShaderSource =
#include "circleShader.frag.glsl"
      ;

  prepareVertexShader(this->vertex_shader_, vertexShaderSource);
  prepareFragShader(this->frag_shader_, fragmentShaderSource);
}

void SolarSystem::linkShader() {
  prepareProgram(this->shader_program_, this->vertex_shader_,
                 this->frag_shader_);
}
